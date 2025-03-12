#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <Graphics/Window.h>
#include <cstring>
#ifdef GL
#include <GL/gl.h>
#include <GL/glx.h>
#endif

void GameWindow::Error(const char* text, const char* filename, int line) {
	const char* message = (std::string(text)).c_str();// +"\n(" + __FILENAME__ + ":" + std::to_string(__LINE__) + ")").c_str();

	int screen = DefaultScreen(this->display);
	Window box = XCreateSimpleWindow(display, DefaultRootWindow(display), 100, 100, 300, 150, 1, BlackPixel(display, screen), WhitePixel(display, screen));
	XStoreName(display, box, "Error");
	XSelectInput(display, box, ExposureMask | KeyPressMask | ButtonPressMask);
	Font font = XLoadFont(display, "fixed");
	GC gc = XCreateGC(display, box, 0, NULL);
	XFontStruct* fontInfo = XQueryFont(display, font);
	int textWidth = XTextWidth(fontInfo, message, strlen(message));
	int x = (300 - textWidth) / 2;
	int y = 50;

	Window button = XCreateSimpleWindow(display, box, 100, 80, 100, 40, 1, BlackPixel(display, screen), WhitePixel(display, screen));
	XStoreName(display, button, "Ok");
	XSelectInput(display, button, ButtonPressMask);

	XMapWindow(display, box);
	XMapWindow(display, button);

	XEvent event;
	int running = 1;
	while (running) {
		if (!XPending(display))
			continue;

		XNextEvent(display, &event);

		if (event.type == Expose)
			XDrawString(display, box, gc, x, y, message, strlen(message));
	}

	XFreeGC(display, gc);
	XUnloadFont(display, font);
	XDestroyWindow(display, box);
	XDestroyWindow(display, button);
	exit(1);
}

void GameWindow::createWindowInternal(const char* title, int Width, int Height) {
	this->display = XOpenDisplay(NULL);
	if (!display)
		Error("Failed to Open Display", __FILE__, __LINE__);

	this->screen = DefaultScreen(display);

	int ScreenWidth = DisplayWidth(display, screen);
	int ScreenHeight = DisplayHeight(display, screen);

	static int visualAttribs[] = {
		GLX_X_RENDERABLE,	1,
		GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
		GLX_RENDER_TYPE,	GLX_RGBA_BIT,
		GLX_RED_SIZE,		8,
		GLX_GREEN_SIZE,		8,
		GLX_BLUE_SIZE,		8,
		GLX_ALPHA_SIZE,		8,
		GLX_DEPTH_SIZE,		24,
		GLX_STENCIL_SIZE,	8,
		GLX_DOUBLEBUFFER,	1,
		None
	};

	int fbcount;
	GLXFBConfig* fbc = glXChooseFBConfig(display, screen, visualAttribs, &fbcount);
	if (!fbc)
		Error("Failed to create Frame Buffer Config", __FILE__, __LINE__);

	XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbc[0]);//glXChooseVisual(display, screen, visualAttribs);
	if (vi == NULL)
		Error("Failed to create VisualInfo", __FILE__, __LINE__);

	this->vii = (void*)vi;
	this->fbcc = (void*)fbc;
	Colormap colormap = XCreateColormap(display, RootWindow(display, screen), vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = colormap;
	swa.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask;

	int X = ScreenWidth / 2 - Width / 2;
	int Y = ScreenHeight / 2 - Height / 2;

	window = XCreateWindow(display, RootWindow(display, screen), X, Y, Width, Height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	if (!window)
		Error("Failed to create Window", __FILE__, __LINE__);

	XStoreName(display, window, title);
	XMapWindow(display, window);
}

void GameWindow::setTitle(const char* title) {
	data.title = title;
	XStoreName(display, window, title);
}

void GameWindow::setMinimalSizeInternal() {
	XSizeHints sizeHints;
	sizeHints.flags = PMinSize;
	sizeHints.min_width = data.MinimalWidth;
	sizeHints.min_height = data.MinimalHeight;
	XSetWMNormalHints(display, window, &sizeHints);
}

void GameWindow::setIcon(Texture* texture, Rect source) {
	//Not implemented
}


#ifdef GL
bool GameWindow::loadGL() {
	typedef GLXContext(*glXCreateContextAttribsARB_type)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
	glXCreateContextAttribsARB_type glXCreateContextAttribsARB = (glXCreateContextAttribsARB_type)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
	
	if (glXCreateContextAttribsARB) {
		int major = 3;
		int minor = 3;
		GLint attrs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, major,
			GLX_CONTEXT_MINOR_VERSION_ARB, minor,
			GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			None
		};

		context = glXCreateContextAttribsARB(display, ((GLXFBConfig*)fbcc)[0], NULL, GL_TRUE, attrs);
	}
	
	if (context == nullptr)
		context = glXCreateContext(display, ((XVisualInfo*)vii), NULL, GL_TRUE);

	if (context == nullptr)
		return false;//Error("Failed to create OpenGL Context");

	if (!glXMakeCurrent(display, window, context)) {
		glXDestroyContext(display, context);
		//Error("Failed to make OpenGL Context Current");
		return false;
	}

	glXSwapIntervalSGI = (glXSwapIntervalSGI_type)glXGetProcAddress((const GLubyte*)"glXSwapIntervalSGI");
	glXSwapIntervalEXT = (glXSwapIntervalEXT_type)glXGetProcAddress((const GLubyte*)"glXSwapIntervalEXT");
	glProc = (void*)glXGetProcAddress;
	return true;
}

