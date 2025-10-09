#pragma once
#include"pch.h"

const Vector2 Directions[4] = {
	{0,-1},
	{1,0},
	{0,1},
	{ -1,0}
};

enum class GAMESTATE
{
	PREGAME,
	PLAY,
	WIN,
	LOSE
};

class Block;
class Texture;
class Core
{
	SINGLETON(Core)
private:
	HWND					m_hWnd;
	HDC						m_hDC;
	Vector2					m_WindowSize;
	Vector2					m_WindowStartPosition;
	Texture*				m_pBackGroundTexture;
	GAMESTATE				m_GameState;
	int						m_iNumOfFlags;

	std::shared_ptr<Block>	m_blockBoard[BOARD_HEIGHT][BOARD_WIDTH];

	HBITMAP					m_hBackBoardBitMap;
	HDC						m_hBackDC;
public:
	void Init(HWND _hWnd);
	void GameLoop();
	HWND GethWnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	Vector2 GetWindowSize() { return m_WindowSize; }
private:
	void UpdateBlocks();
	void Update();
	void Render();

	// 게임 관련 함수들
	void Shuffle();
	void SetBlocks();
	int GetAdjBombs(const Vector2 _pos);
	bool PositionOutOfBounds(const Vector2& _pos);
	void RevealBombs();
	void RevealAdjBlocks(const Vector2 _vec2);

	bool WinCheck();
	void HandleBlockClicked(const Vector2 _vec2);
	void HandleBombClicked();
};

