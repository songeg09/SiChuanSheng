#include "ResourceManager.h"
#include "PathManager.h"
#include "Texture.h"


ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	for (std::map<std::wstring, Texture*>::iterator iter = m_MapTexture.begin(); iter != m_MapTexture.end(); iter++)
		delete iter->second;
}

std::wstring ResourceManager::GetTextureFileName(TEXTURE_TYPE _eTextureType)
{
	switch (_eTextureType)
	{
	case  TEXTURE_TYPE::BACK_GROUND:	return L"back_board.bmp";
	case 	TEXTURE_TYPE::BLOCK_0:			return L"block_0.bmp";
	case 	TEXTURE_TYPE::BLOCK_1:			return L"block_1.bmp";
	case 	TEXTURE_TYPE::BLOCK_2:			return L"block_2.bmp";
	case 	TEXTURE_TYPE::BLOCK_3:			return L"block_3.bmp";
	case 	TEXTURE_TYPE::BLOCK_4:			return L"block_4.bmp";
	case 	TEXTURE_TYPE::BLOCK_5:			return L"block_5.bmp";
	case 	TEXTURE_TYPE::BLOCK_6:			return L"block_6.bmp";
	case 	TEXTURE_TYPE::BLOCK_7:			return L"block_7.bmp";
	case 	TEXTURE_TYPE::BLOCK_8:			return L"block_8.bmp";
	case 	TEXTURE_TYPE::BLOCK_MINE:		return L"mine.bmp";
	case 	TEXTURE_TYPE::BLOCK_FLAG:		return L"flag.bmp";
	case 	TEXTURE_TYPE::BLOCK_CLOSE:		return L"block.bmp";
	default: return L"";
	}
}

Texture* ResourceManager::LoadTexture(TEXTURE_TYPE _eTextureType)
{
	std::wstring strFileName = GetTextureFileName(_eTextureType);
	assert(strFileName.length() != 0);
	std::wstring strKey = strFileName.substr(0, strFileName.length() - 4);

	Texture* pTexture = FindTexture(strKey, strFileName);
	if (pTexture == nullptr)
	{
		pTexture = new Texture;
		std::wstring strPath = PathManager::GetInstance()->GetContentPath();
		strPath += L"texture\\" + strFileName;
		pTexture->Load(strPath);
		pTexture->SetKey(strKey);
		pTexture->SetRelativePath(strFileName);
		m_MapTexture.insert(make_pair(strKey, pTexture));
	}
	return pTexture;
}

Texture* ResourceManager::FindTexture(const std::wstring& _strKey, const std::wstring& _strRelativePath)
{
	std::map<std::wstring, Texture*>::iterator iter = m_MapTexture.find(_strKey);
	if (iter == m_MapTexture.end())
		return nullptr;
	else
		return iter->second;
}