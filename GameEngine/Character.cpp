#include "pch.h"
#include "Character.h"
#include "InputManager.h"
#include "Core.h"
#include <format>

Character::Character()
{
	m_fRadius = 1.0f;
	m_vec2Position = {};
	m_fSpeed = 0.0f;
	m_vec2Force = {};
	m_bDoubleJump = false;
}

Character::~Character()
{

}

void Character::Init(Vector2 _vec2Position, float _fRadius, float _fSpeed)
{
	InputManager::GetInstance()->RegistKey(VK_LEFT);
	InputManager::GetInstance()->RegistKey(VK_RIGHT);
	InputManager::GetInstance()->RegistKey(VK_SPACE);
	m_vec2Position = _vec2Position;
	m_fRadius = _fRadius;
	m_fSpeed = _fSpeed;
}

void Character::Render(HDC _hDC)
{
	std::wstring msg = std::format(L"x = {:.5f}, y = {:.5f}    ", m_vec2Force.x, m_vec2Force.y);
	TextOut(_hDC, 0, 10, msg.c_str(), msg.length());
	Ellipse(_hDC, m_vec2Position.x - m_fRadius, m_vec2Position.y - m_fRadius * 2, m_vec2Position.x + m_fRadius, m_vec2Position.y);
}

void Character::Update()
{
	KEY_STATE eLeftKeyState = InputManager::GetInstance()->GetKeyState(VK_LEFT);
	KEY_STATE eRightKeyState = InputManager::GetInstance()->GetKeyState(VK_RIGHT);

	if (m_vec2Force.y != 0)
	{
		float fFrictionForce = abs(m_vec2Force.x) * WIND_RESISTANCE;
		if (m_vec2Force.x < 0)
		{
			m_vec2Force.x += fFrictionForce;
			if (m_vec2Force.x > 0)
				m_vec2Force.x = 0;
		}
		else if (m_vec2Force.x > 0)
		{
			m_vec2Force.x -= fFrictionForce;
			if (m_vec2Force.x < 0)
				m_vec2Force.x = 0;
		}
	}
	else if (eLeftKeyState == KEY_STATE::DOWN || eLeftKeyState == KEY_STATE::PRESS)
	{
		m_vec2Force.x -= m_fSpeed;
		if (m_vec2Force.x < -MAX_SPEED)
			m_vec2Force.x = -MAX_SPEED;
	}
	else if (eRightKeyState == KEY_STATE::DOWN || eRightKeyState == KEY_STATE::PRESS)
	{
		m_vec2Force.x += m_fSpeed;
		if (m_vec2Force.x > MAX_SPEED)
			m_vec2Force.x = MAX_SPEED;
	}
	else if (m_vec2Force.x != 0 && eLeftKeyState == KEY_STATE::NONE && eRightKeyState == KEY_STATE::NONE)
	{
		float fFrictionForce = abs(m_vec2Force.x) * FRICTION_COEFFICIENT;
		if (m_vec2Force.x < 0)
		{
			m_vec2Force.x += fFrictionForce;
			if (m_vec2Force.x > 0)
				m_vec2Force.x = 0;
		}
		else if (m_vec2Force.x > 0)
		{
			m_vec2Force.x -= fFrictionForce;
			if (m_vec2Force.x < 0)
				m_vec2Force.x = 0;
		}
	}

	KEY_STATE SpaceState = InputManager::GetInstance()->GetKeyState(VK_SPACE);
	if (m_vec2Force.y == 0 && SpaceState == KEY_STATE::DOWN)
	{
		m_vec2Force.y -= JUMP_POWER;
		m_bDoubleJump = true;
	}
	else if (m_bDoubleJump == true && m_vec2Force.y > 0 && SpaceState == KEY_STATE::DOWN)
	{
		m_bDoubleJump = false;
		m_vec2Force.y -= JUMP_POWER * 1.2;
		if (eLeftKeyState == KEY_STATE::PRESS)
		{
			if (m_vec2Force.x > 0)
				m_vec2Force.x = -m_vec2Force.x;
		}
		else if (eRightKeyState == KEY_STATE::PRESS)
		{
			if (m_vec2Force.x < 0)
				m_vec2Force.x = -m_vec2Force.x;
		}
	}

	if (m_vec2Force.y != 0)
		m_vec2Force.y += GRAVITY;

	m_vec2Position += m_vec2Force;
	if (m_vec2Position.y > Core::GetInstance()->GetFloorPositionY())
	{
		m_bDoubleJump = false;
		m_vec2Position.y = Core::GetInstance()->GetFloorPositionY();
		m_vec2Force.y = -(m_vec2Force.y * SHOUT);
		if (abs(m_vec2Force.y) <= 0.0001f)
			m_vec2Force.y = 0.0f;
	}
}