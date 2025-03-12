#pragma once

#include <Graphics/Graphics.h>
#include <Graphics/Window.h>

#ifdef DX11
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

typedef long HRESULT;

struct HWND__;
typedef HWND__* HWND;
#endif


class DirectX11 : public Graphics {
	private:
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		IDXGISwapChain* swapchain = nullptr;
		ID3D11RenderTargetView* renderTargetView = nullptr;

	public:
		DirectX11(HWND HWND, GameWindow* window, bool software = false);
		~DirectX11();
		static bool isSupported(bool software = false);

		void DXError(HRESULT hr, const char* text);
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