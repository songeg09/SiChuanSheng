#include "pch.h"
#include "Button.h"
#include "MouseManager.h"

Button::Button(HDC _hDC, RECT _buttonArea, std::wstring _wstrMsg, std::function<void()> _callbackFunc)
: m_hDC(_hDC), 
m_buttonArea(_buttonArea), 
m_wstrMsg(_wstrMsg), 
m_callbackFunc(_callbackFunc), 
m_bVisibility(false)
{
	
}

Button::~Button()
{

}

bool Button::Update()
{
	if (m_bVisibility == false) return false;

	if (MouseManager::GetInstance()->IsClicked() && PtInRect(&m_buttonArea, MouseManager::GetInstance()->GetMouseCursor()))
	{
		m_callbackFunc();
		return true;
	}
	return false;
}

void Button::Render()
{
	if (m_bVisibility == false) return;

	Rectangle(m_hDC, m_buttonArea.left, m_buttonArea.top, m_buttonArea.right, m_buttonArea.bottom);
	TextOut(m_hDC, (m_buttonArea.left + m_buttonArea.right) / 2, (m_buttonArea.bottom + m_buttonArea.top) / 2, m_wstrMsg.c_str(), m_wstrMsg.length());
}

void Button::SetVisibility(bool _visibility)
{
	m_bVisibility = _visibility;
}