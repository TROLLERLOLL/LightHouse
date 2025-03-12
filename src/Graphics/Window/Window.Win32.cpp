#include <Graphics/Window.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

const char CLASSNAME[] = "LightHouseEngineWindowClass";

void GameWindow::Error(const char* text, const char* filename, int line) {
	MessageBox(NULL, (std::string(text) + "\n(" + filename + ":" + std::to_string(line) + ")").c_str(), "Error", MB_ICONERROR); 
	exit(1);
}

KeyCodes getKey(unsigned int Keycode) {
	//Letters A - Z
	if (Keycode >= 0x41 && Keycode <= 0x5A)
		return (KeyCodes)((unsigned int)KeyCodes::A + (Keycode - 0x41));

	//Numbers 0 - 9
	if (Keycode >= 0x30 && Keycode <= 0x39)
		return (KeyCodes)((unsigned int)KeyCodes::NUM_0 + (Keycode - 0x30));

	//F keys
	if (Keycode >= VK_F1 && Keycode <= VK_F12)
		return (KeyCodes)((unsigned int)KeyCodes::F1 + (Keycode - VK_F1));

	switch (Keycode) {
		case VK_TAB: return KeyCodes::TAB;
		case VK_BACK: return KeyCodes::BACK;
		case VK_RETURN: return KeyCodes::ENTER;
		case VK_ESCAPE: return KeyCodes::ESCAPE;
		case VK_SPACE: return KeyCodes::SPACE;

		case VK_LCONTROL: return KeyCodes::LCTRL;
		case VK_RCONTROL: return KeyCodes::RCTRL;

		case VK_LMENU: return KeyCodes::LALT;
		case VK_RMENU: return KeyCodes::RALT;

		case VK_LSHIFT: return KeyCodes::LSHIFT;
		case VK_RSHIFT: return KeyCodes::RSHIFT;

		case VK_UP: return KeyCodes::UP;
		case VK_DOWN: return KeyCodes::DOWN;
		case VK_LEFT: return KeyCodes::LEFT;
		case VK_RIGHT: return KeyCodes::RIGHT;
	}

	return KeyCodes::NONE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CLOSE: {
			void* handle = GetProp(hwnd, "GameWindowClassHandle");
			if (handle != nullptr)
				((WindowData*)handle)->running = false;
			break;
		}

		case WM_GETMINMAXINFO: {
			void* handle = GetProp(hwnd, "GameWindowClassHandle");
			if (handle == nullptr)
				break;

			int MinWidth = ((WindowData*)handle)->MinimalWidth;
			int MinHeight = ((WindowData*)handle)->MinimalHeight;

			if (MinWidth == 0 && MinHeight == 0)
				break;

			RECT rect = { 0, 0, MinWidth, MinHeight};
			AdjustWindowRect(&rect, GetWindowLong(hwnd, GWL_STYLE), FALSE);

			if (MinWidth != 0)
				((MINMAXINFO*)lParam)->ptMinTrackSize.x = rect.right - rect.left;
	
			if (MinHeight != 0)
				((MINMAXINFO*)lParam)->ptMinTrackSize.y = rect.bottom - rect.top;
			break;
		}

		case WM_SYSCOMMAND:
			if (wParam == SC_KEYMENU && (lParam >> 16) <= 0) 
				return 0;
			break;

		case WM_KEYUP:
		case WM_KEYDOWN: {
			bool down = (msg == WM_KEYDOWN);
			KeyCodes keyCode = getKey(wParam);
			void* handle = GetProp(hwnd, "GameWindowClassHandle");
			if (handle == nullptr)
				break;
			((WindowData*)handle)->inputManager->setKey(keyCode, down);
			break;
		}

		case WM_SIZE:
			void* handle = GetProp(hwnd, "GameWindowClassHandle");
			if (handle == nullptr)
				break;
			((WindowData*)handle)->Minimized = wParam == SIZE_MINIMIZED;
			RECT rect;
			GetClientRect(hwnd, &rect);
			((WindowData*)handle)->Width = rect.right - rect.left;
			((WindowData*)handle)->Height = rect.bottom - rect.top;
			((WindowData*)handle)->justResized = true;
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//DO NOT TOUCH 
//It was a pain to get this working
HICON CreateIconFromTexture(Texture* texture, Rect source, GraphicsBackend backend) {
	HBITMAP color;
	HBITMAP mask;
	if (backend == GraphicsBackend::OpenGL)
		source.Location.Y = texture->Height - 1 - source.Location.Y - source.Size.Y;
	ICONINFO iconInfo = {};

	BITMAPINFO bmi = {};

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = source.Size.X;
	bmi.bmiHeader.biHeight = (backend == GraphicsBackend::OpenGL ? 1 : -1) * source.Size.Y;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	HDC hdc = GetDC(NULL);
	void* data = texture->data;
	//If texture has no data just handle it
	if (texture->data == nullptr)
		data = malloc(source.Size.X * source.Size.Y * 4);
	else {
		
		void* tempData = malloc(source.Size.X * source.Size.Y * 4);
		
		for (int y = 0; y < source.Size.Y; y++) {
			char* start = (char*)data + (source.Location.Y + y) * 4 * texture->Width + source.Location.X * 4;
			memcpy((char*)tempData + y * source.Size.X * 4, start, source.Size.X * 4);
		}
		data = tempData;
	}

	color = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, data, &bmi, DIB_RGB_COLORS);
	mask = CreateBitmap(source.Size.X, source.Size.Y, 1, 1, NULL);
	free(data);

	iconInfo.fIcon = TRUE;
	iconInfo.hbmColor = color;
	iconInfo.hbmMask = mask;

	HICON icon = CreateIconIndirect(&iconInfo);
	
	DeleteObject(color);
	DeleteObject(mask);

	return icon;
}

