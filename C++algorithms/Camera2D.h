#pragma once
#include <Windows.h>
#include "Vector2.h"

// 싱글턴은 안걸어놈 -> 카메라 여러개로 운용 가능
class camera2D
{
public:
	camera2D(void) : camera2D(Vector2::zero()) {}
	camera2D(const Vector2& startPosition) : centerPosition(startPosition), windowSize(Vector2::zero()), camSpeed(17.7f), hWnd(nullptr) {}
	~camera2D(void) = default;


	void initiate(const Vector2&, const HWND&);
	void update(void);
		 
	Vector2 getCamCenterPos(void) const { return centerPosition; }
	Vector2 getWindowSize(void) const { return windowSize; }
		 
	void translate(Vector2 _postion) { centerPosition += _postion; }
	void Position(const Vector2& vector2) { centerPosition = vector2; }


	// 월드좌표 -> 화면좌표
	Vector2 calculateScreenPosition (const Vector2& position);
	// 화면좌표 -> 월드좌표
	Vector2 calculateWorldPosition (const Vector2& position);
	// 월드좌표 -> 화면좌표
	static Vector2 calculateScreenPosition (const HWND& _hwnd, const Vector2& position, const Vector2& camPosition);
	// 화면좌표 -> 월드좌표
	static Vector2 calculateWorldPosition (const HWND& _hwnd, const Vector2& position, const Vector2& camPosition);
private:
	float camSpeed;

	Vector2 centerPosition;
	Vector2 windowSize;

	HWND hWnd;
};

void camera2D::initiate(const Vector2& startPosition, const HWND& _hwnd)
{
	centerPosition = startPosition;
	hWnd = _hwnd;
	update();
}

void camera2D::update(void)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	windowSize = Vector2{ static_cast<float>(clientRect.right - clientRect.left), static_cast<float>(clientRect.bottom - clientRect.top) };
}

// 보통 수학에서의 좌표는 왼쪽 아래가 0,0 지점이지만 화면은 왼쪽 위가 0,0 지점이다.
// 따라서 2D에서는 화면좌표와 월드좌표를 변환시켜줘야된다
// 3D는 별개, 레이트레이싱으로 하는 방법을 찾아야 할 것 같다.
// 전제 : 카메라가 바라보고 있는 중앙의 월드 좌표가 있어야됨(camPosition)
// 월드좌표 -> 화면좌표
Vector2 camera2D::calculateScreenPosition(const Vector2& position)
{
	Vector2 relativePos = position - centerPosition;
	return windowSize / 2 + Vector2(relativePos.x, -relativePos.y);
}
// 화면좌표 -> 월드좌표
Vector2 camera2D::calculateWorldPosition(const Vector2& position)
{
	Vector2 relativePos = position - windowSize / 2;
	return centerPosition + Vector2(relativePos.x, -relativePos.y);
}
// 월드좌표 -> 화면좌표
Vector2 camera2D::calculateScreenPosition(const HWND& _hwnd, const Vector2& position, const Vector2& camPosition)
{
	RECT clientRect;
	GetClientRect(_hwnd, &clientRect);
	float width = static_cast<float>(clientRect.right - clientRect.left);
	float height = static_cast<float>(clientRect.bottom - clientRect.top);

	Vector2 relativePos = position - camPosition;
	return Vector2{ width, height } / 2 + Vector2{ relativePos.x, -relativePos.y };
}
// 화면좌표 -> 월드좌표
Vector2 camera2D::calculateWorldPosition(const HWND& _hwnd, const Vector2& position, const Vector2& camPosition)
{
	RECT clientRect;
	GetClientRect(_hwnd, &clientRect);
	float width = static_cast<float>(clientRect.right - clientRect.left);
	float height = static_cast<float>(clientRect.bottom - clientRect.top);

	Vector2 relativePos = position - Vector2{ width, height } / 2;
	return camPosition + Vector2{ relativePos.x, -relativePos.y };
}
