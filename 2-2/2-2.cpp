#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
#include <string>
#include <ctime>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2-2";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int colCount; // 가로 분할 개수
int num;


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
	
	switch (uMsg) {
	case WM_CREATE:
	{
		srand((unsigned)time(NULL));
		colCount = rand() % 7 + 2; // 2~8
		num = rand() & 8+1;
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);

		int width = 800;
		int height = 600;

		int cellW = width / colCount;	// 가로를 나눔
		int cellH = height / 2;

		TCHAR buf[100];

		wsprintf(buf, TEXT("가로 등분: %d"), colCount);
		TextOut(hDC, 10, 510, buf, lstrlen(buf));
		wsprintf(buf, TEXT("랜덤 숫자: %d"), num);
		TextOut(hDC, 150, 510, buf, lstrlen(buf));

		if ((num / 2) != 0) {
			for (int i = 0; i < colCount; ++i) {
			int dan = i + 2;
			
				COLORREF colors = RGB((dan * 30) % 256, (dan * 60) % 256, (dan * 90) % 256);
				SetTextColor(hDC, colors);

				for (int j = 1; j <= 9; ++j) {
					wsprintf(buf, TEXT("%d * %d = %d"), dan, j, dan * j);

					int x = i * cellW + 10;
					int y = j * 20;

					TextOut(hDC, x, y, buf, lstrlen(buf));
				}

				for (int i = 0; i < colCount; ++i) {
					int dan = colCount + 1 - i;
					COLORREF colors = RGB((dan * 30) % 256, (dan * 60) % 256, (dan * 90) % 256);
					SetTextColor(hDC, colors);
					for (int j = 1; j <= 9; ++j) {
						wsprintf(buf, TEXT("%d * %d = %d"), dan, j, dan * j);
						int x = i * cellW + 10;
						int y = cellH + j * 20;

						TextOut(hDC, x, y, buf, lstrlen(buf));
					}
				}
			}
		}
		else {
			for (int i = 0; i < colCount; ++i) {
				int dan = i + 2;
				for (int j = 1; j <= 9; ++j) {
				
					wsprintf(buf, TEXT("%d * %d = %d"), dan, j, dan * j);
					COLORREF colors = RGB((j * 30) % 256, (j * 60) % 256, (j * 90) % 256);
					SetTextColor(hDC, colors);
					int x = i * cellW + 10;
					int y = j * 20;

					TextOut(hDC, x, y, buf, lstrlen(buf));
				}

				for (int i = 0; i < colCount; ++i) {
					int dan = colCount + 1 - i;
					for (int j = 1; j <= 9; ++j) {
						
						wsprintf(buf, TEXT("%d * %d = %d"), dan, j, dan * j);
						COLORREF colors = RGB((j * 30) % 256, (j * 60) % 256, (j * 90) % 256);
						SetTextColor(hDC, colors);
						int x = i * cellW + 10;
						int y = cellH + j * 20;

						TextOut(hDC, x, y, buf, lstrlen(buf));
					}
				}
			}
		}
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}