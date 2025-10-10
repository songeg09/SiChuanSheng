#include "pch.h"
#include "Card.h"
#include "Texture.h"
#include "ResourceManager.h"
#include <format>



Card::Card(int _iTextureNum, Vector2 _vec2Position)
{
	std::wstring strFileName = std::format(L"card_{:02}.bmp", _iTextureNum);
	m_pTexture = ResourceManager::GetInstance()->LoadTexture(strFileName);

	strFileName = std::format(L"card_{:02}_selected.bmp", _iTextureNum);
	m_pTextureSelected = ResourceManager::GetInstance()->LoadTexture(strFileName);

	m_State = CARD_STATE::VISIBLE;

	SetPosition(_vec2Position);
}

Card::Card(Vector2 _vec2Position)
{
	m_State = CARD_STATE::EMPTY;
	m_pTexture = nullptr;
	m_pTextureSelected = nullptr;
	SetPosition(_vec2Position);
}

void Card::Render(HDC _hDC)
{
	static int x, y;
	switch (m_State)
	{
	case CARD_STATE::VISIBLE:
		x = BOARD_START_X + (m_pTexture->GetWidth() * m_vec2Position.x) - (MARGIN * m_vec2Position.x);
		y = BOARD_START_Y + (m_pTexture->GetHeight() * m_vec2Position.y);
		TransparentBlt(_hDC, x, y, m_pTexture->GetWidth(), m_pTexture->GetHeight(),
			m_pTexture->GetDC(), 0, 0, m_pTexture->GetWidth(), m_pTexture->GetHeight(), RGB(255, 0, 255));
		break;

	case CARD_STATE::SELECTED:
		x = BOARD_START_X + (m_pTextureSelected->GetWidth() * m_vec2Position.x) - (MARGIN * m_vec2Position.x);
		y = BOARD_START_Y + (m_pTextureSelected->GetHeight() * m_vec2Position.y);
		TransparentBlt(_hDC, x, y, m_pTextureSelected->GetWidth(), m_pTextureSelected->GetHeight(), 
			m_pTextureSelected->GetDC(), 0,0, m_pTextureSelected->GetWidth(), m_pTextureSelected->GetHeight(), RGB(255,0,255));
		break;
	}
}

bool Card::Update(const POINT& pt)
{
	if (m_State != CARD_STATE::VISIBLE) return false;

	if (PtInRect(&m_ClickArea, pt))
	{
		m_State = CARD_STATE::SELECTED;
		return true;
	}

	return false;
}

void Card::SetPosition(Vector2 _newPos)
{
	m_vec2Position = _newPos;

	if (m_pTexture != nullptr)
	{
		int x = BOARD_START_X + (m_pTexture->GetWidth() * m_vec2Position.x) - (MARGIN * m_vec2Position.x);
		int y = BOARD_START_Y + (m_pTexture->GetHeight() * m_vec2Position.y);
		m_ClickArea = { x,y,x + m_pTexture->GetWidth() , y + m_pTexture->GetHeight() };
	}
	
}


