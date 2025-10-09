
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
	//��Ʈ�� ����
	m_hBitMap = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	assert(m_hBitMap != nullptr);

	//��Ʈ�ʰ� ������ DC ����
	m_hDC = CreateCompatibleDC(Core::GetInstance()->GetMainDC());

	//��Ʈ�ʰ� DC ����
	HBITMAP hPrevBitMap = (HBITMAP)SelectObject(m_hDC, m_hBitMap);
	DeleteObject(hPrevBitMap);

	GetObject(m_hBitMap, sizeof(BITMAP), &m_BitMapInfo);
}