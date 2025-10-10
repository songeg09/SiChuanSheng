#include "Core.h"
#include "InputManager.h"
#include "PathManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Board.h"


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

	// �¿� ���콺 ��ư ���
	InputManager::GetInstance()->RegistKey(VK_LBUTTON);
	InputManager::GetInstance()->RegistKey(VK_RBUTTON);

	// ������ ����
	m_pBackGroundTexture = ResourceManager::GetInstance()->LoadTexture(L"back_board.bmp");
	m_WindowSize = Vector2{ m_pBackGroundTexture->GetWidth(),m_pBackGroundTexture->GetHeight() };
	m_WindowStartPosition = Vector2{ (GetSystemMetrics(SM_CXSCREEN) / 2) - (m_WindowSize.x / 2),
												(GetSystemMetrics(SM_CYSCREEN) / 2) - (m_WindowSize.y / 2) };
	SetWindowPos(m_hWnd, nullptr, m_WindowStartPosition.x, m_WindowStartPosition.y,
		m_WindowSize.x + 15, m_WindowSize.y + 60, SWP_SHOWWINDOW);

	// ���ȭ��
	m_hBackBoardBitMap = CreateCompatibleBitmap(m_hDC, m_WindowSize.x, m_WindowSize.y);
	m_hBackDC = CreateCompatibleDC(m_hDC);
	HBITMAP hOldBitMap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBoardBitMap);
	DeleteObject(hOldBitMap);

	// ���� ����
	m_Board = std::make_unique<Board>(m_hBackDC);
	m_Board->Init();

	m_State = GAME_STATE::PREGAME;
}

void Core::GameLoop()
{
	Update();
	Render();
}


void Core::Update()
{
	switch (m_State)
	{
	case GAME_STATE::PREGAME:
		m_Board->PrepareGame();
		m_State = GAME_STATE::PLAY;
		break;

	case GAME_STATE::PLAY:
		m_Board->Update();
		break;
	}

}

void Core::Render()
{
	// 1. ���ȭ�� ������
	BitBlt(m_hBackDC, 0, 0, m_pBackGroundTexture->GetWidth(), m_pBackGroundTexture->GetHeight(), m_pBackGroundTexture->GetDC(), 0, 0, SRCCOPY);
	
	// 2. ī�� ������
	m_Board->RenderCards();
	
	// 3. ���� �� �ð� ������

	// 4. ���� ������
	BitBlt(m_hDC, 0, 0, m_WindowSize.x, m_WindowSize.y, m_hBackDC, 0, 0, SRCCOPY);
}
