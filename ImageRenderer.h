#pragma once
#include <Windows.h>
#include <vector>
#include "Vector2.h"
#include "ImageHandler.h"
#include "TimeManager.h"


// 이걸 상속받아서 자식에서 Init 함수로 세팅해주면 됨(state pattern)
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
	AnimationState* nextState; // 이거는 이 클래스에 종속적인 애가 아니라서 동적할당 해제는 여기서 해주면 안됨

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

// 플립북처럼 일정 시간(timeLapse)이 지나면(nextActionTime) 다음 이미지(currentStep,imgSet)를 보여줌
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

// 같은 크기의 이미지가 연속적으로 있는 큰 이미지 하나를 불러와서 이미지가 있는 순번좌표 뭉탱이를 넣으면 알아서 처리해줌
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
// 이미지가 연속적으로 있지 않고 크기도 가변적일때, imgPos에 이미지가 있는 실제 좌표를 넣으면 됨(여러개 있으면 반복문 돌리든지 하셈)
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
