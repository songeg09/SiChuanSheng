#pragma once
#include <functional>

class Button
{
private:
	HDC m_hDC;
	RECT m_buttonArea;
	std::function<void()> m_callbackFunc;
	bool m_bVisibility;
	std::wstring m_wstrMsg;

public:
	Button(HDC _hDC, RECT _buttonArea, std::wstring _wstrMsg, std::function<void()> _callbackFunc);
	~Button();

	bool Update();
	void Render();
	void SetVisibility(bool _visibility);
};

