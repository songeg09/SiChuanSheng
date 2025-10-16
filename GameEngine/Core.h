#pragma once
#include "pch.h"
#include "Timer.h"
#include "Board.h"

const Vector2 Directions[4] = {
	{0,-1},
	{1,0},
	{0,1},
	{ -1,0}
};

enum class GAME_STATE
{
	PRETEST,
	TESTSETUP,
	TESTING,

	PREGAME,
	PLAY,
	CORRECT,
	WIN,
	LOSE
};


class Texture;
class Card;

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
	Timer					m_tTimer;
	Board					m_Board;
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

