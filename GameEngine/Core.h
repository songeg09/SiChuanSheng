#pragma once
#include"pch.h"

const Vector2 Directions[4] = {
	{0,-1},
	{1,0},
	{0,1},
	{ -1,0}
};

enum class GAME_STATE
{
	PREGAME,
	PLAY,
	CORRECT,
	WIN,
	LOSE
};

class Board;
class Texture;
class Card;
class Timer;

class Core
{
	SINGLETON(Core)
private:
	HWND					m_hWnd;
	HDC						m_hDC;
	Vector2					m_WindowSize;
	Vector2					m_WindowStartPosition;
	Texture*				m_pBackGroundTexture;

	int						m_iScore;
	int						m_iShuffleCount;
	int						m_iTimeLeft;
	std::unique_ptr<Timer>	m_tTimer;
	std::unique_ptr<Board>	m_Board;
	GAME_STATE				m_State;

	HBITMAP					m_hBackBoardBitMap;
	HDC						m_hBackDC;
public:
	void Init(HWND _hWnd);
	void GameLoop();
	HWND GethWnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	Vector2 GetWindowSize() { return m_WindowSize; }

	void DecraseTime();

private:
	void Update();
	void Render();
};

