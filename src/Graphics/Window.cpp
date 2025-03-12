#include <Graphics/Window.h>
//Backends
#ifdef DX11
#include <Graphics/DX11/Graphics.DX11.h>
#endif
#ifdef GL
#include <Graphics/OpenGL/Graphics.GL.h>
#endif


void GameWindow::loadGraphics(GraphicsBackend backend, bool setting) {
	if (graphics != nullptr)
		return;

	//Loading the Best Graphics API; Only fully implemented ones
	if (backend == GraphicsBackend::Best) {
		this->backend = backend;

#ifdef DX11
		loadGraphics(GraphicsBackend::DirectX11);
		if (graphics != nullptr)
			return;
#endif
#ifdef GL
		loadGraphics(GraphicsBackend::OpenGL);
		if (graphics != nullptr)
			return;
#endif
#ifdef DX11
		loadGraphics(GraphicsBackend::DirectX11, true); //Software emulation of DX11
		if (graphics != nullptr)
			return;
#endif
		Error("Couldn't initialize Graphics API!", __FILE__, __LINE__);
	}
	else {

#ifdef DX11
		if (backend == GraphicsBackend::DirectX11)
			if (DirectX11::isSupported(setting))
				graphics = (Graphics*)new DirectX11(hwnd, this, setting);
#endif
#ifdef GL
		if (backend == GraphicsBackend::OpenGL)
			if (loadGL()) //Sets the Window up to support OpenGL at the same time checks for Support
				graphics = (Graphics*)new OpenGL(this, glProc);
#endif
	}

	if (graphics == nullptr && this->backend != GraphicsBackend::Best)
		Error((std::string("Couldn't initialize ") + (backend == GraphicsBackend::DirectX11 ? "DirectX11" : backend == GraphicsBackend::OpenGL ? "OpenGL" : "NULL") + " Renderer").c_str(), __FILE__, __LINE__);

	if (graphics != nullptr) {
		this->backend = backend;
		this->graphics = graphics;
	}
}

GameWindow::GameWindow(const char* title, int Width, int Height, GraphicsBackend backend) {
	createWindowInternal(title, Width, Height);
	data.Width = Width;
	data.Height = Height;
	lastTime = std::chrono::high_resolution_clock::now();

	inputManager = new InputManager();
	data.inputManager = inputManager;
	loadGraphics(backend);
	setVsync(true);

	backend = this->backend;
	std::string Extension = " | " + std::string(std::string(backend == GraphicsBackend::Best ? "Best" : backend == GraphicsBackend::DirectX11 ? "DX11" : backend == GraphicsBackend::OpenGL ? "GL" : "NONE"));

	setTitle((title + Extension).c_str());
}

void GameWindow::setMinimalSize(int Width, int Height) {
	data.MinimalWidth = Width;
	data.MinimalHeight = Height;
	setMinimalSizeInternal();
}

bool GameWindow::pollEvents() {
	data.justResized = false;
	inputManager->Update();

	if (!firstFrame)
		graphics->EndFrame();

	pollEventsInternal();
	if (data.justResized)
		graphics->OnResize(data.Width, data.Height);

	graphics->BeginFrame();

	current = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> delta = current - lastTime;
	lastTime = current;
	deltaTime = delta.count();

	if (FPSTimer >= 1.0f) {
		FPS = (int)((float)FPSFrameCounter / FPSTimer);
		FPSTimer = 0;
		FPSFrameCounter = 0;
	}
	FPSTimer += deltaTime;
	FPSFrameCounter++;

	return data.running;
}

void GameWindow::close() {
	if (!data.running)
		return;
	data.running = false;
	closeInternal();
}