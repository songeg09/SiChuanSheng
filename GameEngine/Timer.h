#pragma once
#include "pch.h"

class Timer
{
protected:
	// 멤버 변수
	int m_iOldTime; // 이전시간
	int m_iSecond; // 기준시간
	std::function<void()> m_callbackFunc;

public:
	// 생성자 소멸자
	Timer() {}
	~Timer() {}

	// 타이머가 설정
	void SetTimer(int iSecond, std::function<void()> _callbackFunc)
	{
		m_iSecond = iSecond;
		m_iOldTime = clock();
		m_callbackFunc = _callbackFunc;
	}

	// 타이머 확인
	bool CheckTimer() // 타이머 기능
	{
		if (clock() - m_iOldTime >= m_iSecond)
		{
			m_callbackFunc();
			m_iOldTime = clock();
			return true;
		}

		return false;
	}

	// Getter & Setter
	void ChangeSecond(int iSecond)
	{
		m_iSecond = iSecond;
	}
	int GetSecond() { return m_iSecond; }
	int GetOldTime() { return m_iOldTime; }

};