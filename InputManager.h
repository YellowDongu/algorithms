#pragma once
#include <Windows.h>
#include <vector>

#include "Singleton.h"

enum class KeyType
{
	LeftMouse = VK_LBUTTON,
	RightMouse = VK_RBUTTON,

	Up = VK_UP,
	Down = VK_DOWN,
	Left = VK_LEFT,
	Right = VK_RIGHT,
	Space = VK_SPACE,

	Q = 'Q',
	W = 'W',
	E = 'E',
	R = 'R',
	T = 'T',
	Y = 'Y',
	U = 'U',
	I = 'I',
	O = 'O',
	P = 'P',

	A = 'A',
	S = 'S',
	D = 'D',
	F = 'F',
	G = 'G',
	H = 'H',
	J = 'J',
	K = 'K',
	L = 'L',

	Z = 'Z',
	X = 'X',
	C = 'C',
	V = 'V',
	B = 'B',
	N = 'N',
	M = 'M',
	Shift = VK_SHIFT,
	Ctrl = VK_CONTROL,
	Menu = VK_MENU,
	Escape = VK_ESCAPE,
	Caps = VK_CAPITAL,

	Delete = VK_DELETE,
	Insert = VK_INSERT,
	End = VK_END,
	//Pgup = vk_,
	//Pgdn = VK_INSERT,

	F1 = VK_F1,
	F2 = VK_F2,
	F3 = VK_F3,
	F4 = VK_F4,
	F5 = VK_F5,
	F6 = VK_F6,
	F7 = VK_F7,
	F8 = VK_F8,
	F9 = VK_F9,
	F10 = VK_F10,
	F11 = VK_F11,
	F12 = VK_F12,

	NUMPAD0 = VK_NUMPAD0,
	NUMPAD1 = VK_NUMPAD1,
	NUMPAD2 = VK_NUMPAD2,
	NUMPAD3 = VK_NUMPAD3,
	NUMPAD4 = VK_NUMPAD4,
	NUMPAD5 = VK_NUMPAD5,
	NUMPAD6 = VK_NUMPAD6,
	NUMPAD7 = VK_NUMPAD7,
	NUMPAD8 = VK_NUMPAD8,
	NUMPAD9 = VK_NUMPAD9,

};

enum class KeyState
{
	None,
	Down,
	Press,
	Up,

	END
};

enum
{
	KEY_TYPE_COUNT = static_cast<INT32>(UINT8_MAX) + 1,
	KEY_STATE_COUNT = static_cast<INT32>(KeyState::END)
};


/*
매 프레임마다 키 상태를 확인하는 것은 메시지 기반 처리보다 CPU에 부하를 줌
그러나 현대적인 게임 환경에서 이 정도의 오버헤드는 문제 없을정도
게임의 입력 반응성을 높이는 데 더 효과적인 방법이라 인풋매니저를 만들 이유가 있음
*/


class InputManager
{
	DECLARE_SINGLETON(InputManager)

public:
	void Init(HWND _hwnd);
	void update(void);

	bool getButton(KeyType key) { return getState(key) == KeyState::Press; }
	bool getButtonDown(KeyType key) { return getState(key) == KeyState::Down; }
	bool getButtonUp(KeyType key) { return getState(key) == KeyState::Up; }
	const POINT& getMousePos(void) const { return mousePos; }


private:
	const KeyState& getState(KeyType key) const { return states[static_cast<UINT8>(key)]; }

	HWND hwnd = 0;
	std::vector<KeyState> states = {};
	POINT mousePos = {(0,0)};
};

#define Input GET_SINGLETON(InputManager)






void InputManager::Init(HWND _hwnd)
{
	hwnd = _hwnd;
	states.resize(KEY_TYPE_COUNT, KeyState::None);
}

void InputManager::update(void)
{ // 프레임마다 키를 모두 스캔해서 갱신
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (!::GetKeyboardState(asciiKeys)) { return; }

	for (UINT32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = states[key];
			if (state == KeyState::END) { state = KeyState::None; }
			if (state == KeyState::Press) { continue; }

			if (state == KeyState::Down)
			{
				state = KeyState::Press;
			}
			else
			{
				state = KeyState::Down;
			}

		}
		else
		{
			KeyState& state = states[key];
			if (state == KeyState::END) { state = KeyState::None; }
			if (state == KeyState::None) { continue; }

			if (state == KeyState::Up)
			{
				state = KeyState::None;
			}
			else
			{
				state = KeyState::Up;
			}
		}
	}

	GetCursorPos(&mousePos);
	ScreenToClient(hwnd, &mousePos);
}


class PlayerInput
{
	DECLARE_SINGLETON(PlayerInput)

public:
	const int& horizontal(void) const { return horizontalInput; }
	const int& vertical(void) const { return verticalInput; }

	void update(void);
	void changeKeyVMinus(KeyType key) { vMinus = key; }
	void changeKeyvPlus(KeyType key) { vPlus = key; }
	void changeKeyhMinus(KeyType key) { hMinus = key; }
	void changeKeyhPlus(KeyType key) { hPlus = key; }

private:
	int horizontalInput = 0, verticalInput = 0;
	KeyType vMinus = KeyType::S;
	KeyType vPlus = KeyType::W;
	KeyType hMinus = KeyType::A;
	KeyType hPlus = KeyType::D;

	void checkInput(void);
};

#define vhInput GET_SINGLETON(PlayerInput)


 void PlayerInput::update(void)
{
	checkInput();
}

void PlayerInput::checkInput(void)
{
	if (Input->getButton(hMinus))
	{
		horizontalInput = -1;
	}
	else if (Input->getButton(hPlus))
	{
		horizontalInput = 1;
	}
	else
	{
		horizontalInput = 0;
	}

	if (Input->getButton(vMinus))
	{
		verticalInput = -1;
	}
	else if (Input->getButton(vPlus))
	{
		verticalInput = 1;
	}
	else
	{
		verticalInput = 0;
	}
}