void GameWindow::setIcon(Texture* texture, Rect source) {
	HICON icon = CreateIconFromTexture(texture, source, backend);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
	DeleteObject(icon);
}

void GameWindow::createWindowInternal(const char* title, int Width, int Height) {
	hinstance = GetModuleHandle(NULL);
	//Registering of Window Class
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hinstance;
	wc.lpszClassName = CLASSNAME;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIconW(NULL, MAKEINTRESOURCEW(IDI_WINLOGO));
	if (RegisterClass(&wc) == 0)
		Error("Failed to Register Window Class", __FILE__, __LINE__);
		
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	hwnd = CreateWindowEx(0, wc.lpszClassName, title, WS_OVERLAPPEDWINDOW, ScreenWidth / 2 - Width / 2, ScreenHeight / 2 - Height / 2, Width, Height, nullptr, nullptr, hinstance, nullptr);
	if (hwnd == NULL)
		Error("Failed to create Window", __FILE__, __LINE__);

	RECT rect = { 0, 0, Width, Height };
	AdjustWindowRect(&rect, GetWindowLong(hwnd, GWL_STYLE), FALSE);
	int TempWidth = rect.right - rect.left;
	int TempHeight = rect.bottom - rect.top;
	SetWindowPos(hwnd, nullptr, ScreenWidth / 2 - TempWidth / 2, ScreenHeight / 2 - TempHeight / 2, TempWidth, TempHeight, 0);

	SetProp(hwnd, "GameWindowClassHandle", &data);
}

void GameWindow::setMinimalSizeInternal() {}

#ifdef GL
bool GameWindow::LoadOpenGLVersion(int major, int minor) {
	WNDCLASS fakeClass = WNDCLASS();
	fakeClass.style = CS_OWNDC;
	fakeClass.lpfnWndProc = DefWindowProc;
	fakeClass.hInstance = hinstance;
	fakeClass.lpszClassName = "FAKEWINDOWCLASSFORGAME";
	if (!RegisterClass(&fakeClass))
		Error("Couldn't Register Fake Window Class for OpenGL", __FILE__, __LINE__);

	HWND handle = CreateWindow(fakeClass.lpszClassName, "GAME GL HELPER IGNORE", 0, 0, 0, 10, 10, 0, 0, hinstance, 0);
	if (handle == 0)
		Error("Couldn't create Fake Window for OpenGL", __FILE__, __LINE__);

	HDC dc = GetDC(handle);
	if (dc == NULL) {
		DestroyWindow(handle);
		UnregisterClass(fakeClass.lpszClassName, hinstance);
		return false;
	}

	setPixelFormat(dc);
	HGLRC fakeGL = wglCreateContext(dc);
	if (fakeGL == NULL) {
		ReleaseDC(handle, dc);
		DestroyWindow(handle);
		UnregisterClass(fakeClass.lpszClassName, hinstance);
		return false;
	}

	wglMakeCurrent(dc, fakeGL);
	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fakeGL);

	//This should in theory check if we even support the correct OpenGL version (in this case 3.3)
	//This might be expanded to support OpenGL 2.0 and even OpenGL 1.0 if I seek a challenge
	//This isn't the best approach and might even disqualify working systems
	//But it shouldn't!!! Source: https://registry.khronos.org/OpenGL/specs/gl/glspec20.pdf Page: 254 (or Page 268 in PDF) 
	bool success = false;
	if (wglCreateContextAttribsARB) {
		int gl_attribs[] = { 0x2091, major, 0x2092, minor, 0x9126, 0x00000001, 0 };
		fakeGL = wglCreateContextAttribsARB(dc, 0, gl_attribs);
		if (fakeGL == NULL) {
			ReleaseDC(handle, dc);
			DestroyWindow(handle);
			UnregisterClass(fakeClass.lpszClassName, hinstance);
			return false;
		}

		wglMakeCurrent(dc, fakeGL);
		glGetString = (glGetString_type*)wglGetProcAddress("glGetString");
		if (glGetString == NULL) { //Must be under version 2.0 or something else must have gone wrong... No support
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(fakeGL);
			ReleaseDC(handle, dc);
			DestroyWindow(handle);
			UnregisterClass(fakeClass.lpszClassName, hinstance);
			return false;
		}
		
		const char* glVersion = (char*)glGetString(0x1F02); //GL_VERSION
		printf("%s\n", glVersion);
		int majorOut = 1;
		int minorOut = 0;
		
		//Spec: Major.Minor or Major.Minor.Patch
		if (sscanf(glVersion, "%d.%d", &majorOut, &minorOut) == 2)
			if (majorOut > major || (majorOut == major && minorOut >= minor))
				success = true;

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(fakeGL);
	}

	ReleaseDC(handle, dc);
	DestroyWindow(handle);
	UnregisterClass(fakeClass.lpszClassName, hinstance);
	return success;
}

