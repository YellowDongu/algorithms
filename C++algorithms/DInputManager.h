#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")

#include <dinput.h>
#include <thread>
#include <vector>

#pragma comment (lib, "dinput8.lib")

// directX에서 제공되는 기능을 이용한 입력장치 상태 확인
// directX 세팅을 하고서 해야된다

enum class KeyType
{
	Escape = DIK_ESCAPE
	, K1 = DIK_1
	, K2 = DIK_2
	, K3 = DIK_3
	, K4 = DIK_4
	, K5 = DIK_5
	, K6 = DIK_6
	, K7 = DIK_7
	, K8 = DIK_8
	, K9 = DIK_9
	, K0 = DIK_0
	, Minus = DIK_MINUS
	, Equal = DIK_EQUALS
	, Back = DIK_BACK
	, Tab = DIK_TAB
	, Q = DIK_Q
	, W = DIK_W
	, E = DIK_E
	, R = DIK_R
	, T = DIK_T
	, Y = DIK_Y
	, U = DIK_U
	, I = DIK_I
	, O = DIK_O
	, P = DIK_P
	, LBracket = DIK_LBRACKET
	, RBracket = DIK_RBRACKET
	, Return = DIK_RETURN
	, LCtrl = DIK_LCONTROL
	, A = DIK_A
	, S = DIK_S
	, D = DIK_D
	, F = DIK_F
	, G = DIK_G
	, H = DIK_H
	, J = DIK_J
	, K = DIK_K
	, L = DIK_L
	, Semicolon = DIK_SEMICOLON
	, Apostrophe = DIK_APOSTROPHE
	, Grave = DIK_GRAVE
	, LShift = DIK_LSHIFT
	, BackSlash = DIK_BACKSLASH
	, Z = DIK_Z
	, X = DIK_X
	, C = DIK_C
	, V = DIK_V
	, B = DIK_B
	, N = DIK_N
	, M = DIK_M
	, Comma = DIK_COMMA
	, Period = DIK_PERIOD
	, Slash = DIK_SLASH
	, Rshift = DIK_RSHIFT
	, Multiply = DIK_MULTIPLY
	, LMenu = DIK_LMENU
	, Space = DIK_SPACE
	, Capital = DIK_CAPITAL
	, F1 = DIK_F1
	, F2 = DIK_F2
	, F3 = DIK_F3
	, F4 = DIK_F4
	, F5 = DIK_F5
	, F6 = DIK_F6
	, F7 = DIK_F7
	, F8 = DIK_F8
	, F9 = DIK_F9
	, F10 = DIK_F10
	, NUMLOCK = DIK_NUMLOCK
	, SCROLL = DIK_SCROLL
	, NUMPAD7 = DIK_NUMPAD7
	, NUMPAD8 = DIK_NUMPAD8
	, NUMPAD9 = DIK_NUMPAD9
	, SUBTRACT = DIK_SUBTRACT
	, NUMPAD4 = DIK_NUMPAD4
	, NUMPAD5 = DIK_NUMPAD5
	, NUMPAD6 = DIK_NUMPAD6
	, ADD = DIK_ADD
	, NUMPAD1 = DIK_NUMPAD1
	, NUMPAD2 = DIK_NUMPAD2
	, NUMPAD3 = DIK_NUMPAD3
	, NUMPAD0 = DIK_NUMPAD0
	, DECIMAL = DIK_DECIMAL
	, OEM_102 = DIK_OEM_102
	, F11 = DIK_F11
	, F12 = DIK_F12
	, F13 = DIK_F13
	, F14 = DIK_F14
	, F15 = DIK_F15
	, KANA = DIK_KANA
	, CONVERT = DIK_CONVERT
	, NOCONVERT = DIK_NOCONVERT
	, YEN = DIK_YEN
	, NUMPADEQUALS = DIK_NUMPADEQUALS
	, PREVTRACK = DIK_PREVTRACK
	, AT = DIK_AT
	//, Close = DIK_CLOSE
	//, Copy = DIK_COPY
	//, Paste = DIK_PASTE
	//, Print = DIK_PRINT
	, Home = DIK_HOME
	, UP = DIK_UP
	, PGUP = DIK_PGUP
	, LEFT = DIK_LEFT
	, RIGHT = DIK_RIGHT
	, END = DIK_END
	, DOWN = DIK_DOWN
	, PGDN = DIK_PGDN
	, INSERT = DIK_INSERT
	, Delete = DIK_DELETE
	//, LMETA = DIK_LMETA
	//, RMETA = DIK_RMETA
	, APPS = DIK_APPS
};

enum MouseKey
{
	LButton,
	RButton,
	MButton,
	Button1,
	//Button2,
	END
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




class InputManager
{
private:
	InputManager(void) {}
	~InputManager(void) {}

	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

public:
	static InputManager* Init(HINSTANCE hInstance, HWND _hwnd);
	void update(void);
	void release(void);
	void infiniteUpdate(void);
	void threadUpdate(void);

	bool getButton(KeyType key) { return getState(key) == KeyState::Press; }
	bool getButtonDown(KeyType key) { return getState(key) == KeyState::Down; }
	bool getButtonUp(KeyType key) { return getState(key) == KeyState::Up; }

