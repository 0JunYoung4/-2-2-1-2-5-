#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <ctime>

std::vector<POINT> points;
int colCount;	// 랜덤한 단

COLORREF Colors; // 줄 색

COLORREF RandomColor() {
	return RGB(rand() % 256, rand() % 256, rand() % 256);
}

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"My Window Class";
LPCTSTR lpszWindowName = L"2-4-1";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessege, WPARAM wParam, LPARAM IParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
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
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 윈도우 색
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam) {

	//메시지 처리하기
	switch (uMsg) {
	case WM_CREATE:
	{
		srand((unsigned)time(NULL));
	

	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);

		RECT rt;
		GetClientRect(hWnd, &rt);
		colCount = rand() % 15 + 2;		// 몇단으로?
		int num = rand() % colCount;

		int width = rt.right;		// 윈도우 가로
		int height = rt.bottom;		// 윈도우 세로

		int CellWidth = width / colCount;
		TCHAR buf[100];

		for (int i = 1; i <= 9; ++i) {
			SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
			int dan = colCount;
			wsprintf(buf, TEXT("%d * %d = %d"), dan, i, dan * i);
			int x= CellWidth * num + i * 10;
			int y = i * 20;
			TextOut(hDC, x, y+20, buf, lstrlen(buf));
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