void GameWindow::setPixelFormat(HDC DC) {
	PIXELFORMATDESCRIPTOR format = PIXELFORMATDESCRIPTOR();
	format.nSize = sizeof(format);
	format.iPixelType = PFD_TYPE_RGBA;
	format.iLayerType = PFD_MAIN_PLANE;
	format.nVersion = 1;
	format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	format.cColorBits = 32;
	//format.cDepthBits = 32;
	int Pformat = ChoosePixelFormat(DC, &format);

	SetPixelFormat(DC, Pformat, &format);
}

bool GameWindow::loadGL() {
	int major = 3;
	int minor = 3;

	bool success = LoadOpenGLVersion(major, minor);
	if (!success)
		return false;

	dc = GetDC(hwnd);
	setPixelFormat(dc);

	int gl_attribs[] = { 0x2091, major, 0x2092, minor, 0x9126, 0x00000001, 0 };
	context = wglCreateContextAttribsARB != NULL ? wglCreateContextAttribsARB(dc, 0, gl_attribs) : wglCreateContext(dc);;
	if (context == NULL)
		Error("Failed to create generic OpenGL Context", __FILE__, __LINE__);

	if (!wglMakeCurrent(dc, context))
		Error("Failed to make OpenGL Context Current", __FILE__, __LINE__);

	wglSwapIntervalEXT = (wglSwapIntervalEXT_type*)wglGetProcAddress("wglSwapIntervalEXT");
	glProc = wglGetProcAddress;
	return true;
}
#endif

void GameWindow::setTitle(const char* title) {
	data.title = title;
	SetWindowText(hwnd, title);
}

void GameWindow::setVsync(bool value) {
#ifdef GL
	if (backend == GraphicsBackend::OpenGL)
		wglSwapIntervalEXT(value ? 1 : 0);
#endif

	vsync = value;
}

void GameWindow::showCursor(bool value) {
	ShowCursor(value);
}

void GameWindow::showWindow(bool value) {
	if (firstFrame) {
		firstFrame = value;
		return;
	}

	ShowWindow(hwnd, value ? 1 : 0);
}

void GameWindow::HandleEventsInternal() {
	MSG msg;
	while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void GameWindow::pollEventsInternal() {
	//The first frame is actually shown to the Player
	if (firstFrame) {
		firstFrame = false;
		ShowWindow(hwnd, 1);
	}
	else {
#ifdef GL
		if (backend == GraphicsBackend::OpenGL)
			SwapBuffers(dc);
		else
#endif
			graphics->Present();
	}

	HandleEventsInternal();
}

void GameWindow::closeInternal() {
#ifdef GL
	if (backend == GraphicsBackend::OpenGL) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(context);
		ReleaseDC(hwnd, dc);
	}
#endif
	UnregisterClass(CLASSNAME, hinstance);
	DestroyWindow(hwnd);
	RemoveProp(hwnd, "GameWindowClassHandle");
}