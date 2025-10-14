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

bool Board::OutOfRange(const Vector2 _vec2)
{
	return !(0<=_vec2.x && _vec2.x < BOARD_WIDTH && 0<=_vec2.y && _vec2.y < BOARD_HEIGHT);
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
			if (m_Cards[y][x]->HasTexture() && m_Cards[y][x]->GetState() == CARD_STATE::INVISIBLE)
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
	for (int i = 0; i < NUM_OF_SHUFFLES; ++i)
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
	for (int i = 0; i < NUM_OF_SHUFFLES; ++i)
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

bool Board::CardSelection()
{
	UpdateCards();
	if (m_SelectedCard1 == nullptr || m_SelectedCard2 == nullptr) 
		return false;

	if (*m_SelectedCard1 == *m_SelectedCard2 && CanBeReached(m_SelectedCard1, m_SelectedCard2))
	{
		return true;
	}

	m_SelectedCard1->SetState(CARD_STATE::VISIBLE);
	m_SelectedCard1 = m_SelectedCard2;
	return false;
}

bool Board::CanBeReached(std::shared_ptr<Card> card1, std::shared_ptr<Card> card2)
{
	static Vector2 Directions[4] = { {0,-1},{1,0},{0,1},{-1,0} };
	
	std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> PQ;
	// first = turn, second = F
	std::vector<std::vector<std::pair<int, int>>> vecMapBestCost(BOARD_HEIGHT, std::vector<std::pair<int, int>>(BOARD_WIDTH, { INT32_MAX,INT32_MAX }));
	std::map<std::pair<int, Vector2>, std::pair<int, Vector2>> mapBestParent;
	
	AStarNode CurNode = { card1->GetPosition().GetDistance(card2->GetPosition()),0, 0, card1->GetPosition(), Vector2(-1,-1)};
	
	PQ.push(CurNode);

	while (!PQ.empty())
	{
		CurNode = PQ.top();
		PQ.pop();

		if (vecMapBestCost[CurNode.Pos.y][CurNode.Pos.x] < std::make_pair(CurNode.Turn , CurNode.F))
			continue;
		if (CurNode.Pos == card2->GetPosition())
			break;

		for (Vector2 Dir: Directions)
		{
			AStarNode NextNode; 
			NextNode.Pos = CurNode.Pos + Dir;
			NextNode.PrevPos = CurNode.Pos;
			// 갈 수 없는 곳이면 continue
			if (OutOfRange(NextNode.Pos)) continue;
			if (NextNode.Pos != card2->GetPosition() && m_Cards[NextNode.Pos.y][NextNode.Pos.x]->GetState() != CARD_STATE::INVISIBLE) continue;
			
			NextNode.Turn = CurNode.Turn;
			if (CurNode.PrevPos != Vector2(-1,-1) 
				&& abs((NextNode.Pos - CurNode.PrevPos).x) == 1
				&& abs((NextNode.Pos - CurNode.PrevPos).y) == 1)
				NextNode.Turn++;
			if (NextNode.Turn >= 3) continue;

			NextNode.G = CurNode.G + 1;
			NextNode.F = NextNode.G + NextNode.Pos.GetDistance(card2->GetPosition());
			if (vecMapBestCost[NextNode.Pos.y][NextNode.Pos.x] < std::make_pair(NextNode.Turn, NextNode.F))
				continue;
			
			vecMapBestCost[NextNode.Pos.y][NextNode.Pos.x] = std::make_pair(NextNode.Turn, NextNode.F);
			
			if (mapBestParent.find({ NextNode.Turn, NextNode.Pos }) != mapBestParent.end()
				|| mapBestParent[{NextNode.Turn, NextNode.Pos}] > std::make_pair(CurNode.Turn, CurNode.Pos))
			{
				mapBestParent[{NextNode.Turn, NextNode.Pos}] = { CurNode.Turn, CurNode.Pos };
				PQ.push(NextNode);
			}
				
		}
	}

	bool FinishEarly = true;
	for (int i = 0; i < 3; ++i)
	{
		if (mapBestParent.find({i, card2->GetPosition() }) != mapBestParent.end())
		{
			FinishEarly = false;
			break;
		}
	}
	if (FinishEarly) 
		return false;
	
	
	for (int turn = 0; turn < 3; ++turn)
	{
		if (mapBestParent.find({ turn, card2->GetPosition() }) == mapBestParent.end())
			continue;

		int CurTurn = turn;
		std::pair<int, Vector2> Prev = { CurTurn,card2->GetPosition() };
		std::pair<int, Vector2> Cur = mapBestParent[{CurTurn, card2->GetPosition()}];
		std::pair<int, Vector2> Next;

		while (true)
		{
			if (Cur.second == card1->GetPosition())
				break;

			Next = mapBestParent[Cur];

			// 여기서 마킹 해주기
			m_Cards[Cur.second.y][Cur.second.x]->MarkPath(Cur.second - Prev.second, Cur.second - Next.second);

			m_vec2MarkedPaths.push(Cur.second);
			Prev = Cur;
			Cur = Next;
		}
	}

	return true;
}

void Board::UpdateCards()
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
				m_SelectedCard2 = card;
			}
			break;
		}
	}
}

void Board::HandleCorrect()
{
	Vector2 vec2Pos;
	while (!m_vec2MarkedPaths.empty())
	{
		vec2Pos = m_vec2MarkedPaths.front();
		m_vec2MarkedPaths.pop();
		
		m_Cards[vec2Pos.y][vec2Pos.x]->SetState(CARD_STATE::INVISIBLE);
	}

	m_SelectedCard1->SetState(CARD_STATE::INVISIBLE);
	m_SelectedCard2->SetState(CARD_STATE::INVISIBLE);

	m_VisibleCards.erase(m_SelectedCard1);
	m_VisibleCards.erase(m_SelectedCard2);

	m_SelectedCard1 = nullptr;
	m_SelectedCard2 = nullptr;
}

bool Board::WinCheck()
{
	return m_VisibleCards.size() == 0;
}


