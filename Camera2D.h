#pragma once
#include <Windows.h>
#include "Vector2.h"

// �̱����� �Ȱɾ�� -> ī�޶� �������� ��� ����
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


	// ������ǥ -> ȭ����ǥ
	Vector2 calculateScreenPosition (const Vector2& position);
	// ȭ����ǥ -> ������ǥ
	Vector2 calculateWorldPosition (const Vector2& position);
	// ������ǥ -> ȭ����ǥ
	static Vector2 calculateScreenPosition (const HWND& _hwnd, const Vector2& position, const Vector2& camPosition);
	// ȭ����ǥ -> ������ǥ
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

// ���� ���п����� ��ǥ�� ���� �Ʒ��� 0,0 ���������� ȭ���� ���� ���� 0,0 �����̴�.
// ���� 2D������ ȭ����ǥ�� ������ǥ�� ��ȯ������ߵȴ�
// 3D�� ����, ����Ʈ���̽����� �ϴ� ����� ã�ƾ� �� �� ����.
// ���� : ī�޶� �ٶ󺸰� �ִ� �߾��� ���� ��ǥ�� �־�ߵ�(camPosition)
// ������ǥ -> ȭ����ǥ
Vector2 camera2D::calculateScreenPosition(const Vector2& position)
{
	Vector2 relativePos = position - centerPosition;
	return windowSize / 2 + Vector2(relativePos.x, -relativePos.y);
}
// ȭ����ǥ -> ������ǥ
Vector2 camera2D::calculateWorldPosition(const Vector2& position)
{
	Vector2 relativePos = position - windowSize / 2;
	return centerPosition + Vector2(relativePos.x, -relativePos.y);
}
// ������ǥ -> ȭ����ǥ
Vector2 camera2D::calculateScreenPosition(const HWND& _hwnd, const Vector2& position, const Vector2& camPosition)
{
	RECT clientRect;
	GetClientRect(_hwnd, &clientRect);
	float width = static_cast<float>(clientRect.right - clientRect.left);
	float height = static_cast<float>(clientRect.bottom - clientRect.top);

	Vector2 relativePos = position - camPosition;
	return Vector2{ width, height } / 2 + Vector2{ relativePos.x, -relativePos.y };
}
// ȭ����ǥ -> ������ǥ
Vector2 camera2D::calculateWorldPosition(const HWND& _hwnd, const Vector2& position, const Vector2& camPosition)
{
	RECT clientRect;
	GetClientRect(_hwnd, &clientRect);
	float width = static_cast<float>(clientRect.right - clientRect.left);
	float height = static_cast<float>(clientRect.bottom - clientRect.top);

	Vector2 relativePos = position - Vector2{ width, height } / 2;
	return camPosition + Vector2{ relativePos.x, -relativePos.y };
}
