// Tic-Tac-Toe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Tic-Tac-Toe.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

const int cell_size = 100;
int playerTurn = 1;
int index, winner;
int wins[3];
int gameBoard[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

HBRUSH BLUE_BRUSH, RED_BRUSH;
HICON XIcon, OIcon;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TICTACTOE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TICTACTOE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

BOOL GetGameBoardRect(HWND hwnd, RECT *pRect) {
	RECT rc;
	if (GetClientRect(hwnd, &rc)) {
		int width = rc.right - rc.left, height = rc.bottom - rc.top;
		pRect->left = (width - cell_size * 3) / 2;
		pRect->top = (height - cell_size * 3) / 2;
		pRect->right = pRect->left + cell_size * 3;
		pRect->bottom = pRect->top + cell_size * 3;

		return TRUE;
	}

	SetRectEmpty(pRect);
	return FALSE;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TICTACTOE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TICTACTOE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

int GetCellNumberFromPoint(HWND hwnd, int x, int y) {
	POINT pt = { x, y };
	RECT rc;

	if (GetGameBoardRect(hwnd, &rc)) {
		if (PtInRect(&rc, pt)) {
			//User clicked
			//Normalized ( 0 to 3 * cell_size)
			x = pt.x - rc.left;
			y = pt.y - rc.top;

			int column = x / cell_size;
			int row = y / cell_size;

			//convert to index (0 to 8)
			return column + row * 3;
		}
	}

	return -1;
}

BOOL GetCellRect(HWND hWnd, int index, RECT *pRect) {
	RECT rcBoard;

	SetRectEmpty(pRect);

	if (index < 0 || index > 8) {
		return FALSE;
	}

	if (GetGameBoardRect(hWnd, &rcBoard)) {
		//Convert index from 0 to 8

		int y = index / 3; //Row number
		int x = index % 3; //Col number

		pRect->left = rcBoard.left + x * cell_size + 1;
		pRect->top = rcBoard.top + y * cell_size + 1;

		pRect->right = pRect->left + cell_size - 1;
		pRect->bottom = pRect->top + cell_size - 1;

		return TRUE;
	}

	return FALSE;
}

/*
	Returns int
	0 - No Winner
	1 - Player 1 wins
	2 - Player 2 wins
	3 - Tie
*/

/*
	0,1,2
	3,4,5
	6,7,8
*/
int GetWinner(int win[3]) {
	int cells[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 3, 6, 1, 4, 7, 2, 5, 8, 0, 4, 8, 2, 4, 6};

	for (int i = 0; i < ARRAYSIZE(cells); i += 3) {
		if (gameBoard[cells[i]] == gameBoard[cells[i + 1]] && gameBoard[cells[i]] == gameBoard[cells[i + 2]]) {
			win[0] = cells[i];
			win[1] = cells[i + 1];
			win[2] = cells[i + 2];

			return gameBoard[cells[i]];
		}
	}

	for (int i = 0; i < ARRAYSIZE(gameBoard); ++i) {
		if (gameBoard[i] == 0) {
			return 0; // continue to play
		}
	}

	return 3;
}

void ShowTurn(HWND hwnd, HDC hdc) {
	RECT rc;
	const WCHAR turn1[] = L"Player 1 Turn";
	const WCHAR turn2[] = L"Player 2 Turn";

	const WCHAR* TurnText;

	switch (winner) {
	case 0:
		TurnText = (playerTurn % 2 == 1) ? turn1 : turn2;
		break;
	case 1:
		TurnText = L"Player 1 is the winner!";
		break;
	case 2:
		TurnText = L"Player 2 is the winner!";
		break;
	case 3:
		TurnText = L"It's a draw";
		break;
	}

	if (TurnText != NULL && GetClientRect(hwnd, &rc)) {
		rc.top = rc.bottom - 48;

		FillRect(hdc, &rc, (HBRUSH)GetStockObject(GRAY_BRUSH));
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, TurnText, lstrlen(TurnText), &rc, DT_CENTER | DT_VCENTER);
	}
}

void DrawIconCentered(HDC hdc, RECT* pRect, HICON hicon) {
	const int ICON_WIDTH = 32;
	const int ICON_HEIGHT = 32;

	if (pRect != NULL) {
		int left = pRect->left + ((pRect->right - pRect->left) - 32) / 2, top = pRect->top + ((pRect->bottom - pRect->top) - 32) / 2;
		DrawIcon(hdc, left, top, hicon);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE: 
		BLUE_BRUSH = CreateSolidBrush(RGB(0, 0, 255));
		RED_BRUSH = CreateSolidBrush(RGB(255, 0, 0));

		XIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_P111));
		OIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_P11));
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_FILE_NEWGAME: {
					int ret = MessageBox(hWnd, L"Are you sure you want to start a new game?", L"New Game", MB_YESNO | MB_ICONQUESTION);

					if (ret == IDYES) {
						playerTurn = 1;
						winner = 0;
						ZeroMemory(gameBoard, sizeof(gameBoard));
						InvalidateRect(hWnd, NULL, TRUE); // Post WM_PAINT to our windowProc
						UpdateWindow(hWnd); //forces immediate handling of WM_PAINT
					}
				}
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_LBUTTONDOWN: {
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		if (playerTurn == 0) {
			break;
		}

		index = GetCellNumberFromPoint(hWnd, xPos, yPos);
		
		HDC hdc = GetDC(hWnd);

		if (hdc != NULL) {
			WCHAR temp[100];
			wsprintf(temp, L"Index = %d", index);
			//TextOut(hdc, xPos, yPos, temp, lstrlen(temp));

			if (index != -1) {
				RECT rcCell;
				
				if (gameBoard[index] == 0 && GetCellRect(hWnd, index, &rcCell)) {
					playerTurn % 2 == 0 ? gameBoard[index] = 2 : gameBoard[index] = 1;
					DrawIcon(hdc, rcCell.left, rcCell.top, playerTurn % 2 == 1 ? XIcon : OIcon);
					winner = GetWinner(wins);

					if (winner == 1 || winner == 2) {
						MessageBox(hWnd, (winner == 2) ? L"Player 2 is the winner" : L"Player 1 is the winner", L"You Win", MB_OK | MB_ICONINFORMATION);
						playerTurn = 0;
					}

					else if (winner == 3) {
						MessageBox(hWnd, L"No one wins this time", L"it's a tie", MB_OK | MB_ICONEXCLAMATION);
						playerTurn = 0;
					}

					else if (winner == 0) {
						++playerTurn;
					}
					ShowTurn(hWnd, hdc);
				}
			}
			ReleaseDC(hWnd, hdc);
		}
	}
	break;
	case WM_GETMINMAXINFO: {
			MINMAXINFO* pMinMax = (MINMAXINFO*)lParam;

			pMinMax->ptMinTrackSize.x = cell_size * 5;
			pMinMax->ptMinTrackSize.y = cell_size * 5;
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			RECT rc;
			if (GetGameBoardRect(hWnd, &rc)) {

				RECT rcClient;

				if (GetClientRect(hWnd, &rcClient)) {
					//Draw player one and player 2
					const WCHAR szP1[] = L"Player 1";
					const WCHAR szP2[] = L"Player 2";

					SetBkMode(hdc, TRANSPARENT);

					SetTextColor(hdc, RGB(255, 0, 0));
					TextOut(hdc, 16, 16, szP1, ARRAYSIZE(szP1));

					SetTextColor(hdc, RGB(0, 0, 255));
					TextOut(hdc, rcClient.right - 72, 16, szP2, ARRAYSIZE(szP1));

					ShowTurn(hWnd, hdc);
				}

				FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
				//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

				for (int i = 1; i < 3; ++i) {

					//Draw verticle line
					DrawLine(hdc, rc.left + cell_size * i, rc.top, rc.left + cell_size * i, rc.bottom);

					//Draw horizontal line
					DrawLine(hdc, rc.left, rc.top + cell_size * i, rc.right, rc.top + cell_size * i);
				}
				RECT rcCell;

				for (int i = 0; i < 9; ++i) {
					if ((gameBoard[i] != 0) && GetCellRect(hWnd, i, &rcCell)) {
						FillRect(hdc, &rcCell, gameBoard[i] % 2 == 0 ? BLUE_BRUSH : RED_BRUSH);
					}
				}
			}

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		DeleteObject(BLUE_BRUSH);
		DeleteObject(RED_BRUSH);
		DestroyIcon(XIcon);
		DestroyIcon(OIcon);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
