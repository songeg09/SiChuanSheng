#pragma once
#include "pch.h"

enum BLOCK_STATE
{
	EMPTY,
	VISIBLE,
	SELECTED,
	INVISIBLE,
};

class Texture;
class Block
{
private:
	BLOCK_STATE m_State;

	Texture*	m_pTexture;
	Texture*	m_pTextureSelected;
	RECT        m_ClickArea;
	Vector2     m_vec2Position;

public:
	Block(bool _IsBomb, Vector2 _pos);
	~Block();

	void Render(HDC _hDC);
	void SetClickArea();

	bool Update(POINT _mousePt);


};

