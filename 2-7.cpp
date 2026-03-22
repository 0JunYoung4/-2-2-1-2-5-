#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <random>
HINSTANCE g_hInst;
#include <string>
#include <ctime>


LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2-7";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

TCHAR text[10][31];
int lineLen[10] = {};		//각줄 길이
int curLine = 0;		// 현재 줄(0~9)
int curCol = 0;		//현재 위치(0~30)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam) {
	PAINTSTRUCT ps;
	HDC hDC;
	static SIZE size;


	switch (uMsg) {
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 1, 15);
		ShowCaret(hWnd);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		for (int i = 0; i < 10; i++) {
			TextOut(hDC, 10, 20 + i * 20, text[i], lineLen[i]);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_ESCAPE) {
			for (int i = 0; i < 10; i++) {
				lineLen[i] = 0;
				text[i][0] = '\0';
			}
			curLine = 0;
			curCol = 0;

			InvalidateRect(hWnd, NULL, TRUE);
			SetCaretPos(10, 20);
		}
		else if (wParam == VK_RETURN) {
			curLine = (curLine + 1) % 10;
			curCol = 0;

			lineLen[curLine] = 0;
			text[curLine][0] = '\0';
		}
		else if (wParam == VK_BACK) {
			if (curCol > 0) {
				for (int i = curCol - 1; i < lineLen[curLine] - 1; i++) {
					text[curLine][i] = text[curLine][i + 1];
				}

				lineLen[curLine]--;
				curCol--;

				text[curLine][lineLen[curLine]] = '\0';
			}
		}
		else {
			if (curCol < lineLen[curLine]) {
				text[curLine][curCol] = (TCHAR)wParam;
			}
			else if (lineLen[curLine] < 30) {
				text[curLine][curCol] = (TCHAR)wParam;
				lineLen[curLine]++;
			}

			curCol++;
			if (curCol >= 30) {
				curLine = (curLine + 1) % 10;
				curCol = 0;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		hDC = GetDC(hWnd);
		GetTextExtentPoint32(hDC, text[curLine], curCol, &size);
		SetCaretPos(10 + size.cx, 20 + curLine*20);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_DESTROY:
		
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}