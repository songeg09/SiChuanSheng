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

Texture* ResourceManager::LoadTexture(std::wstring _strFileName)
{
	assert(_strFileName.length() != 0);
	std::wstring strKey = _strFileName.substr(0, _strFileName.length() - 4);

	Texture* pTexture = FindTexture(strKey, _strFileName);
	if (pTexture == nullptr)
	{
		pTexture = new Texture;
		std::wstring strPath = PathManager::GetInstance()->GetContentPath();
		strPath += L"texture\\" + _strFileName;
		pTexture->Load(strPath);
		pTexture->SetKey(strKey);
		pTexture->SetRelativePath(_strFileName);
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