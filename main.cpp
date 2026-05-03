#include "Main.h"
#include "Food.h"
#include "Snake.h"

static Snake snake;
static Food snakeFood;
static int score = 0;

DWORD lastMoveTime = 0;
const DWORD moveDelay = 180; // ms

LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_CREATE:
		SetTimer(hwnd, 1, 180, NULL);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		RECT rect;
		GetClientRect(hwnd, &rect);

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		//1. create back buffer
		HDC memDc = CreateCompatibleDC(hdc);
		HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(memDc, memBitmap);

		//2. GDI+ draw on back buffer
		Gdiplus::Graphics graphics(memDc);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
		//graphics.SetSmoothingMode(Gdiplus::SmoothingModeNone);
		//graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);

		Gdiplus::FontFamily fontFamily(L"Segoe UI");
		Gdiplus::Font font(&fontFamily, 18, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

		Gdiplus::SolidBrush lightBrush(Gdiplus::Color(255, 204, 214, 127));
		Gdiplus::SolidBrush darkBrush(Gdiplus::Color(255, 190, 200, 110));
		Gdiplus::SolidBrush scoreBrush(Gdiplus::Color(255, 255, 248, 236));


		// make mesh like bg
		for (int i = 0;i < CELL_COUNT;i++) {
			for (int j = 0;j < CELL_COUNT;j++) {
				graphics.FillRectangle(
					(i + j) % 2 == 0 ? &lightBrush : &darkBrush,
					j * CELL_SIZE,
					i * CELL_SIZE,
					CELL_SIZE,
					CELL_SIZE
				);
			}
		}

		std::wstring scoreText = L"Score: " + std::to_wstring(score);



		snakeFood.draw(graphics);
		snake.draw(graphics);

		graphics.DrawString(scoreText.c_str(), -1, &font, Gdiplus::PointF(width - 100, 5), &scoreBrush);

		//3. copy to screen
		BitBlt(hdc, 0, 0, width, height, memDc, 0, 0, SRCCOPY);

		//4. cleanup
		SelectObject(memDc, oldBitmap);
		DeleteObject(memBitmap);
		DeleteDC(memDc);

		EndPaint(hwnd, &ps);

		return 0;
	}
	case WM_TIMER: {

		DWORD now = GetTickCount64();

		if (now - lastMoveTime >= moveDelay) {
			snake.move();
			lastMoveTime = now;
		}
		if (snake.isEatingFood(snakeFood)) {
			score++;
			snakeFood.respon();
		}

		InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	}
	case WM_KEYDOWN: {
		if (wParam == VK_RIGHT) {
			if (snake.dir != Snake::LEFT) {
				snake.dir = Snake::RIGHT;
			}
		}
		if (wParam == VK_LEFT) {
			if (snake.dir != Snake::RIGHT) {
				snake.dir = Snake::LEFT;
			}
		}
		if (wParam == VK_UP) {
			if (snake.dir != Snake::DOWN) {
				snake.dir = Snake::UP;
			}
		}
		if (wParam == VK_DOWN) {
			if (snake.dir != Snake::UP) {
				snake.dir = Snake::DOWN;
			}
		}

		return 0;
	}
	case WM_ERASEBKGND:
		return 1;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Gdiplus::GdiplusStartupInput startupToken;
	ULONG_PTR gdiPlusToken;
	Gdiplus::GdiplusStartup(&gdiPlusToken, &startupToken, NULL);

	const wchar_t CLASS_NAME[] = L"Main_Window";
	const wchar_t mainWIndowName[] = L"Snake game";

	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = mainWindowProc;
	windowClass.lpszClassName = CLASS_NAME;
	windowClass.hInstance = hInstance;
	RECT rc = { 0, 0, CELL_SIZE * CELL_COUNT, CELL_SIZE * CELL_COUNT };
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

	RegisterClass(&windowClass);

	HWND mainWindow = CreateWindow(
		CLASS_NAME,
		mainWIndowName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(mainWindow, nCmdShow);

	MSG msg = {};

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiPlusToken);
	return 0;
}