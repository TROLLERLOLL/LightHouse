#pragma once
#include <string>
#include <Graphics/Graphics.h>
#include <chrono>
#include <Utils/InputManager.h>

#ifdef WIN32 //Windows is weird
#define __FILENAME__(File) std::string(File).substr(std::string(File).find("\\src\\") + 5)
#else
#define __FILENAME__(File) std::string(File).substr(std::string(File).find("/src/") + 5)
#endif

#ifdef WIN32
struct HWND__;
struct HDC__;
struct HINSTANCE__;
struct HGLRC__;
typedef struct HWND__* HWND;
typedef struct HDC__* HDC;
typedef struct HINSTANCE__* HINSTANCE;
typedef struct HGLRC__* HGLRC;
#endif

#ifdef X11
struct _XDisplay;
typedef _XDisplay Display;

typedef unsigned int X11Window;
	#ifdef GL
		typedef struct __GLXcontextRec* GLXContext;
	#endif
#endif

struct WindowData {
	bool running = true;
	int Width;
	int Height;
	const char* title;
	bool justResized = false;

	int MinimalWidth = 0;
	int MinimalHeight = 0;
	bool Minimized = false;
	InputManager* inputManager;
};

class GameWindow {
	private:
		void loadGraphics(GraphicsBackend backend, bool setting = false);
		std::chrono::high_resolution_clock::time_point lastTime;
		std::chrono::high_resolution_clock::time_point current;

#ifdef WIN32
#define WINAPI __stdcall
		HWND hwnd;
		HDC dc;
		HINSTANCE hinstance;

	#ifdef GL
		typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC dc, HGLRC hShareContext, const int* attribList);
		wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;
		typedef signed char* WINAPI glGetString_type(unsigned int); //minimal version is 2.0
		glGetString_type* glGetString;
		typedef HGLRC WINAPI wglSwapIntervalEXT_type(int interval);
		wglSwapIntervalEXT_type* wglSwapIntervalEXT;
		HGLRC context;
		void setPixelFormat(HDC DC);
	#endif
#undef WINAPI
#endif

#ifdef GL
	#ifdef WIN32
			bool LoadOpenGLVersion(int major, int minor);
	#else
			bool LoadOpenGLVersion();
	#endif
		void* glProc;
		bool loadGL();
		void unloadGL();
#endif

#ifdef X11
		Display* display;
		int screen;
		X11Window window;

		KeyCodes getKeyCode(unsigned int key);
	#ifdef GL
		GLXContext context = nullptr;
		typedef int (*glXSwapIntervalSGI_type)(unsigned int interval);
		glXSwapIntervalSGI_type glXSwapIntervalSGI = nullptr;
		typedef int (*glXSwapIntervalEXT_type)(Display* display, X11Window drawable, int interval);
		glXSwapIntervalEXT_type glXSwapIntervalEXT = nullptr;
		void* vii;
		void* fbcc;
	#endif
#endif

		WindowData data;
		bool firstFrame = true;
		GraphicsBackend backend;
		Graphics* graphics = nullptr;
		float deltaTime = 0.16f;

		float FPSTimer = 0.0f;
		int FPSFrameCounter = 0;
		int FPS = 0;

		bool vsync = true;

		void pollEventsInternal();
		void HandleEventsInternal();
		void closeInternal();
		void createWindowInternal(const char* title, int Width, int Height);
		void setMinimalSizeInternal();
	public:
		InputManager* inputManager;
		GameWindow(const char* title, int Width, int Height, GraphicsBackend backend = GraphicsBackend::Best);
		bool pollEvents();
		void close();
		
		void setTitle(const char* title);
		void setVsync(bool value);
		inline bool getVsync() { return vsync; }
		void showCursor(bool value);
		void showWindow(bool value);
		void Error(const char* message, const char* file, int line);

		inline int getFPS() { return FPS; }
		void GameWindow::setIcon(Texture* texture, Rect source);

		inline bool justResized() { return data.justResized; }
		void setMinimalSize(int Width, int Height);
		inline int getWidth() { return data.Width; }
		inline int getHeight() { return data.Height; }
		inline bool isMinimized() { return data.Minimized; }
		inline float getDeltaTime() { return deltaTime; }
		inline Graphics* getGraphics() { return graphics; }
};