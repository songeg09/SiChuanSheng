#pragma once
#include "pch.h"

class Timer
{
protected:
	// ��� ����
	int m_iOldTime; // �����ð�
	int m_iSecond; // ���ؽð�
	std::function<void()> m_callbackFunc;

public:
	// ������ �Ҹ���
	Timer() {}
	~Timer() {}

	// Ÿ�̸Ӱ� ����
	void SetTimer(int iSecond, std::function<void()> _callbackFunc)
	{
		m_iSecond = iSecond;
		m_iOldTime = clock();
		m_callbackFunc = _callbackFunc;
	}

	// Ÿ�̸� Ȯ��
	bool CheckTimer() // Ÿ�̸� ���
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