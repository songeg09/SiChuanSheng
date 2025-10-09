#include "pch.h"
#include "Card.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "MouseManager.h"

Card::Card()
{
	m_vec2Position = {};
	m_pTexture = nullptr;
}

Card::~Card()
{

}

bool Card::operator==(const Card& _other) const
{
	return m_pTexture->GetKey() == _other.m_pTexture->GetKey();
}

bool Card::Update()
{
	if (m_bFaceUp == true) return false;

	if (PtInRect(&m_rectClickArea, MouseManager::GetInstance()->GetMouseCursor()))
	{
		if (MouseManager::GetInstance()->IsClicked())
		{
			m_bFaceUp = true;
			return true;
		}
	}

	return false;
}

void Card::Init(Vector2 _vec2Position, Texture* _pTexture)
{
	m_vec2Position = _vec2Position;
	m_pTexture = _pTexture;
	m_rectClickArea.left = GetPosition().x - m_pTexture->GetWidth() / 2;
	m_rectClickArea.top = GetPosition().y - m_pTexture->GetHeight() / 2;
	m_rectClickArea.right = m_rectClickArea.left + m_pTexture->GetWidth();
	m_rectClickArea.bottom = m_rectClickArea.top + m_pTexture->GetHeight();
	m_bFaceUp = true;
}

void Card::SetPosition(Vector2 _vec2Position)
{
	m_vec2Position = _vec2Position;
	m_rectClickArea.left = GetPosition().x - m_pTexture->GetWidth() / 2;
	m_rectClickArea.top = GetPosition().y - m_pTexture->GetHeight() / 2;
	m_rectClickArea.right = m_rectClickArea.left + m_pTexture->GetWidth();
	m_rectClickArea.bottom = m_rectClickArea.top + m_pTexture->GetHeight();
}

void Card::Render(HDC _hDC)
{
	Vector2 Position = GetPosition();
	static Texture* BackSide = ResourceManager::GetInstance()->FindTexture(L"µÞ¸é");
	if (m_bFaceUp == true)
	{
		Position.x -= m_pTexture->GetWidth() / 2;
		Position.y -= m_pTexture->GetHeight() / 2;
		BitBlt(_hDC, Position.x, Position.y, m_pTexture->GetWidth(), m_pTexture->GetHeight(), m_pTexture->GetDC(), 0, 0, SRCCOPY);
	}
	else
	{
		Position.x -= BackSide->GetWidth() / 2;
		Position.y -= BackSide->GetHeight() / 2;
		BitBlt(_hDC, Position.x, Position.y, BackSide->GetWidth(), BackSide->GetHeight(), BackSide->GetDC(), 0, 0, SRCCOPY);
	}
	
}
