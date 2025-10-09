
#include "Texture.h"
#include"Core.h"

Texture::Texture()
{
	m_BitMapInfo = {};
	m_hBitMap = nullptr;
	m_hDC = nullptr;
}

Texture::~Texture()
{
}

void Texture::Load(const std::wstring _strFilePath)
{
	//비트맵 생성
	m_hBitMap = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	assert(m_hBitMap != nullptr);

	//비트맵과 연결할 DC 생성
	m_hDC = CreateCompatibleDC(Core::GetInstance()->GetMainDC());

	//비트맵과 DC 연결
	HBITMAP hPrevBitMap = (HBITMAP)SelectObject(m_hDC, m_hBitMap);
	DeleteObject(hPrevBitMap);

	GetObject(m_hBitMap, sizeof(BITMAP), &m_BitMapInfo);
}