#pragma once

#include <Graphics/Graphics.h>

class OpenGL : public Graphics {
	private:
		float R, G, B, A;
	public:
		OpenGL(GameWindow* window, void* proc);
		~OpenGL();

		void OnResize(int Width, int Height) override;
		void BeginFrame() override;
		void EndFrame() override;
		void Present() override;
		void Clear(float R, float G, float B, float A) override;
		RenderTexture* createRenderTarget(int Width, int Height) override;
		void setRenderTarget(RenderTexture* target) override;
		void setViewport(int X, int Y, int Width, int Height) override;
		SpriteBatch* getSpritebatch() override;
		Shader* getShader(const char* name) override;
		Texture* createTexture(int Width, int Height, const unsigned char* data) override;
		void setWireframe(bool value) override;
};