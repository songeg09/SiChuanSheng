#include "Block.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Core.h"
#include "InputManager.h"


Block::Block(bool _IsBomb, Vector2 _pos)
	:m_bIsBomb(_IsBomb), m_iAdjBombs(0), m_State(BLOCK_STATE::CLOSE), m_vec2Position(_pos)
{
	m_ClickArea = {};
	m_pTexture = nullptr;
	if (m_bIsBomb)
	{
		m_pTexture = ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::BLOCK_MINE);
		SetClickArea();
	}
}

Block::~Block()
{
}

void Block::Render(HDC _hDC)
{
	switch (m_State)
	{
	case BLOCK_STATE::VISIBLE:
		BitBlt(_hDC, m_vec2Position.x, m_vec2Position.y, m_pTexture->GetWidth(), m_pTexture->GetHeight(), m_pTexture->GetDC(), 0, 0, SRCCOPY);
		return;
	case BLOCK_STATE::SELECTED:
		BitBlt(_hDC, m_vec2Position.x, m_vec2Position.y, m_pTextureSelected->GetWidth(), m_pTextureSelected->GetHeight(), m_pTextureSelected->GetDC(), 0, 0, SRCCOPY);
		return;
	default:
		return;
	}
}

void Block::SetClickArea()
{
	m_ClickArea = {
		m_vec2Position.x,
		m_vec2Position.y,
		m_vec2Position.x + m_pTexture->GetWidth(),
		m_vec2Position.y + m_pTexture->GetHeight()
	};
}

bool Block::Update(POINT _mousePt)
{
	if (m_State == BLOCK_STATE::SELECTED) return false;

	if (PtInRect(&m_ClickArea, _mousePt))
	{
		// ÁÂÅ¬¸¯½Ã
		if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEY_STATE::DOWN)
		{
			if (m_State == BLOCK_STATE::OPEN || m_State == BLOCK_STATE::FLAGGED) 
				return ACTION_RESULT::ACTION_NOTHING;

			return ACTION_RESULT::ACTION_OPEN;
		}
	}

	return ACTION_RESULT::ACTION_NOTHING;
}
