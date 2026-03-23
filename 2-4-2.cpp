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
LPCTSTR lpszWindowName = L"2-4-2";

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
	static int RectWidth;		// 사각형의 가로 폭
	static int RectHeight;		// 사각형의 세로 폭
	int x, y;	// 사각형 위치
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
		HDC hDC = BeginPaint(hWnd,&ps);

		//폰트 생성(고정폭)
		HFONT hFont = CreateFont(
			20, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			L"Consolas"
		);

		HFONT oldFont = (HFONT)SelectObject(hDC, hFont);

		RECT rt;
		GetClientRect(hWnd, &rt);
		
		int width = rt.right;		// 윈도우 가로
		int height = rt.bottom;		// 윈도우 세로

		int HarfW = width / 2;
		int HarfH = height / 2;

		RectWidth = HarfW - 50;
		RectHeight = HarfH - 50;

		x = rand() % (width - RectWidth);
		y = rand() % (height - RectHeight-10);

		std::wstring text;
		/*int len = rand() % 10 + 5;

		for (int i = 0; i < len; ++i) {
			text += L'A' + rand() % 26;
		}*/

		SIZE size;
		int maxW = 0, maxH = 0;

		for (wchar_t ch = L'A'; ch <= L'Z'; ch++) {
			GetTextExtentPoint32(hDC, &ch, 1, &size);
			if (size.cx > maxW) maxW = size.cx;
			if (size.cy > maxH) maxH = size.cy;
		}

		int cols = RectWidth / maxW;
		int rows = RectHeight / maxH;


		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		for (int i = 0; i <cols; ++i) {	// 위쪽			(사각형의 가로 길이를 픽셀 크기로 나누면 몇개의 문자를 넣을지 나옴)
			
			wchar_t ch = L'A' + rand() % 26;
			TextOut(hDC, x + i * maxW, y, &ch, 1);
		}
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		for (int i = 0; i < rows; i++) {	//오른쪽
			
			wchar_t ch = L'A' + rand() % 26;
			TextOut(hDC, x + RectWidth, y + i * maxH, &ch, 1);
		}
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		for (int i = 1; i < cols; i++) {	//아래쪽
			
			wchar_t ch = L'A' + rand() % 26;
			TextOut(hDC, x + i * maxW, y + RectHeight-maxH, &ch, 1);
		}
		SetTextColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
		for (int i = 0; i < rows; i++) {	//왼쪽
			
			wchar_t ch = L'A' + rand() % 26;
			TextOut(hDC, x, y + i * maxH, &ch, 1);
		}

		SelectObject(hDC, oldFont);
		DeleteObject(hFont);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}