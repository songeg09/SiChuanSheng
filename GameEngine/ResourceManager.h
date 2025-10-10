#pragma once
#include "pch.h"

class Texture;
class ResourceManager
{
	SINGLETON(ResourceManager)
private:
	std::map<std::wstring, Texture*> m_MapTexture;
public:
	Texture* LoadTexture(std::wstring _strFileName);
	Texture* FindTexture(const std::wstring& _strKey, const std::wstring& _strRelativePath);
};

