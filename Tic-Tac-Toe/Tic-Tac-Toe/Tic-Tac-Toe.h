#pragma once

#include "resource.h"
#include <windowsx.h>

BOOL GetGameBoardRect(HWND hwnd, RECT *pRect);
void DrawLine(HDC hdc, int x1, int y1, int x2, int y2);
BOOL GetCellNumberFromPoint(HWND hwnd, int x, int y);
int GetWinner(int win[3]);
void ShowTurn(HWND hwnd, HDC hdc);
void DrawIconCentered(HDC hdc, RECT* rc, HICON hicon);