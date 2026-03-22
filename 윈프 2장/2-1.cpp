#include <windows.h> //--- 윈도우 헤더 파일
#include <tchar.h>
HINSTANCE g_hInst;

#include <vector>
#include <string>
#include <ctime>

std::vector<POINT> points;  // 좌표 저장

LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2-1";
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

	switch (uMsg) {
	case WM_CREATE:
	{
		srand((unsigned)time(NULL));

		int width = 800;		// 윈도우 가로길이
		int height = 600;		// 윈도우 세로길이

		while (points.size() < 15) {	//숫자좌표 15개 만들때 까지
			int x = rand() % (width - 100);		// 100은 여유공간
			int y = rand() % (height - 30);

			bool overlap = false;		// 숫자가 겹치는지 확인

			for (auto& p : points) {
				if (abs(p.x - x) < 80 && abs(p.y - y) < 30) {		// 거리가 너무 가까우면
					overlap = true;
					break;
				}
			}
			if (!overlap) {
				points.push_back({ x,y });
			}
		}
	}
	break;
	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &ps);
		TCHAR text[100];
		for (int i = 0; i < 15; ++i) {
			wsprintf(text, TEXT("%d: (%d,%d)"), i + 1, points[i].x, points[i].y);	// 출력할 내용을 TCHAR타입 배열에 저장
			TextOut(hDC, points[i].x, points[i].y, text, lstrlen(text));		// 어느 위치에 무엇을 어느정도 길이로 출력
		}
		int centerX = 400;
		int centerY = 300;

		TCHAR centerText[100];

		wsprintf(centerText, TEXT("0: (%d, %d)"), 400, 300);

		TextOut(hDC, centerX, centerY, centerText, lstrlen(centerText));
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}