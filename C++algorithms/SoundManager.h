#pragma once

#include <list>
#include <vector>
#include <string>

#include <Windows.h>
/*
#include <mmsystem.h>
#include <vfw.h>
#pragma comment(lib, "vfw32.lib")
*/

class MCIChannel
{
public:
	MCIChannel() : CurrentSound("NULLSOUND") {}
	~MCIChannel() { stop(); }

	void play(std::string _variableName);
	void stop();
	void volume(int level);
private:
	std::string CurrentSound;

};


void MCIChannel::play(std::string _variableName)
{
	std::string fullCommand = "";
	stop();
	if (_variableName == "NULLSOUND")
	{
		CurrentSound = "NULLSOUND";
		return;
	}
	std::string fullCommand = "play " + _variableName;
	MCIERROR mciErr = mciSendStringA(fullCommand.c_str(), NULL, 0, NULL);
	if (mciErr == 0) CurrentSound = _variableName;
}

void MCIChannel::stop()
{
	if (CurrentSound == "NULLSOUND") return;

	std::string fullCommand = "stop " + CurrentSound;
	mciSendStringA(fullCommand.c_str(), NULL, 0, NULL);
	CurrentSound = "NULLSOUND";
}

void MCIChannel::volume(int level)
{
	std::string fullCommand = "setaudio " + CurrentSound + " volume to ";
	if (level <= 0)
	{
		fullCommand += "0";
	}
	else { fullCommand += level; }
	mciSendStringA(fullCommand.c_str(), NULL, 0, NULL);
}



class MCISoundManager
{
public:
	MCISoundManager() {}
	~MCISoundManager() { release(); }

	void stopAllSound();
	void loadSound(std::string _Path, std::string _variableName);
	void release();
private:
	std::list<MCIChannel*> channelList;

};


void MCISoundManager::stopAllSound()
{
	if (channelList.empty()) return;

	for (auto& sound : channelList)
	{
		sound->stop();
	}

}

void MCISoundManager::loadSound(std::string _Path, std::string _variableName)
{
	std::string fullCommand = "open \"" + _Path + "\" type mpegvideo alias " + _variableName;
	MCIERROR mciErr = mciSendStringA(fullCommand.c_str(), NULL, 0, NULL);
	//if (mciErr != 0) MessageBox();
}

void MCISoundManager::release()
{
	if (channelList.empty()) return;
	for (auto& sound : channelList)
	{
		delete sound;
	}
	channelList.clear();
}

//x64
#include "D:\PJ\C++\FMods\x64\fmod.h"
#include "D:\PJ\C++\FMods\x64\fmod.hpp"
#pragma comment (lib, "D:\PJ\C++\FMods\x64\fmodL_vc.lib")

#include <map>
#include <memory>
#include <unordered_map>

//class Channel
//{
//public:
//	Channel(void) : channel{ nullptr }, sounds{ SoundManager::GetInstance()->sounds }, system{ SoundManager::GetInstance()->fmodSys } {} // 상호참조 방지위해 cpp에 배치하면 됨
//	~Channel(void) = default;
//
//	bool IsPlaying(void) { bool playing; if (channel->isPlaying(&playing) != FMOD_RESULT::FMOD_OK) return false; return playing; }
//	HRESULT Play(const std::string& soundName, bool loop = false)
//	{
//		std::map<std::string, FMOD::Sound*>::iterator sound = sounds.find(soundName);
//		if (sound == sounds.end())
//			return E_FAIL;
//
//		if (system->playSound(sound->second, 0, false, &channel) != FMOD_RESULT::FMOD_OK)
//			return E_FAIL;
//		if (loop)
//		{
//			channel->setMode(FMOD_LOOP_NORMAL);
//			channel->setLoopCount(-1);
//		}
//
//		return S_OK;
//	}
//	void Stop(void) { if (!IsPlaying()) return; channel->stop(); }
//	HRESULT PlayNew(const std::string& soundName, bool loop = false) { Stop(); return Play(soundName, loop); }
//
//	//HRESULT PlayCrossFade(std::wstring nextSoundName);
//	HRESULT LoadSound(const std::string& soundPath, const std::string& soundName) { return SoundManager::GetInstance()->loadSound(soundPath, soundName); }
//	void ChangeVolume(float value)
//	{
//		if (value > 1.0f) value = 1.0f;
//		else if (value < 0.0f) value = 0.0f;
//		channel->setVolume(value);
//	}
//
//private:
//	//friend class SoundManager;
//	FMOD::Channel* channel;
//	std::map<std::string, FMOD::Sound*>& sounds;
//	FMOD::System*& system;
//};

class SoundManager
{
	SoundManager(void) = default;
	~SoundManager(void) = default;
public:
	class Channel // 나중에 분리하도록 -> 구조 상 상호참조가 되어있어 샘플코드만 어쩔 수 없이 이렇게 함
	{
	public:
		Channel(void) : channel{ nullptr }, sounds{ SoundManager::GetInstance()->sounds }, system{ SoundManager::GetInstance()->fmodSys } {} // 상호참조 방지
		~Channel(void) = default;

		bool IsPlaying(void) { bool playing; if (channel->isPlaying(&playing) != FMOD_RESULT::FMOD_OK) return false; return playing; }
		HRESULT Play(const std::string& soundName, bool loop = false)
		{
			std::map<std::string, FMOD::Sound*>::iterator sound = sounds.find(soundName);
			if (sound == sounds.end())
				return E_FAIL;

			if (system->playSound(sound->second, 0, false, &channel) != FMOD_RESULT::FMOD_OK)
				return E_FAIL;
			if (loop)
			{
				channel->setMode(FMOD_LOOP_NORMAL);
				channel->setLoopCount(-1);
			}

			return S_OK;
		}
		void Stop(void) { if (!IsPlaying()) return; channel->stop(); }
		HRESULT PlayNew(const std::string& soundName, bool loop = false) { Stop(); return Play(soundName, loop); }

