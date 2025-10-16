#pragma once

enum CARD_STATE
{
	VISIBLE,
	SELECTED,
	INVISIBLE,

	PATH_HORIZONTAL,
	PATH_VERTICAL,
	PATH_UPTOLEFT,
	PATH_UPTORIGHT,
	PATH_DOWNTOLEFT,
	PATH_DOWNTORIGHT,
};

class Texture;
class Card
{
private:
	Texture*	m_pTexture;
	Texture*	m_pTextureSelected;
	RECT        m_ClickArea;
	Vector2     m_vec2Position;

	CARD_STATE	m_State;

public:
	Card(int _iTextureNum, Vector2 _vec2Position);
	Card(Vector2 _vec2Position);

	bool Update(const POINT& pt);
	void Render(HDC _hDC);

	bool operator==(const Card _other);

	void MarkPath( Vector2 inVec,  Vector2 outVec);

	bool HasTexture() { return m_pTexture != nullptr; }

	// Test¿ë
	bool SelectCard(POINT pt);

	// Getters & Setters
	CARD_STATE GetState() { return m_State; }
	Vector2 GetPosition() { return m_vec2Position; }
	RECT GetClickArea() { return m_ClickArea; }

	void SetState(CARD_STATE _State) { m_State = _State; }
	void SetPosition(Vector2 _newPos);

};