bool GameWindow::LoadOpenGLVersion() {
	return true;
}
#endif

void GameWindow::setVsync(bool value) {
#ifdef	GL
	if (backend == GraphicsBackend::OpenGL) {
		if (glXSwapIntervalSGI)
			glXSwapIntervalSGI(value ? 1 : 0);
		else if (glXSwapIntervalEXT)
			glXSwapIntervalEXT(display, glXGetCurrentDrawable(), value ? 1 : 0);
	}
#endif

	vsync = value;
}

void GameWindow::showCursor(bool value) {
	if (value) {
		//Needs to be looked over
		Cursor cursor = XCreateFontCursor(display, XC_arrow);
		XDefineCursor(display, window, cursor);
		XFreeCursor(display, cursor);
	}
	else
		XUndefineCursor(display, window);
}

void GameWindow::showWindow(bool value) {
	if (value)
		XMapWindow(display, window);
	else
		XUnmapWindow(display, window);
}

KeyCodes GameWindow::getKeyCode(unsigned int key) {
	if (key == XK_BackSpace) return KeyCodes::BACK;
	if (key == XK_Tab) return KeyCodes::TAB;
	if (key == XK_Return) return KeyCodes::ENTER;
	if (key == XK_Escape) return KeyCodes::ESCAPE;
	if (key == XK_space) return KeyCodes::SPACE;
	
	if (key == XK_Shift_L) return KeyCodes::LSHIFT;
	if (key == XK_Shift_R) return KeyCodes::RSHIFT;

	if (key == XK_Control_L) return KeyCodes::LCTRL;
	if (key == XK_Control_R) return KeyCodes::RCTRL;

	if (key == XK_Alt_L) return KeyCodes::LALT;
	if (key == XK_Alt_R) return KeyCodes::RALT;

	if (key == XK_Up) return KeyCodes::UP;
	if (key == XK_Down) return KeyCodes::DOWN;
	if (key == XK_Left) return KeyCodes::LEFT;
	if (key == XK_Right) return KeyCodes::RIGHT;

	if (key == XK_0) return KeyCodes::NUM_0;
	if (key == XK_1) return KeyCodes::NUM_1;
	if (key == XK_2) return KeyCodes::NUM_2;
	if (key == XK_3) return KeyCodes::NUM_3;
	if (key == XK_4) return KeyCodes::NUM_4;
	if (key == XK_5) return KeyCodes::NUM_5;
	if (key == XK_6) return KeyCodes::NUM_6;
	if (key == XK_7) return KeyCodes::NUM_7;
	if (key == XK_8) return KeyCodes::NUM_8;
	if (key == XK_9) return KeyCodes::NUM_9;

	if (key == XK_A || key == XK_a) return KeyCodes::A;
	if (key == XK_B || key == XK_b) return KeyCodes::B;
	if (key == XK_C || key == XK_c) return KeyCodes::C;
	if (key == XK_D || key == XK_d) return KeyCodes::D;
	if (key == XK_E || key == XK_e) return KeyCodes::E;
	if (key == XK_F || key == XK_f) return KeyCodes::F;
	if (key == XK_G || key == XK_g) return KeyCodes::G;
	if (key == XK_H || key == XK_h) return KeyCodes::H;
	if (key == XK_I || key == XK_i) return KeyCodes::I;
	if (key == XK_J || key == XK_j) return KeyCodes::J;
	if (key == XK_K || key == XK_k) return KeyCodes::K;
	if (key == XK_L || key == XK_l) return KeyCodes::L;
	if (key == XK_M || key == XK_m) return KeyCodes::M;
	if (key == XK_N || key == XK_n) return KeyCodes::N;
	if (key == XK_O || key == XK_o) return KeyCodes::O;
	if (key == XK_P || key == XK_p) return KeyCodes::P;
	if (key == XK_Q || key == XK_q) return KeyCodes::Q;
	if (key == XK_R || key == XK_r) return KeyCodes::R;
	if (key == XK_S || key == XK_s) return KeyCodes::S;
	if (key == XK_T || key == XK_t) return KeyCodes::T;
	if (key == XK_U || key == XK_u) return KeyCodes::U;
	if (key == XK_V || key == XK_v) return KeyCodes::V;
	if (key == XK_W || key == XK_w) return KeyCodes::W;
	if (key == XK_X || key == XK_x) return KeyCodes::X;
	if (key == XK_Y || key == XK_y) return KeyCodes::Y;
	if (key == XK_Z || key == XK_z) return KeyCodes::Z;

	if (key == XK_F1) return KeyCodes::F1;
	if (key == XK_F2) return KeyCodes::F2;
	if (key == XK_F3) return KeyCodes::F3;
	if (key == XK_F4) return KeyCodes::F4;
	if (key == XK_F5) return KeyCodes::F5;
	if (key == XK_F6) return KeyCodes::F6;
	if (key == XK_F7) return KeyCodes::F7;
	if (key == XK_F8) return KeyCodes::F8;
	if (key == XK_F9) return KeyCodes::F9;
	if (key == XK_F10) return KeyCodes::F10;
	if (key == XK_F11) return KeyCodes::F11;
	if (key == XK_F12) return KeyCodes::F12;

	return KeyCodes::NONE;
}

