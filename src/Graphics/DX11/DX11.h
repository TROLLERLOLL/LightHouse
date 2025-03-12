#pragma once

#ifdef DX11
#include <Graphics/Texture.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <windows.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <cstdlib>

struct DX11Texture : public virtual Texture {
	ID3D11Texture2D* texture = NULL;
	ID3D11ShaderResourceView* shaderResource = NULL;

	DX11Texture(ID3D11Texture2D* texture, ID3D11ShaderResourceView* shaderResource, int Width, int Height, unsigned char* data = nullptr) : texture(texture), shaderResource(shaderResource), Texture(Width, Height, data) {}

	~DX11Texture() {
		if (data != nullptr)
			free(data);

		shaderResource->Release();
		texture->Release();
	}
};

struct DX11RenderTexture : public RenderTexture, public DX11Texture {
	ID3D11RenderTargetView* target = NULL;
	DX11RenderTexture(ID3D11RenderTargetView* target, ID3D11Texture2D* texture, ID3D11ShaderResourceView* shaderResource, int Width, int Height) : target(target), Texture(Width, Height), RenderTexture(Width, Height), DX11Texture(texture, shaderResource, Width, Height) { }
	~DX11RenderTexture() {
		target->Release();
	}
};
#endif