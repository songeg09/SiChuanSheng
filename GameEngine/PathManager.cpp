#include "PathManager.h"

PathManager::PathManager()
{

}

PathManager::~PathManager()
{
	AllocConsole();
}

void PathManager::Init()
{
	GetCurrentDirectory(sizeof(m_szContentsPath), m_szContentsPath);
	int length = lstrlen(m_szContentsPath);
	int i;
	for (i = length - 1; m_szContentsPath[i] != '\\'; --i);
	m_szContentsPath[i] = NULL;
	wcscat_s(m_szContentsPath, L"\\Release\\Resource\\");
}