#pragma once
class Texture;
class Card
{
private:
	Vector2 m_vec2Position;
	RECT m_rectClickArea;
	Texture* m_pTexture;
	bool m_bFaceUp;

public:
	Card();
	~Card();
	bool Update();
	void Render(HDC _hDC);
	void Init(Vector2 _vec2Position, Texture* _pTexture);

	Vector2 GetPosition() { return m_vec2Position; }
	void SetPosition(Vector2 _vec2Position); 
	void SetFaceUp(bool _bFaceUp) {m_bFaceUp = _bFaceUp;}
	bool GetFaceUp() { return m_bFaceUp; }

	bool operator==(const Card& _other) const;

};

