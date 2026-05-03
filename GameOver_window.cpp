#include "Main.h"
#include "helper.h"

#define IDC_RESTART_BUTTON 101
#define IDC_QUIT_BUTTON 102

struct GameOverWindowData {
	int score;
	bool* restartRequest;
};

static void DrawGameOverBackground(Gdiplus::Graphics& graphics, int width, int height) {
	Gdiplus::SolidBrush lightBrush(Gdiplus::Color(255, 210, 196, 149));
	Gdiplus::SolidBrush darkBrush(Gdiplus::Color(255, 176, 151, 95));

	for (int y = 0; y < height; y += CELL_SIZE) {
		for (int x = 0; x < width; x += CELL_SIZE) {
			Gdiplus::Rect tileRect(x, y, CELL_SIZE, CELL_SIZE);
			graphics.FillRectangle(
				((x / CELL_SIZE) + (y / CELL_SIZE)) % 2 == 0 ? &lightBrush : &darkBrush,
				tileRect
			);
		}
	}
}

static void DrawRoundedButton(Gdiplus::Graphics& graphics, const RECT& rect, bool pressed, const wchar_t* text) {
	Gdiplus::GraphicsPath path;
	CreateRundedRect(path, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 10);

	Gdiplus::SolidBrush baseBrush(Gdiplus::Color(255, 90, 67, 41));
	Gdiplus::SolidBrush fillBrush(pressed ? Gdiplus::Color(255, 106, 74, 46) : Gdiplus::Color(255, 143, 102, 64));
	Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 248, 236));
	Gdiplus::Pen borderPen(pressed ? Gdiplus::Color(255, 74, 51, 30) : Gdiplus::Color(255, 92, 66, 41), 2);

	Gdiplus::Rect buttonRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	graphics.FillRectangle(&baseBrush, buttonRect);
	graphics.FillPath(&fillBrush, &path);
	graphics.DrawPath(&borderPen, &path);

	Gdiplus::FontFamily fontFamily(L"Segoe UI");
	Gdiplus::Font font(&fontFamily, 10, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentCenter);
	format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
	graphics.DrawString(text, -1, &font, Gdiplus::RectF((Gdiplus::REAL)rect.left, (Gdiplus::REAL)rect.top, (Gdiplus::REAL)(rect.right - rect.left), (Gdiplus::REAL)(rect.bottom - rect.top)), &format, &textBrush);
}

