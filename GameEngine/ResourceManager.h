#pragma once
#include "pch.h"

enum class TEXTURE_TYPE
{
	BACK_GROUND,
	BLOCK_0,
	BLOCK_1,
	BLOCK_2,
	BLOCK_3,
	BLOCK_4,
	BLOCK_5,
	BLOCK_6,
	BLOCK_7,
	BLOCK_8,
	BLOCK_MINE,
	BLOCK_FLAG,
	BLOCK_CLOSE,
};

class Texture;
class ResourceManager
{
	SINGLETON(ResourceManager)
private:
	std::map<std::wstring, Texture*> m_MapTexture;
	std::wstring GetTextureFileName(TEXTURE_TYPE _eTextureType);
public:
	Texture* LoadTexture(TEXTURE_TYPE _eTextureType);
	Texture* FindTexture(const std::wstring& _strKey, const std::wstring& _strRelativePath);
};

