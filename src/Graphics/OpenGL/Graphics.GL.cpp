#include <Graphics/OpenGL/Graphics.GL.h>
#include <Graphics/OpenGL/GL.h>
#include <Graphics/OpenGL/Spritebatch.GL.h>
#include <Graphics/OpenGL/Shader.GL.h>
#include <Graphics/SpriteShader.h>
#include <Graphics/Window.h>

OpenGL::OpenGL(GameWindow* window, void* proc) : Graphics(window, GraphicsBackend::OpenGL) {
	loadGL(proc);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	setViewport(0, 0, window->getWidth(), window->getHeight());
	printf("Created GL Context: %s\n", glGetString(GL_VERSION));
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_SCISSOR_TEST);
}

OpenGL::~OpenGL() {
	printf("Unloaded OpenGL\n");
	//unloadGL();
}

void OpenGL::OnResize(int Width, int Height) {

}

void OpenGL::BeginFrame() {

}

void OpenGL::EndFrame() {

}

void OpenGL::Clear(float R, float G, float B, float A) {
	if (R != this->R || G != this->G || B != this->B || A != this->A) {
		this->R = R;
		this->G = G;
		this->B = B;
		this->A = A;
		glClearColor(R, G, B, A);
	}
	glClear(GL_COLOR_BUFFER_BIT);
}

RenderTexture* OpenGL::createRenderTarget(int Width, int Height) {
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderTexture* target = (RenderTexture*)new GLRenderTexture(fbo, tex, Width, Height);
	renderTargets.push_back(target);
	return target;
}

void OpenGL::setRenderTarget(RenderTexture* temptarget) {
	GLRenderTexture* target = (GLRenderTexture*)temptarget;

	glBindFramebuffer(GL_FRAMEBUFFER, (target != nullptr) ? target->fbo : 0);
	if (target != nullptr)
		setViewport(0, 0, target->Width, target->Height);
	else
		setViewport(0, 0, window->getWidth(), window->getHeight());
}

void OpenGL::setViewport(int X, int Y, int Width, int Height) {
	glViewport(X, Y, Width, Height);
	Viewport = Rect(X, Y, Width, Height);
}

SpriteBatch* OpenGL::getSpritebatch() {
	static SpriteBatch* spritebatch = (SpriteBatch*)new SpritebatchGL(this);
	return spritebatch;
}

Shader* OpenGL::getShader(const char* name) {
	return (Shader*)new ShaderGL(SpriteShader_GL_Fragment, SpriteShader_GL_Vertex);
}

Texture* OpenGL::createTexture(int Width, int Height, const unsigned char* data) {
	unsigned int ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return new GLTexture(ID, Width, Height, false, (unsigned char*)data);
}

/*
Texture* OpenGL::createFontTexture(int Width, int Height, const unsigned char* data) {
	unsigned int ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, Width, Height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	return new GLTexture(ID, Width, Height);
}*/

void OpenGL::Present() {}

void OpenGL::setWireframe(bool value) {
	if (value) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}