LRESULT CALLBACK gameOverWindoProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE: {

		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		GameOverWindowData* data = (GameOverWindowData*)cs->lpCreateParams;

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)data);

		CreateWindowW(L"BUTTON", L"Restart", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW | BS_DEFPUSHBUTTON,
			62, 130, 80, 25, hwnd, (HMENU)IDC_RESTART_BUTTON, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

		CreateWindowW(L"BUTTON", L"Quit", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
			158, 130, 80, 25, hwnd, (HMENU)IDC_QUIT_BUTTON, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		RECT rect;
		GetClientRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		Gdiplus::Graphics graphics(hdc);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
		DrawGameOverBackground(graphics, width, height);

		Gdiplus::GraphicsPath panelPath;
		CreateRundedRect(panelPath, 22, 18, width - 44, height - 36, 18);
		Gdiplus::SolidBrush panelBrush(Gdiplus::Color(235, 90, 67, 41));
		Gdiplus::Pen panelPen(Gdiplus::Color(255, 71, 52, 30), 2);
		graphics.FillPath(&panelBrush, &panelPath);
		graphics.DrawPath(&panelPen, &panelPath);
		Gdiplus::FontFamily fontFamily(L"Segoe UI");
		Gdiplus::Font titleFont(&fontFamily, 20, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
		Gdiplus::Font scoreFont(&fontFamily, 13, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		Gdiplus::SolidBrush titleBrush(Gdiplus::Color(255, 255, 248, 236));
		Gdiplus::SolidBrush scoreBrush(Gdiplus::Color(255, 220, 210, 178));

		GameOverWindowData* data = (GameOverWindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		int score = data ? data->score : 0;

		std::wstring titleText = L"Game Over";
		std::wstring scoreText = L"Score: " + std::to_wstring(score);
		std::wstring hintText = L"Try again when you're ready.";

		Gdiplus::RectF titleRect(0, 30, (Gdiplus::REAL)width, 30);
		Gdiplus::RectF scoreRect(0, 68, (Gdiplus::REAL)width, 24);
		Gdiplus::RectF hintRect(0, 92, (Gdiplus::REAL)width, 20);
		Gdiplus::StringFormat stringFormat;
		stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
		stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
		graphics.DrawString(titleText.c_str(), -1, &titleFont, titleRect, &stringFormat, &titleBrush);
		graphics.DrawString(scoreText.c_str(), -1, &scoreFont, scoreRect, &stringFormat, &scoreBrush);
		graphics.DrawString(hintText.c_str(), -1, &scoreFont, hintRect, &stringFormat, &scoreBrush);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DRAWITEM: {
		DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;
		if (!dis || dis->CtlType != ODT_BUTTON) {
			return FALSE;
		}

		const wchar_t* text = (dis->CtlID == IDC_RESTART_BUTTON) ? L"Restart" : L"Quit";
		bool pressed = (dis->itemState & ODS_SELECTED) != 0;

		Gdiplus::Graphics graphics(dis->hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
		DrawRoundedButton(graphics, dis->rcItem, pressed, text);
		return TRUE;
	}
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case IDC_RESTART_BUTTON: {
			GameOverWindowData* data = (GameOverWindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (data && data->restartRequest) {
				*(data->restartRequest) = true;
			}
			DestroyWindow(hwnd);
			break;
		}
		case IDC_QUIT_BUTTON: {
			GameOverWindowData* data = (GameOverWindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (data && data->restartRequest) {
				*(data->restartRequest) = false;
			}
			// Signal application quit and close this window
			PostQuitMessage(0);
			DestroyWindow(hwnd);
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}
	case WM_ERASEBKGND:
		return 1;
	case WM_DESTROY:
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

bool showGameOverWindow(HINSTANCE hInstance,HWND parentWindow, int score) {
	const wchar_t windowClassId[] = L"Gameover_window";

	WNDCLASS gameOverWindowClass = {};
	gameOverWindowClass.lpfnWndProc = gameOverWindoProc;
	gameOverWindowClass.lpszClassName = windowClassId;
	gameOverWindowClass.hInstance = hInstance;
	gameOverWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);


	RegisterClass(&gameOverWindowClass);

	bool restartRequested = false;
	GameOverWindowData data = { score, &restartRequested };

	// Calculate center position on parent window
	RECT parentRect;
	GetWindowRect(parentWindow, &parentRect);
	int parentWidth = parentRect.right - parentRect.left;
	int parentHeight = parentRect.bottom - parentRect.top;
	int windowWidth = 300;
	int windowHeight = 200;
	int centerX = parentRect.left + (parentWidth - windowWidth) / 2;
	int centerY = parentRect.top + (parentHeight - windowHeight) / 2;

	HWND gameOverWindow = CreateWindowEx(
		0,
		windowClassId,
		L"Game Over",
		WS_POPUP | WS_VISIBLE,
		centerX, centerY, windowWidth, windowHeight,
		parentWindow,
		NULL,
		hInstance,
		&data
	);

	EnableWindow(parentWindow, FALSE);
	ShowWindow(gameOverWindow, SW_SHOW);

	UpdateWindow(gameOverWindow);
	MSG msg;
	// Run a modal-like loop until the game over window is closed.
	while (IsWindow(gameOverWindow)) {
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	EnableWindow(parentWindow, TRUE);
	ShowWindow(parentWindow, SW_SHOW);
	SetFocus(parentWindow);
	return restartRequested;
}

