#include "Core.h"
#include "InputManager.h"
#include "PathManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Board.h"
#include "Windows.h"
#include "Timer.h"


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
	m_pBackGroundTexture = ResourceManager::GetInstance()->LoadTexture(L"back_board.bmp");
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

	SetTextAlign(m_hBackDC, TA_CENTER);

	// 보드 생성 및, 초기 값 설정
	m_Board.Init(m_hBackDC);
	m_tTimer.SetTimer(TIME_SPEED, std::bind(&Core::DecraseTime, this));
	m_State = GAME_STATE::PRETEST;
}

void Core::GameLoop()
{
	Update();
	Render();
}

void Core::DecraseTime()
{
	m_iTimeLeft--;
	if (m_iTimeLeft < 0)
		m_State = GAME_STATE::LOSE;
}

void Core::Update()
{
	switch (m_State)
	{
	case GAME_STATE::PRETEST:
		m_Board.PrepareTest();
		m_State = GAME_STATE::TESTSETUP;
		break;

	case GAME_STATE::TESTSETUP:
		InputManager::GetInstance()->Update();
		if (InputManager::GetInstance()->GetKeyState(VK_RBUTTON) == KEY_STATE::DOWN)
		{
			m_Board.TestIsReady();
			m_State = GAME_STATE::TESTING;
		}
		else if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEY_STATE::DOWN)
		{
			m_Board.SettingCards();
		}
		break;

	case GAME_STATE::TESTING:
		InputManager::GetInstance()->Update();
		if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEY_STATE::DOWN)
		{
			m_Board.SelectTestCards();
		}
		break;

	case GAME_STATE::PREGAME:
		m_iScore = 0;
		m_iShuffleCount = MAX_SHUFFLE_COUNT;
		m_iTimeLeft = MAX_TIME;
		m_tTimer.ResetTimer();
		m_Board.PrepareGame();
		m_State = GAME_STATE::PLAY;
		break;

	case GAME_STATE::PLAY:
		m_tTimer.CheckTimer();
		InputManager::GetInstance()->Update();

		if (InputManager::GetInstance()->GetKeyState(VK_RBUTTON) == KEY_STATE::DOWN && m_iShuffleCount > 0)
		{
			m_Board.InGameShuffle();
			m_iShuffleCount--;
			
		}
		else if (InputManager::GetInstance()->GetKeyState(VK_LBUTTON) == KEY_STATE::DOWN)
		{
			if (m_Board.CardSelection())
				m_State = GAME_STATE::CORRECT;
		}
		break;

	case GAME_STATE::CORRECT:
		Sleep(SHOWING_TIME);
		m_iScore += 10;
		m_iTimeLeft = min(MAX_TIME, m_iTimeLeft + TIMER_PER_BLOCK);
		m_Board.HandleCorrect();
		if (m_Board.WinCheck())
			m_State = GAME_STATE::WIN;
		else
			m_State = GAME_STATE::PLAY;
		break;

	case GAME_STATE::WIN:
		if (MessageBox(m_hWnd, L"Play Again?", L"You Win!", MB_YESNO) == IDYES)
		{
			m_State = GAME_STATE::PREGAME;
			break;
		}
		else
		{
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);
		}
		break;

	case GAME_STATE::LOSE:
		if (MessageBox(m_hWnd, L"Play Again?", L"You Lose!", MB_YESNO) == IDYES)
		{
			m_State = GAME_STATE::PREGAME;
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
	// 1. 배경화면 렌더링
	BitBlt(m_hBackDC, 0, 0, m_pBackGroundTexture->GetWidth(), m_pBackGroundTexture->GetHeight(), m_pBackGroundTexture->GetDC(), 0, 0, SRCCOPY);
	
	// 2. 카드 렌더링
	m_Board.RenderCards();
	
	// 3. 점수 및 남은 셔플 횟수 렌더링
	static std::wstring txt;
	txt = std::to_wstring(m_iScore);
	TextOut(m_hBackDC, SCORE_X, SCORE_Y, txt.c_str(), txt.size());
	
	txt = std::to_wstring(m_iShuffleCount);
	TextOut(m_hBackDC, SHUFFLE_COUNT_X, SHUFFLE_COUNT_Y, txt.c_str(), txt.size());

	// 4. 시간 렌더링
	static Texture* TimerBlock = ResourceManager::GetInstance()->LoadTexture(L"timer_block.bmp");
	static int x;
	for (int i = 0; i <= m_iTimeLeft / TIMER_PER_BLOCK; ++i)
	{
		x = TIMER_START_X + (TimerBlock->GetWidth() * i);

		TransparentBlt(m_hBackDC, x, TIMER_START_Y, TimerBlock->GetWidth(), TimerBlock->GetHeight(),
			TimerBlock->GetDC(), 0, 0, TimerBlock->GetWidth(), TimerBlock->GetHeight(), RGB(255, 0, 255));
	}

	// 4. 최종 렌더링
	BitBlt(m_hDC, 0, 0, m_WindowSize.x, m_WindowSize.y, m_hBackDC, 0, 0, SRCCOPY);
}