void GameWindow::HandleEventsInternal() {
	XEvent event;
	while (XPending(display)) {
		XNextEvent(display, &event);

		if (event.type == KeyPress || event.type == KeyRelease) {
			bool down = (event.type == KeyPress);
			KeySym key = XLookupKeysym(&event.xkey, 0);
			KeyCodes code = getKeyCode((unsigned int)key);
			inputManager->setKey(code, down);
		}

		if (event.type == ConfigureNotify) {
			XConfigureEvent xce = event.xconfigure;
			data.Width = xce.width;
			data.Height = xce.height;
			data.justResized = true;
		}

		if (event.type == ClientMessage) {
			if ((Atom)event.xclient.data.l[0] == XInternAtom(display, "WM_DELETE_WINDOW", false)) {
				data.running = false;
				return;
			}
		}
	}
}

void GameWindow::pollEventsInternal() {
	HandleEventsInternal();

#ifdef GL
	if (backend == GraphicsBackend::OpenGL)
		glXSwapBuffers(display, window);
#endif
}

void GameWindow::closeInternal() {
#ifdef GL
	if (backend == GraphicsBackend::OpenGL) {
		glXMakeCurrent(display, None, nullptr);
		glXDestroyContext(display, context);
	}
#endif

	XFree(((GLXFBConfig*)fbcc));
	XDestroyWindow(display, window);
	XCloseDisplay(display);
}