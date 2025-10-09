#include "Core.h"
#include "Block.h"
#include "InputManager.h"
#include "PathManager.h"
#include "ResourceManager.h"
#include "Texture.h"


Core::Core()
{
	m_hDC = nullptr;
	m_hWnd = nullptr;
	m_WindowStartPosition = {};
	m_WindowSize = {};
	m_pBackGroundTexture = nullptr;
	m_hBackBoardBitMap = nullptr;
	m_hBackDC = nullptr;
}

Core::~Core()
{
	ReleaseDC(m_hWnd, m_hDC);
}

void Core::Init(HWND _hWnd)
{
	srand(time(NULL));
	m_hWnd = _hWnd;
	m_hDC = GetDC(m_hWnd);

	InputManager::GetInstance()->Init();
	PathManager::GetInstance()->Init();

	// 좌우 마우스 버튼 등록
	InputManager::GetInstance()->RegistKey(VK_LBUTTON);
	InputManager::GetInstance()->RegistKey(VK_RBUTTON);

	// 윈도우 세팅
	m_pBackGroundTexture = ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::BACK_GROUND);
	m_WindowSize = Vector2{ m_pBackGroundTexture->GetWidth(),m_pBackGroundTexture->GetHeight() };
	m_WindowStartPosition = Vector2{ (GetSystemMetrics(SM_CXSCREEN) / 2) - (m_WindowSize.x / 2),
												(GetSystemMetrics(SM_CYSCREEN) / 2) - (m_WindowSize.y / 2) };
	SetWindowPos(m_hWnd, nullptr, m_WindowStartPosition.x, m_WindowStartPosition.y,
		m_WindowSize.x + 15, m_WindowSize.y + 60, SWP_SHOWWINDOW);

	// 배경화면
	m_hBackBoardBitMap = CreateCompatibleBitmap(m_hDC, m_WindowSize.x, m_WindowSize.y);
	m_hBackDC = CreateCompatibleDC(m_hDC);
	HBITMAP hOldBitMap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBoardBitMap);
	DeleteObject(hOldBitMap);

	
	// Blocks 세팅
	const int BLOCK_HEIGHT = ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::BLOCK_CLOSE)->GetHeight();
	const int BLOCK_WIDTH = ResourceManager::GetInstance()->LoadTexture(TEXTURE_TYPE::BLOCK_CLOSE)->GetHeight();
	
	int numOfBombs = 0;
	for (int  y= 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			if (numOfBombs < NUM_OF_BOMBS)
			{
				m_blockBoard[y][x] = std::make_shared<Block>(true, Vector2(BOARD_START_X + x* BLOCK_WIDTH, BOARD_START_Y + y * BLOCK_HEIGHT));
				numOfBombs++;
			}
			else
			{
				m_blockBoard[y][x] = std::make_shared<Block>(false, Vector2(BOARD_START_X + x * BLOCK_WIDTH, BOARD_START_Y + y * BLOCK_HEIGHT));
			}
		}
	}

	m_GameState = GAMESTATE::PREGAME;
}

void Core::GameLoop()
{
	Update();
	Render();
}


void Core::UpdateBlocks()
{
	POINT mouseCursor = InputManager::GetInstance()->GetCursorPosition();
	for (int  y = 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			ACTION_RESULT ActionResult = m_blockBoard[y][x]->Update(mouseCursor);
			
			if (ActionResult == ACTION_RESULT::ACTION_NOTHING)
				continue;
		
			if (ActionResult == ACTION_RESULT::ACTION_OPEN)
			{
				if (m_blockBoard[y][x]->IsBomb())
					HandleBombClicked();
				else
					HandleBlockClicked(Vector2(x,y));

				return;
			}

			if (ActionResult == ACTION_RESULT::ACTION_UNFLAG)
			{
				m_iNumOfFlags++;
				return;
			}

			if (ActionResult == ACTION_RESULT::ACTION_FLAG)
			{
				if (m_iNumOfFlags <= 0)
					m_blockBoard[y][x]->Close();
				else
					m_iNumOfFlags--;
				return;
			}
		}
	}
}

void Core::Shuffle()
{
	int x1, y1;
	int x2, y2;
	for (int i = 0; i < 480; ++i)
	{
		x1 = rand() % BOARD_WIDTH;
		y1 = rand() % BOARD_HEIGHT;

		x2 = rand() % BOARD_WIDTH;
		y2 = rand() % BOARD_HEIGHT;

		
		std::shared_ptr<Block> temp = m_blockBoard[y1][x1];
		m_blockBoard[y1][x1] = m_blockBoard[y2][x2];
		m_blockBoard[y2][x2] = temp;

		Vector2 tempVec2 = m_blockBoard[y1][x1]->GetPosition();
		m_blockBoard[y1][x1]->SetPosition(m_blockBoard[y2][x2]->GetPosition());
		m_blockBoard[y2][x2]->SetPosition(tempVec2);
	}
}

