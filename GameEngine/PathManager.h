#pragma once
#include "pch.h"
class PathManager
{
	SINGLETON(PathManager)
private:
	WCHAR m_szContentsPath[512];
public:
	void Init();
	const WCHAR* GetContentPath() { return m_szContentsPath; }
};