	bool getMouseButton(MouseKey key) { return getMouseState(key) == KeyState::Press; }
	bool getMouseButtonDown(MouseKey key) { return getMouseState(key) == KeyState::Down; }
	bool getMouseButtonUp(MouseKey key) { return getMouseState(key) == KeyState::Up; }

	const POINT& getMousePos(void) const { return mousePos; }
	const int& getMouseWheel(void) const { return mouseWheel; }


private:
	KeyState getState(KeyType key) { return states[static_cast<UINT8>(key)]; }
	KeyState getMouseState(MouseKey key) { return mouseStates[static_cast<int>(key)]; }

	HWND hwnd = 0;
	std::vector<KeyState> states = {};
	std::vector<KeyState> mouseStates = {};
	POINT mousePos = { (0,0) };
	int mouseWheel = 0;

	LPDIRECTINPUT8 dInput = NULL; // DirectInput 객체
	LPDIRECTINPUTDEVICE8 dInputDevice = NULL; // DirectInput 장치 (키보드)
	LPDIRECTINPUTDEVICE8 dMouseDevice = NULL; // DirectInput 장치 (키보드)
	DIMOUSESTATE mouseState = { 0,0,0,0 }; // 마우스 상태


	std::thread* updateThread;
	bool on = true;
	bool off = false;
};


//extern InputManager* Input;

//cpp
//#include "InputManager.h"

InputManager* Input = nullptr;


InputManager* InputManager::Init(HINSTANCE hInstance, HWND _hwnd)
{
	InputManager* input = new InputManager();
	input->hwnd = _hwnd;
	input->states.resize(KEY_TYPE_COUNT, KeyState::None);
	input->mouseStates.resize((int)MouseKey::END, KeyState::None);

	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&input->dInput, NULL)))
	{
		delete input;
		return nullptr;
	}

	// 키보드 장치 생성
	if (FAILED(input->dInput->CreateDevice(GUID_SysKeyboard, &input->dInputDevice, NULL)))
	{
		delete input;
		return nullptr;
	}

	// 마우스 장치 생성
	if (FAILED(input->dInput->CreateDevice(GUID_SysMouse, &input->dMouseDevice, NULL)))
	{
		delete input;
		return nullptr;
	}

	// 장치에 대한 객체를 설정
	if (FAILED(input->dInputDevice->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		delete input;
		return nullptr;
	}

	// 장치에 대한 객체를 설정
	if (FAILED(input->dMouseDevice->SetCooperativeLevel(_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		delete input;
		return nullptr;
	}

	// 키보드 데이터 포맷 설정
	if (FAILED(input->dInputDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		delete input;
		return nullptr;
	}

	// 키보드 데이터 포맷 설정
	if (FAILED(input->dMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		delete input;
		return nullptr;
	}



	return input;
}

void InputManager::update()
{ // 프레임마다 키를 모두 스캔해서 갱신
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	HRESULT hr = dInputDevice->GetDeviceState(sizeof(asciiKeys), (LPVOID)&asciiKeys);
	if (FAILED(hr))
	{
		// 에러 코드에 따라 적절한 처리를 합니다.
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			// 장치가 잃어버리거나 소유권이 없음, 다시 Acquire 시도
			if (FAILED(dInputDevice->Acquire()))
				return;
		}
	}



	for (UINT32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = states[key];
			if (state == KeyState::END) { state = KeyState::None; }
			if (state == KeyState::Press) { continue; }
			else if (state == KeyState::Down)
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

	hr = dMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	if (FAILED(hr))
	{
		// 에러 코드에 따라 적절한 처리를 합니다.
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			// 장치가 잃어버리거나 소유권이 없음, 다시 Acquire 시도
			if (FAILED(dMouseDevice->Acquire()))
				return;
		}
	}


	for (int i = (int)MouseKey::LButton; i < (int)MouseKey::END; i++)
	{
		if (mouseState.rgbButtons[i] & 0x80)
		{
			KeyState& state = mouseStates[i];
			if (state == KeyState::END) { state = KeyState::None; }
			//if (state == KeyState::Press) { continue; }
			else if (state == KeyState::Down)
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
			KeyState& state = mouseStates[i];
			if (state == KeyState::END) { state = KeyState::None; }
			//if (state == KeyState::None) { continue; }
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

	mouseWheel = mouseState.lZ;

	GetCursorPos(&mousePos);
	ScreenToClient(hwnd, &mousePos);

}

void InputManager::release()
{
	if (dInputDevice)
	{
		dInputDevice->Unacquire();
		dInputDevice->Release();
	}
	if (dInput)
	{
		dInput->Release();
	}

	on = false;

	while (!off)
	{

	}
}

void InputManager::infiniteUpdate()
{
	while (on)
	{
		update();
	}
	off = true;
}

void InputManager::threadUpdate()
{
	std::thread newThread(&InputManager::infiniteUpdate, this);
	updateThread = &newThread;
	// 스레드가 종료될 때까지 대기
	newThread.detach();
}