void Core::SetBlocks()
{
	for (int y = 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			m_blockBoard[y][x]->Close();

			// 폭탄일 경우 ClickArea만 수정
			if (m_blockBoard[y][x]->IsBomb())
				m_blockBoard[y][x]->SetClickArea();
			// 폭탄이 아닌경우 주변 폭탄 갯수에 따라 텍스쳐도 수정
			else
				m_blockBoard[y][x]->SetupBlock(GetAdjBombs(Vector2(x,y)));
		}
	}
}

int Core::GetAdjBombs(Vector2 _pos)
{
	int Result = 0;
	
	Vector2 AdjPos;
	for (const Vector2& Dir : Directions)
	{
		AdjPos = _pos + Dir;
		if (PositionOutOfBounds(AdjPos)) continue;
		if (m_blockBoard[AdjPos.y][AdjPos.x]->IsBomb())
			Result++;
	}

	return Result;
}

bool Core::PositionOutOfBounds(const Vector2& _pos)
{
	return !(0 <= _pos.x && _pos.x < BOARD_WIDTH && 0 <= _pos.y && _pos.y < BOARD_HEIGHT);
}

void Core::HandleBlockClicked(const Vector2 _vec2)
{
	RevealAdjBlocks(_vec2);
	if (WinCheck())
		m_GameState = GAMESTATE::WIN;
}

void Core::RevealAdjBlocks(const Vector2 _vec2)
{
	static std::queue<Vector2> adjBlocks;

	m_blockBoard[_vec2.y][_vec2.x]->Open();
	adjBlocks.push(_vec2);

	Vector2 Current, Next;
	while (!adjBlocks.empty())
	{
		Current = adjBlocks.front();
		adjBlocks.pop();

		if (m_blockBoard[Current.y][Current.x]->GetAdjBombs() > 0) continue;

		for (int d = 0; d < 8; d += 2)
		{
			Next = Current + Directions[d];

			if (PositionOutOfBounds(Next)) continue;
			if (m_blockBoard[Next.y][Next.x]->GetState() == BLOCK_STATE::OPEN) continue;
			if (m_blockBoard[Next.y][Next.x]->IsBomb()) continue;

			if (m_blockBoard[Next.y][Next.x]->GetState() == BLOCK_STATE::FLAGGED)
				m_iNumOfFlags++;
			m_blockBoard[Next.y][Next.x]->Open();

			if (m_blockBoard[Next.y][Next.x]->GetAdjBombs() == 0)
				adjBlocks.push(Next);
		}
	}
}

bool Core::WinCheck()
{
	static constexpr int TotalBlock = (BOARD_WIDTH * BOARD_HEIGHT) - NUM_OF_BOMBS;

	int Opened = 0;
	for (int y = 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			if (m_blockBoard[y][x]->GetState() == BLOCK_STATE::OPEN)
				Opened++;
		}
	}

	return Opened == TotalBlock;
}



void Core::HandleBombClicked()
{
	RevealBombs();
	m_GameState = GAMESTATE::LOSE;
}

void Core::RevealBombs()
{
	for (int y = 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			if (m_blockBoard[y][x]->IsBomb())
				m_blockBoard[y][x]->Open();
		}
	}
}

void Core::Update()
{
	switch (m_GameState)
	{
	case GAMESTATE::PREGAME:
		m_iNumOfFlags = NUM_OF_BOMBS;
		Shuffle();
		SetBlocks();
		m_GameState = GAMESTATE::PLAY;
		break;
	case GAMESTATE::PLAY:
		InputManager::GetInstance()->Update();
		UpdateBlocks();
		break;
	case GAMESTATE::WIN:
		if (MessageBox(m_hWnd, L"Play Again?", L"You Win!", MB_YESNO) == IDYES)
		{
			m_GameState = GAMESTATE::PREGAME;
			break;
		}
		else
		{
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);
		}
		break;
	case GAMESTATE::LOSE:
		if (MessageBox(m_hWnd, L"Play Again?", L"You Lose!", MB_YESNO) == IDYES)
		{
			m_GameState = GAMESTATE::PREGAME;
			break;
		}
		else
		{
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);
		}
		break;
	}

}

void Core::Render()
{
	BitBlt(m_hBackDC, 0, 0, m_pBackGroundTexture->GetWidth(), m_pBackGroundTexture->GetHeight(), m_pBackGroundTexture->GetDC(), 0, 0, SRCCOPY);
	
	for (int y = 0; y < BOARD_HEIGHT; ++y)
	{
		for (int x = 0; x < BOARD_WIDTH; ++x)
		{
			m_blockBoard[y][x]->Render(m_hBackDC);
		}
	}
	
	std::wstring msg = std::to_wstring(m_iNumOfFlags);
	TextOut(m_hBackDC, FLAGLEFT_START_X, FLAGLEFT_START_Y, msg.c_str(), msg.length());

	BitBlt(m_hDC, 0, 0, m_WindowSize.x, m_WindowSize.y, m_hBackDC, 0, 0, SRCCOPY);
}
