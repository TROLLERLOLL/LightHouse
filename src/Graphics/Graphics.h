#pragma once
#include <Graphics/Texture.h>
#include <Graphics/Shader.h>
#include <vector>

class GameWindow;
class SpriteBatch;

enum class GraphicsBackend {
	Best,
	OpenGL,
	DirectX11,
};

class Graphics {
	protected:
		std::vector<RenderTexture*> renderTargets;
		RenderTexture* activeRenderTarget = nullptr;
		GameWindow* window;
		Rect Viewport;

	public:
		Graphics(GameWindow* window, GraphicsBackend backend) : window(window), backend(backend) {};
		
		virtual void OnResize(int Width, int Height) = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Present() = 0;
		virtual void Clear(float R, float G, float B, float A) = 0;
		virtual RenderTexture* createRenderTarget(int Width, int Height) = 0;
		virtual void setRenderTarget(RenderTexture* target) = 0;
		virtual void setViewport(int X, int Y, int Width, int Height) = 0;
		virtual SpriteBatch* getSpritebatch() = 0;
		virtual Shader* getShader(const char* name) = 0;
		virtual Texture* createTexture(int Width, int Height, const unsigned char* data) = 0;
		virtual void setWireframe(bool value) = 0;
		const GraphicsBackend backend;
		
		inline Rect getViewport() { return Viewport; }
};
