#pragma once
#include <Windows.h>
#include <vector>
#include "Vector2.h"
#include "ImageHandler.h"
#include "TimeManager.h"


// �̰� ��ӹ޾Ƽ� �ڽĿ��� Init �Լ��� �������ָ� ��(state pattern)
class AnimationState
{
public:
	AnimationState() : animationSpeed(0.0f), timeLapse(0.0f), nextActionTime(0.0f), currentStep(0), nextState(nullptr), imageSize(Vector2Int::zero()) {}
	~AnimationState() { release(); }

	virtual void Init() abstract;

	void update();
	void release();
	void render(HDC _hdc, Vector2 _objectPos);
	void loadImage(std::string filePath, std::vector<Vector2Int> imgPos, Vector2Int imgSize);
	void loadImage(std::string filePath, Vector2Int imgPos, Vector2Int imgSize);


	void setNextState(AnimationState* _nextState) { nextState = _nextState; }

	AnimationState* toNextState() const { return nextState; }
protected:
	std::vector<HBITMAP> imgSet;
	Vector2Int imageSize;
	AnimationState* nextState; // �̰Ŵ� �� Ŭ������ �������� �ְ� �ƴ϶� �����Ҵ� ������ ���⼭ ���ָ� �ȵ�

	float animationSpeed;
	float timeLapse;
	float nextActionTime;
	int currentStep;
};

void AnimationState::release()
{
	while (!imgSet.empty())
	{
		DeleteObject(imgSet.back());
		imgSet.pop_back();
	}
}

void AnimationState::render(HDC _hdc, Vector2 _objectPos)
{
	ImageHandler::renderWithoutBack(imgSet[currentStep], _hdc, (int)_objectPos.x, (int)_objectPos.y);
}

// �ø���ó�� ���� �ð�(timeLapse)�� ������(nextActionTime) ���� �̹���(currentStep,imgSet)�� ������
void AnimationState::update()
{
	timeLapse += Time->deltaTime() * animationSpeed;
	if (nextActionTime <= timeLapse)
	{
		timeLapse = 0.0f;
		currentStep++;
		if (currentStep >= imgSet.size())
		{
			currentStep = 0;
		}
	}
}

// ���� ũ���� �̹����� ���������� �ִ� ū �̹��� �ϳ��� �ҷ��ͼ� �̹����� �ִ� ������ǥ �����̸� ������ �˾Ƽ� ó������
void AnimationState::loadImage(std::string filePath, std::vector<Vector2Int> imgPos, Vector2Int imgSize)
{
	HBITMAP wholeImage = ImageHandler::loadImg(filePath);
	imageSize = imgSize;

	for (int i = 0; i < imgPos.size(); i++)
	{
		Vector2Int startPos = Vector2Int(imgPos[i].x * imgSize.x, imgPos[i].y * imgSize.y);
		imgSet.push_back(ImageHandler::cropImage(wholeImage, startPos, imgSize));
	}
}
// �̹����� ���������� ���� �ʰ� ũ�⵵ �������϶�, imgPos�� �̹����� �ִ� ���� ��ǥ�� ������ ��(������ ������ �ݺ��� �������� �ϼ�)
void AnimationState::loadImage(std::string filePath, Vector2Int imgPos, Vector2Int imgSize)
{
	HBITMAP wholeImage = ImageHandler::loadImg(filePath);
	imageSize = imgSize;

	Vector2Int startPos = Vector2Int(imgPos.x, imgPos.y);
	imgSet.push_back(ImageHandler::cropImage(wholeImage, startPos, imgSize));
}




class AnimationController
{
public:
	virtual void Init() abstract;
	void release();
	void update();
	void render(HDC _hdc, Vector2 _objectPosition);
	void enlistState(AnimationState* state);
	void toNextState();
	void changeState(AnimationState* otherState);

protected:
	std::vector<AnimationState*> states;
	AnimationState* currentState;

};

void AnimationController::release()
{
	if (!states.empty())
	{
		for (int i = 0; i < states.size(); i++)
		{
			if (states[i])
			{
				delete states[i];
				states[i] = nullptr;
			}
		}
	}
}

void AnimationController::update()
{
	if (currentState)
	{
		currentState->update();
	}
}

void AnimationController::render(HDC _hdc, Vector2 _objectPosition)
{
	if (currentState)
	{
		currentState->render(_hdc, _objectPosition);
	}
}

void AnimationController::enlistState(AnimationState* state)
{
	if (!state) { return; }
	for (int i = 0; i < states.size(); i++)
	{
		if (states[i] == state)
		{
			return;
		}
	}
	states.push_back(state);
}

void AnimationController::toNextState()
{
	if (currentState->toNextState())
	{
		changeState(currentState->toNextState());
	}
}

void AnimationController::changeState(AnimationState* otherState)
{
	if (otherState)
	{
		currentState = otherState;
	}
}
