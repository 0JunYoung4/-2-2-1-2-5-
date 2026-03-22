#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <random>
HINSTANCE g_hInst;
#include <string>
#include <ctime>


LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2-6";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

struct DrawData {
	int x, y, n, count;
	COLORREF textColor;		//글자 색
	COLORREF bgColor;		//배경 색
};
DrawData arr[10];
int datacount = 0;
int currentIndex = -1;		//현재 출력할 것
bool showAll = false;		// 전체 출력 모드

TCHAR input[100] = TEXT("");	//입력 문자열
int inputlen = 0;

bool isOverlap(int x, int y, int count) {
	int left = x;
	int top = y;
	int right = x + count * 20;
	int bottom = y + count * 20;

	for (int i = 0; i < datacount; i++) {
		int left2 = arr[i].x;
		int top2 = arr[i].y;
		int right2 = arr[i].x + arr[i].count * 20;
		int bottom2 = arr[i].y + arr[i].count * 20;

		// 겹치지 않는 경우
		if (right <= left2 || left >= right2 ||
			bottom <= top2 || top >= bottom2) {
			continue;
		}

		return true; // 겹침 발생
	}

	return false; // 안 겹침
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	srand((unsigned)time(NULL));

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
	{
		hDC = BeginPaint(hWnd, &ps);
		TCHAR buf[10];

		for (int i = 0; i < datacount; ++i) {
			if (!showAll && i != currentIndex)
				continue;		//하나만 출력
			for (int row = 0; row < arr[i].count; ++row) {
				for (int col = 0; col < arr[i].count; ++col) {
					HBRUSH hBrush = CreateSolidBrush(arr[i].bgColor);
					HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					Rectangle(hDC, arr[i].x + col * 20, arr[i].y + row * 20, arr[i].x + col * 20 + 20, arr[i].y + row * 20 + 20);

					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					wsprintf(buf, TEXT("%d"), arr[i].n);
					TextOut(hDC, arr[i].x + col * 20+5, arr[i].y + row * 20+2, buf, lstrlen(buf));
					SetBkMode(hDC, TRANSPARENT);
				}
			}
		}
		SetTextColor(hDC, RGB(0, 0, 0));
		SetBkColor(hDC, RGB(255, 255, 255));
		SetBkMode(hDC, OPAQUE);
		TextOut(hDC, 10, 500, input, lstrlen(input));
		GetTextExtentPoint32(hDC, input, lstrlen(input), &size);
		SetCaretPos(10 + size.cx, 500);
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_KEYDOWN:
	{
		if (wParam == 'r'||wParam=='R') {
			datacount = 0;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'a'||wParam=='A') {
			showAll = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (wParam == 'q' || wParam == 'Q') {
			PostQuitMessage(0);
		}
	}
	break;
	case WM_CHAR:
	{
		if (wParam == VK_BACK) {
			if (inputlen > 0) {
				--inputlen;
				input[inputlen] = '\0';
			}
		}
		else if (wParam == VK_RETURN) {
			int x, y, n, count;
		
			if (_stscanf_s(input, TEXT("%d %d %d %d"), &x, &y, &n, &count) == 4) {
				if (!isOverlap(x, y, count)) {
					if (datacount < 10) {
						arr[datacount].x = x;
						arr[datacount].y = y;
						arr[datacount].n = n;
						arr[datacount].count = count;

						arr[datacount].textColor = RGB(rand() % 256, rand() % 256, rand() % 256);
						arr[datacount].bgColor = RGB(rand() % 256, rand() % 256, rand() % 256);

						currentIndex = datacount;
						++datacount;
						showAll = false;
					}
				}
				else {
					MessageBox(hWnd, TEXT("겹치는 위치입니다!"), TEXT("오류"), MB_OK);
				}
			}
			inputlen = 0;
			input[0] = '\0';
		}
		else {
			if (inputlen < 99) {
				input[inputlen++] = (TCHAR)wParam;
				input[inputlen] = '\0';
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}