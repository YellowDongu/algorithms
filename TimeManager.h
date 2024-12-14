#pragma once
#include "Singleton.h"


//보안에 취약함
// 다만 getTicKCount()보다 보안성능이 낫다
// 하드웨어(아마 마더보드)에서 생성시켜주기 따문에
// os에서 생성시켜주는 getTicKCount보다 접근 난이도가 높음

#include <Windows.h>
class WTimeManager
{
	DECLARE_SINGLETON(WTimeManager);

public:
	void init(void);
	void update(void);

	UINT32 getFPS(void) { return fps; }
	float deltaTime(void) { return deltaT; }
private:
	UINT64 frequency = 0;
	UINT64 prevCount = 0;
	float deltaT = 0.0f; //이전 프레임과 현재 프레임 사이의 시간 간격 (초단위)

	UINT32 frameCount = 0; // 현재 초 동안 프레임 수
	UINT32 fps = 0; //초당 프레임 수(FPS)
	float frameTime = 0.0f;
};

#ifdef Time
#undef Time
#endif
#define Time GET_SINGLETON(WTimeManager)

void WTimeManager::init(void)
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&prevCount));
}

void WTimeManager::update(void)
{
	UINT64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	deltaT = (currentCount - prevCount) / static_cast<float>(frequency);
	prevCount = currentCount;
	frameCount++;
	frameTime += deltaT;
	if (frameTime >= 1.0f)
	{
		fps = static_cast<UINT32>(frameCount / frameTime);

		frameTime = 0.0f;
		frameCount = 0;
	}
}





//windowAPI에 의존적이진 않지만 테스트 안해봄
#include <chrono>

class CronoTimeManager
{
	DECLARE_SINGLETON(CronoTimeManager);
public:
	void Init(void);
	void update(void);

	unsigned int GetFPS(void) { return fps; }
	float deltaTime(void) { return deltaT; }

private:
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	TimePoint prevTime;
	float deltaT = 0.0f; // 이전 프레임과 현재 프레임 사이의 시간 간격 (초단위)

	unsigned int frameCount = 0; // 현재 초 동안 프레임 수
	unsigned int fps = 0; // 초당 프레임 수(FPS)
	float frameTime = 0.0f;
};
#ifdef Time
#undef Time
#endif
#define Time GET_SINGLETON(CronoTimeManager)


void CronoTimeManager::Init(void)
{
	prevTime = Clock::now();
}

void CronoTimeManager::update(void)
{
	TimePoint currentTime = Clock::now();
	std::chrono::duration<float> elapsedTime = currentTime - prevTime;

	deltaT = elapsedTime.count();
	prevTime = currentTime;
	frameCount++;
	frameTime += deltaT;
	if (frameTime >= 1.0f)
	{
		fps = static_cast<unsigned int>(frameCount / frameTime);

		frameTime = 0.0f;
		frameCount = 0;
	}
}













class TimeManager
{
private:
	TimeManager(void);
	~TimeManager(void);
public:
	TimeManager(const TimeManager&) = delete;
	TimeManager(TimeManager&&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;
	TimeManager& operator=(TimeManager&&) = delete;

	static TimeManager& GetInstance(void) { static TimeManager sMgr; return sMgr; }

public:
	void 	Init(void);
	void 	Update(void);
	void	FrameLimit(void);
	float 	GetDeltaTime(void) const { return mDeltaTime; }
	float 	GetUnscaledDeltaTime(void) const { return mUnscaledDeltaTime; }
	float 	GetTimeScale(void) const { return mTimeScale; }
	void 	SetTimeScale(float scale) { mTimeScale = scale; }
	float 	GetUnscaledTime() const { return mUnscaledTime; }
	void 	SetTargetFrameRate(float fps) { mTargetFrameTime = 1.0f / fps; }
	void	SlowMotion(float timeScale, float slowMotionTime) { mTimeScale = timeScale; mSlowMotionTime = slowMotionTime; }

private:
	LARGE_INTEGER 	mFrequency;
	LARGE_INTEGER 	mLastFrameTime;
	LARGE_INTEGER 	mCurrentFrameTime;
	float 			mDeltaTime;
	float 			mUnscaledDeltaTime;
	float 			mTimeScale;
	float 			mUnscaledTime;
	float 			mTargetFrameTime;
	float			mSlowMotionTime;
	double			mAccTime;
	float			mSlowTime;
	int				mFPS;
	TCHAR			m_szFPS[16];
};

#include <thread>

using namespace std::chrono;

TimeManager::TimeManager() : mFrequency(), mLastFrameTime(), mCurrentFrameTime(), mDeltaTime(0), mUnscaledDeltaTime(0),
mTimeScale(1.0f),
mUnscaledTime(0),
mTargetFrameTime(0), mSlowMotionTime(0),
mAccTime(0), mSlowTime(0),
mFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

TimeManager::~TimeManager() = default;

void TimeManager::Init(void)
{
	QueryPerformanceFrequency(&mFrequency);
	QueryPerformanceCounter(&mLastFrameTime);
	mDeltaTime = 0.0f;
	mTimeScale = 1.0f;
	mUnscaledTime = 0.0f;
	mTargetFrameTime = 1.0f / 60.f;
}

void TimeManager::Update(void)
{
	QueryPerformanceCounter(&mCurrentFrameTime);
	LONGLONG elapsedTicks = mCurrentFrameTime.QuadPart - mLastFrameTime.QuadPart;
	double elapsedSeconds = static_cast<double>(elapsedTicks) / static_cast<double>(mFrequency.QuadPart);

	mDeltaTime = static_cast<float>(elapsedSeconds) * mTimeScale;
	mUnscaledDeltaTime = static_cast<float>(elapsedSeconds);
	mUnscaledTime += mUnscaledDeltaTime;

	mAccTime += mUnscaledDeltaTime;
	mSlowTime += mUnscaledDeltaTime;

	++mFPS;

	if (mAccTime >= 1.0)
	{
		swprintf_s(m_szFPS, L"FPS : %d", mFPS);
		//SetWindowText(g_hWnd, m_szFPS);
		// 1초마다 동기화 로직
		mFPS = 0;
		mAccTime = 0;
	}

	if (mSlowTime >= mSlowMotionTime)
	{
		mSlowTime = 0.f;
		mTimeScale = 1.0f;
		mSlowMotionTime = -1.f;
	}

	mLastFrameTime = mCurrentFrameTime;
}

void TimeManager::FrameLimit(void)
{
	QueryPerformanceCounter(&mCurrentFrameTime);
	LONGLONG elapsedTicks = mCurrentFrameTime.QuadPart - mLastFrameTime.QuadPart;
	double elapsedSeconds = static_cast<double>(elapsedTicks) / static_cast<double>(mFrequency.QuadPart);

	// Wait if frame time is shorter than the target frame time
	while (elapsedSeconds < mTargetFrameTime)
	{
		QueryPerformanceCounter(&mCurrentFrameTime);
		elapsedTicks = mCurrentFrameTime.QuadPart - mLastFrameTime.QuadPart;
		elapsedSeconds = static_cast<double>(elapsedTicks) / static_cast<double>(mFrequency.QuadPart);
	}
}
