#pragma once
#include "pch.h"

struct KeyStatus
{
	bool m_bDown;
	bool m_bPress;
	bool m_bUp;
	bool m_bClick;
	bool m_bDrag;
};

class MouseManager
{
	SINGLETON(MouseManager)

private:
	HWND m_hWnd;
	POINT m_ptMouse;
	KeyStatus m_mouseLButtonStatus;

public:
	POINT GetMouseCursor() { return m_ptMouse; }
	bool IsClicked() { return m_mouseLButtonStatus.m_bClick; }

	void Init(HWND _hWnd);
	void Update();
	void KeyUpdate();
	double GetDistanceBtwnTwoPoints(POINT p1, POINT p2);
};