		//HRESULT PlayCrossFade(std::wstring nextSoundName);
		HRESULT LoadSound(const std::string& soundPath, const std::string& soundName) { return SoundManager::GetInstance()->loadSound(soundPath, soundName); }
		void ChangeVolume(float value)
		{
			if (value > 1.0f) value = 1.0f;
			else if (value < 0.0f) value = 0.0f;
			channel->setVolume(value);
		}

	private:
		//friend class SoundManager;
		FMOD::Channel* channel;
		std::map<std::string, FMOD::Sound*>& sounds;
		FMOD::System*& system;
	};

	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	static std::shared_ptr<SoundManager> Create(void)
	{
		if (instance) return instance;

		instance = std::make_shared<SoundManager>();
		if (FAILED(instance->Start()))
		{
			instance.reset();
			return nullptr;
		}
		return instance;
	}
	static std::shared_ptr<SoundManager> GetInstance(void) { if (instance == nullptr) return Create(); return instance; }
	static void Destroy(void)
	{
		instance->release();
		instance.reset();
	}

	HRESULT Start(void)
	{
		if (FMOD::System_Create(&fmodSys) != FMOD_RESULT::FMOD_OK)
			return E_FAIL;

		// 최대 32채널 사용
		if (fmodSys->init(32, FMOD_INIT_NORMAL, 0) != FMOD_RESULT::FMOD_OK)
			return E_FAIL;

		fmodSys->update();
		return S_OK;
	}
	void Update(void) { fmodSys->update(); }
	void release(void)
	{
		for (auto& channel : channels)
		{
			channel->Stop();
		}
		channels.clear();
		for (auto& sound : sounds)
		{
			sound.second->release();
		}
		sounds.clear();

		fmodSys->close();
		fmodSys->release();
	}


	HRESULT loadSound(const std::string& path, const std::string& fileName)
	{
		FMOD::Sound* newSound = nullptr;
		if (fmodSys->createSound((path + fileName).c_str(), FMOD_DEFAULT, 0, &newSound) != FMOD_RESULT::FMOD_OK)
			return E_FAIL;
		sounds[fileName] = newSound;
		return S_OK;
	}
	std::shared_ptr<Channel> CreateChannel(void)
	{
		std::shared_ptr<Channel> newChannel = std::make_unique<Channel>();
		if(newChannel == nullptr)
			return newChannel;

		channels.push_back(newChannel);
		return newChannel;
	}



private:
	friend class Channel;
	FMOD::System* fmodSys{nullptr};
	std::map<std::string, FMOD::Sound*> sounds;
	std::vector<std::shared_ptr<Channel>> channels;
	static std::shared_ptr<SoundManager> instance;
};

std::shared_ptr<SoundManager> SoundManager::instance{nullptr};



//x86
// 이건 윈API는 직접적으로 쓰진 않지만
// 외부 api라 추가로 깔아야됨
//#include "D:\PJ\C++\FMods\fmod.h"
//#include "D:\PJ\C++\FMods\fmod.hpp"
//#pragma comment(lib, "fmodex_vc.lib")
//
//class SoundManagerx86
//{
//public:
//	SoundManagerx86() : fmodSys() { FMOD_System_Create(&fmodSys); }
//	~SoundManagerx86() { release(); }
//
//	void init();
//	void update() { FMOD_System_Update(fmodSys); }
//	FMOD_SOUND* loadSound(std::string filePath);
//	void play(FMOD_SOUND* sound, FMOD_CHANNEL* channel);
//	void pause(FMOD_CHANNEL* channel, bool pause) { FMOD_Channel_SetPaused(channel, pause); }
//	void stop(FMOD_CHANNEL* channel);
//	void release();
//
//private:
//	std::list<FMOD_SOUND*> soundList;
//	FMOD_CHANNEL* bgmChannel;
//	std::vector<FMOD_CHANNEL*> channelList;
//	FMOD_SYSTEM* fmodSys;
//
//};
//
//void SoundManagerx86::init()
//{
//	FMOD_System_Init(fmodSys, 32, FMOD_INIT_NORMAL, NULL);
//}
//
//FMOD_SOUND* SoundManagerx86::loadSound(std::string filePath)
//{
//	FMOD_SOUND* newSound;
//	FMOD_System_CreateSound(fmodSys, filePath.c_str(), FMOD_DEFAULT, 0, &newSound);
//	return newSound;
//}
//
//void SoundManagerx86::play(FMOD_SOUND* sound, FMOD_CHANNEL* channel)
//{
//	stop(channel);
//	FMOD_System_PlaySound(fmodSys, FMOD_CHANNEL_FREE, sound, false, &channel);
//}
//
//void SoundManagerx86::stop(FMOD_CHANNEL* channel)
//{
//	int condition;
//	FMOD_RESULT result = FMOD_Channel_IsPlaying(channel, &condition);
//
//	if (result != FMOD_OK) return;
//	if (condition) FMOD_Channel_Stop(channel);
//}
//
//void SoundManagerx86::release()
//{
//	if (!channelList.empty())
//	{
//		for (auto& channel : channelList)
//		{
//			stop(channel);
//		}
//	}
//
//	if (!soundList.empty())
//	{
//		for (auto& sound : soundList)
//		{
//			FMOD_Sound_Release(sound);
//		}
//		soundList.clear();
//	}
//	FMOD_System_Release(fmodSys);
//	FMOD_System_Close(fmodSys);
//}

