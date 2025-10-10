#include "Board.h"
#include "Card.h"
#include "InputManager.h"

Board::Board(HDC _hBackDC)
{
	m_hBackDC = _hBackDC;
}

Board::~Board()
{
}

void Board::Init()
{
	int CardNum = 0;
	for (int y = 0; y < BOARD_HEIGHT; ++y)
	{
		int x = 0;
		while (x < BOARD_WIDTH)
		{
			if (x == 0 || x == BOARD_WIDTH - 1 || y == 0 || y == BOARD_HEIGHT - 1 || CardNum >= TOTAL_CARDS)
			{
				m_Cards[y][x] = std::make_shared<Card>(Vector2(x, y));
				x++;
			}
			else
			{				
				m_Cards[y][x] = std::make_shared<Card>(CardNum, Vector2(x, y));
				m_Cards[y][x + 1] = std::make_shared<Card>(CardNum, Vector2(x + 1, y));
				m_VisibleCards.insert(m_Cards[y][x]);
				m_VisibleCards.insert(m_Cards[y][x + 1]);

				CardNum++;
				x += 2;
			}
		}
	}
}

void Board::PrepareGame()
{
	// 1. 맞췄던 카드들 복구
	for (int y = 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			if (m_Cards[y][x]->GetState() == CARD_STATE::INVISIBLE)
			{
				m_Cards[y][x]->SetState(CARD_STATE::VISIBLE);
				m_VisibleCards.insert(m_Cards[y][x]);
			}
		}
	}

	// 2. 초기 셔플
	InitialShuffle();

	// 3. 값 초기화
	m_SelectedCard1 = nullptr;
	m_SelectedCard2 = nullptr;
}

void Board::InitialShuffle()
{
	int x1, y1, x2, y2;
	for (int i = 0; i < TOTAL_CARDS * 4; ++i)
	{
		x1 = (rand() % (BOARD_WIDTH - 2)) + 1 ;
		y1 = (rand() % (BOARD_HEIGHT - 2)) + 1 ;

		x2 = (rand() % (BOARD_WIDTH - 2)) + 1;
		y2 = (rand() % (BOARD_HEIGHT - 2)) + 1;

		SwapCards(m_Cards[y1][x1], m_Cards[y2][x2]);
	}
}

void Board::InGameShuffle()
{
	// iterator들 캐싱하기
	std::vector< std::set<std::shared_ptr<Card>>::iterator> its;
	its.reserve(m_VisibleCards.size());
	for (std::set<std::shared_ptr<Card>>::iterator it = m_VisibleCards.begin(); it != m_VisibleCards.end(); it++)
		its.push_back(it);

	// 실제 셔플은 여기서 수행
	int index1, index2;
	for (int i = 0; i < its.size() * 4; ++i)
	{
		index1 = rand() % its.size();
		index2 = rand() % its.size();

		if(index1 == index2)
			index2 = (index2 + 1) % its.size();

		SwapCards(*its[index1], *its[index2]);
	}
}

void Board::SwapCards(std::shared_ptr<Card> card1, std::shared_ptr<Card> card2)
{
	Vector2 temp = card1->GetPosition();
	card1->SetPosition(card2->GetPosition());
	card2->SetPosition(temp);

	Vector2 card1Pos = card1->GetPosition();
	Vector2 card2Pos = card2->GetPosition();
	m_Cards[card1Pos.y][card1Pos.x] = card1;
	m_Cards[card2Pos.y][card2Pos.x] = card2;
}

void Board::RenderCards()
{
	for (int y = 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = BOARD_WIDTH-1; x >= 0; --x)
		{
			m_Cards[y][x]->Render(m_hBackDC);
		}
	}
}

UPDATE_RESULT Board::Update()
{
	InputManager::GetInstance()->Update();

	if (InputManager::GetInstance()->GetKeyState(VK_RBUTTON) == KEY_STATE::DOWN)
	{
		InGameShuffle();
		return UPDATE_RESULT::KEEP_PLAYING;
	}
	else if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEY_STATE::DOWN)
	{
		POINT pt = InputManager::GetInstance()->GetCursorPosition();
		
		for (std::shared_ptr<Card> card : m_VisibleCards)
		{
			if (card->Update(pt))
			{
				if (m_SelectedCard1 == nullptr)
				{
					m_SelectedCard1 = card;
				}
				else
				{
					m_SelectedCard1->SetState(CARD_STATE::VISIBLE);
					m_SelectedCard1 = card;

					
				}

				break;
			}
		}
	}
}
