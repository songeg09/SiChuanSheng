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
	m_State = CARD_STATE::INVISIBLE;
	m_pTexture = nullptr;
	m_pTextureSelected = nullptr;
	SetPosition(_vec2Position);
}

void Card::Render(HDC _hDC)
{
	static Texture* pPathHorizontal = ResourceManager::GetInstance()->LoadTexture(L"Line1.bmp");
	static Texture* pPathVertical = ResourceManager::GetInstance()->LoadTexture(L"Line2.bmp");
	static Texture* pPathUpToRight = ResourceManager::GetInstance()->LoadTexture(L"Line3.bmp");
	static Texture* pPathDownToRight = ResourceManager::GetInstance()->LoadTexture(L"Line4.bmp");
	static Texture* pPathDownToLeft = ResourceManager::GetInstance()->LoadTexture(L"Line5.bmp");
	static Texture* pPathUpToLeft = ResourceManager::GetInstance()->LoadTexture(L"Line6.bmp");

	Texture* CurTexture = nullptr;
	switch (m_State)
	{
	case CARD_STATE::VISIBLE:
		CurTexture = m_pTexture;
		break;
	case CARD_STATE::SELECTED:
		CurTexture = m_pTextureSelected;
		break;
	case CARD_STATE::PATH_HORIZONTAL:
		CurTexture = pPathHorizontal;
		break;
	case CARD_STATE::PATH_VERTICAL:
		CurTexture = pPathVertical;
		break;
	case CARD_STATE::PATH_UPTOLEFT:
		CurTexture = pPathUpToLeft;
		break;
	case CARD_STATE::PATH_UPTORIGHT:
		CurTexture = pPathUpToRight;
		break;
	case CARD_STATE::PATH_DOWNTOLEFT:
		CurTexture = pPathDownToLeft;
		break;
	case CARD_STATE::PATH_DOWNTORIGHT:
		CurTexture = pPathDownToRight;
		break;
	}


	if (m_State != CARD_STATE::INVISIBLE)
	{
		static int x, y;
		x = BOARD_START_X + (CurTexture->GetWidth() * m_vec2Position.x) - (MARGIN * m_vec2Position.x);
		y = BOARD_START_Y + (CurTexture->GetHeight() * m_vec2Position.y);

		TransparentBlt(_hDC, x, y, CurTexture->GetWidth(), CurTexture->GetHeight(),
			CurTexture->GetDC(), 0, 0, CurTexture->GetWidth(), CurTexture->GetHeight(), RGB(255, 0, 255));
	}
}

bool Card::operator==(const Card _other)
{
	if (m_pTexture == nullptr || _other.m_pTexture == nullptr) 
		return false;

	return m_pTexture->GetKey() == _other.m_pTexture->GetKey();
}

void Card::MarkPath(const Vector2 dir)
{
	if (abs(dir.x) == 2)
		m_State = CARD_STATE::PATH_HORIZONTAL;
	else if (abs(dir.y) == 2)
		m_State = CARD_STATE::PATH_VERTICAL;
	else if (dir.x == -1 && dir.y == -1)
		m_State = CARD_STATE::PATH_DOWNTOLEFT;
	else if (dir.x == 1 && dir.y == -1)
		m_State = CARD_STATE::PATH_DOWNTORIGHT;
	else if(dir.x == -1 && dir.y == 1)
		m_State = CARD_STATE::PATH_UPTOLEFT;
	else if(dir.x == 1 && dir.y == 1)
		m_State = CARD_STATE::PATH_UPTORIGHT;
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


