#pragma once
const float CHARACTER_MOVE_SPEED = 0.000001f; //�̵��ӵ�
const float MAX_SPEED = 0.05f;//�ִ��̵��ӵ�
const float JUMP_POWER = 0.1f;//������
const float SHOUT = 0.8f;//ź��

class Character
{
private:
	Vector2 m_vec2Position;
	Vector2 m_vec2Force;
	float m_fRadius;
	float m_fSpeed;
	bool m_bDoubleJump;
public:
	Character();
	~Character();
	void Init(Vector2 _vec2Position, float _fRadius, float _fSpeed);
	void Render(HDC _hDC);
	void Update();

};

