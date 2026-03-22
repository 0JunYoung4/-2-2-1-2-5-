#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <random>
HINSTANCE g_hInst;
#include <string>
#include <ctime>


LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2-5";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

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
	static TCHAR str[5][20];
	static SIZE size;		// 문장 길이
	static int x, y, xPos, yPos, R, G, B;

	switch (uMsg) {
	case WM_CREATE:
		x = rand() % 550;
		y = rand() % 500;
		xPos = 0;
		yPos = 0;
		R = rand() % 256;
		G = rand() % 256;
		B = rand() % 256;
		CreateCaret(hWnd, NULL, 1, 15);
		ShowCaret(hWnd);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SetTextColor(hDC, RGB(R, G, B));
		for (int i = 0; i <= yPos; ++i) {
			COLORREF colors = RGB(R, G, B);
			TextOut(hDC, x, y + (i * 20), str[i], lstrlen(str[i]));
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == VK_BACK) {
			if (xPos > 0) {
				--xPos;
				str[yPos][xPos] = '\0';
			}
			else if (yPos > 0) {
				--yPos;
				xPos = lstrlen(str[yPos]);
			}
		}
		else if (wParam == VK_RETURN) {
			if (yPos < 5) {
				++yPos;
				xPos = 0;
			}
		}
		else {
			if (xPos < 19) {
				str[yPos][xPos++] = wParam;
				str[yPos][xPos] = '\0';
			}
		}
		str[yPos][xPos] = '\0';
		hDC = GetDC(hWnd);		// 커서를 찍어야 되니까
		GetTextExtentPoint32(hDC, str[yPos], lstrlen(str[yPos]), &size);
		SetCaretPos(size.cx + x, yPos * 20 + y);
		InvalidateRect(hWnd, NULL, TRUE);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}