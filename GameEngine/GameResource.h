#pragma once
#include "pch.h"
class GameResource
{
private:
	std::wstring m_strKey;			//���ҽ� Ű
	std::wstring m_strRelativePath;	//���ҽ� �����
public:
	void SetKey(const std::wstring _strKey) { m_strKey = _strKey; }
	void SetRelativePath(const std::wstring _strRelativePath) { m_strRelativePath = _strRelativePath; }

	const std::wstring& GetKey() { return m_strKey; }
	const std::wstring& GetRelativePath() { return m_strRelativePath; }
	GameResource();
	~GameResource();
};

