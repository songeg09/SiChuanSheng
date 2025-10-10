#pragma once
#include "pch.h"

enum UPDATE_RESULT
{
	KEEP_PLAYING
};

class Card;

class Board
{
private:
	HDC m_hBackDC;

	std::shared_ptr<Card>			m_Cards[BOARD_HEIGHT][BOARD_WIDTH];
	std::set<std::shared_ptr<Card>> m_VisibleCards;

	std::shared_ptr<Card>			m_SelectedCard1;
	std::shared_ptr<Card>			m_SelectedCard2;

public:
	void Init();

	void PrepareGame();
	void InitialShuffle();
	void InGameShuffle();
	void SwapCards(std::shared_ptr<Card> card1, std::shared_ptr<Card> card2);

	UPDATE_RESULT Update();
	void RenderCards();

	Board(HDC _hBackDC);
	~Board();
};


