#include "MouseManager.h"
#include <math.h>

MouseManager::MouseManager()
{

}

MouseManager::~MouseManager()
{

}

void MouseManager::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;
}

void MouseManager::Update()
{
	KeyUpdate();
}

void MouseManager::KeyUpdate()
{
	static constexpr double THRESHOLD = 5.0;
	static POINT prevPt;

	GetCursorPos(&m_ptMouse);
	ScreenToClient(m_hWnd, &m_ptMouse);
	

	// 마우스가 눌려있다
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_mouseLButtonStatus.m_bPress == false)
		{
			m_mouseLButtonStatus.m_bUp = false;
			m_mouseLButtonStatus.m_bDown = true;
			m_mouseLButtonStatus.m_bPress = true;
			prevPt = m_ptMouse;
		}
			
		else
		{
			m_mouseLButtonStatus.m_bDown = false;
			
			if (m_mouseLButtonStatus.m_bDrag == false && GetDistanceBtwnTwoPoints(m_ptMouse, prevPt) >= THRESHOLD)
				m_mouseLButtonStatus.m_bDrag = true;
		}
	}
	else
	{
		if (m_mouseLButtonStatus.m_bPress == true)
		{
			m_mouseLButtonStatus.m_bPress = false;
			m_mouseLButtonStatus.m_bUp = true;
			if (m_mouseLButtonStatus.m_bDrag == false)
				m_mouseLButtonStatus.m_bClick = true;
		}	
		else
		{
			m_mouseLButtonStatus.m_bUp = false;
			m_mouseLButtonStatus.m_bDrag = false;
			m_mouseLButtonStatus.m_bClick = false;
			m_mouseLButtonStatus.m_bDown = false;
			m_mouseLButtonStatus.m_bPress = false;
		}
	}
}

double MouseManager::GetDistanceBtwnTwoPoints(POINT p1, POINT p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}