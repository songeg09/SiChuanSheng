#pragma once
#include "pch.h"

class Card;

class Board
{
private:
	HDC m_hBackDC;

	std::shared_ptr<Card>			m_Cards[BOARD_HEIGHT][BOARD_WIDTH];
	std::set<std::shared_ptr<Card>> m_VisibleCards;
	std::queue<Vector2>				m_vec2MarkedPaths;

	std::shared_ptr<Card>			m_SelectedCard1;
	std::shared_ptr<Card>			m_SelectedCard2;

public:
	void Init(HDC _hBackDC);

	void PrepareGame();

	void PrepareTest();
	void SettingCards();
	void TestIsReady();
	void SelectTestCards();

	void InitialShuffle();
	void InGameShuffle();
	void SwapCards(std::shared_ptr<Card> card1, std::shared_ptr<Card> card2);

	void UpdateCards();
	bool CardSelection();
	bool CanBeReached(std::shared_ptr<Card> card1, std::shared_ptr<Card> card2);

	void HandleCorrect();
	bool WinCheck();

	void RenderCards();

	Board();
	~Board();

	// Getter
	std::shared_ptr<Card> GetSelectedCard1() { return m_SelectedCard1; }
	std::shared_ptr<Card> GetSelectedCard2() { return m_SelectedCard2; }

private:
	bool OutOfRange(const Vector2 _vec2);
};


