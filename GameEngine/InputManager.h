#pragma once
#include "pch.h"

enum class KEY_STATE
{
	DOWN,
	PRESS,
	UP,
	NONE,
};

struct KeyInfo
{
	KEY_STATE m_eState;
	bool m_bPrevDown;
	KeyInfo()
	{
		m_eState = KEY_STATE::NONE;
		m_bPrevDown = false;
	}
};

class InputManager
{
	SINGLETON(InputManager)
private:
	std::map<int, KeyInfo> m_mapKeyStatus;
	POINT m_ptCursor;

public:
	void Init();
	void Update();
	void RegistKey(int _iKey);
	KEY_STATE GetKeyState(int _iKey);
	POINT GetCursorPosition() { return m_ptCursor; }
};

