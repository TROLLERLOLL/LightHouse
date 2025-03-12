#include <Graphics/DX11/Graphics.DX11.h>
#include <Graphics/DX11/DX11.h>
#include <Graphics/DX11/Shader.DX11.h>
#include <Graphics/DX11/Spritebatch.DX11.h>

DirectX11::DirectX11(HWND hwnd, GameWindow* window, bool software) : Graphics(window, GraphicsBackend::DirectX11) {
	DXGI_SWAP_CHAIN_DESC scdesc = {};
	scdesc.BufferCount = 1;
	scdesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scdesc.BufferDesc.Width = window->getWidth();
	scdesc.BufferDesc.Height = window->getHeight();
	scdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scdesc.OutputWindow = hwnd;
	scdesc.SampleDesc.Count = 1;
	scdesc.Windowed = true;
	scdesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, software ? D3D_DRIVER_TYPE_WARP : D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scdesc, &swapchain, &device, nullptr, &context);
	DXError(hr, "Failed to create Device and Swap Chain");

	ID3D11Texture2D* backbuffer = nullptr;
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbuffer);
	DXError(hr, "Failed to get Backbuffer from Swapchain");
	hr = device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView);
	DXError(hr, "Failed to create Backbuffer RenderTarget");
	backbuffer->Release();
	context->OMSetRenderTargets(1, &renderTargetView, nullptr);

	setViewport(0, 0, window->getWidth(), window->getHeight());
	printf("Created DirectX11 Context: %s\n", "DirectX11");


	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ID3D11BlendState* blendState;
	hr = device->CreateBlendState(&blendDesc, &blendState);
	DXError(hr, "Couldn't create blend State");
	context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
	blendState->Release();

	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;

	ID3D11RasterizerState* rasterizerState;
	device->CreateRasterizerState(&rasterDesc, &rasterizerState);
	context->RSSetState(rasterizerState);
	rasterizerState->Release();
}

DirectX11::~DirectX11() {
	delete getSpritebatch();
	for (RenderTexture* target : renderTargets)
		delete target;

	renderTargets.clear();

	swapchain->Release();
	context->Release();
	device->Release();
}

bool DirectX11::isSupported(bool software) {
	HRESULT hr = S_OK;

	ID3D11Device* tempDevice = nullptr;
	D3D_FEATURE_LEVEL featureLevel;

	//Software should be supported always
	hr = D3D11CreateDevice(nullptr, software ? D3D_DRIVER_TYPE_WARP : D3D_DRIVER_TYPE_HARDWARE, 0, 0, nullptr, 0, D3D11_SDK_VERSION, &tempDevice, &featureLevel, nullptr);
	if (FAILED(hr))
		return false;

	if (featureLevel >= D3D_FEATURE_LEVEL_11_0) {
		tempDevice->Release();
		return true;
	}

	tempDevice->Release();
	return false;
}

void DirectX11::BeginFrame() {

}

void DirectX11::EndFrame() {

}

void DirectX11::OnResize(int Width, int Height) {
	if (window->isMinimized())
		return;

	if (renderTargetView != NULL) {
		renderTargetView->Release();
		renderTargetView = NULL;
	}

	//Can result in an error
	HRESULT hr = swapchain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	DXError(hr, "Failed to Resize Swapchain Buffers");
	ID3D11Texture2D* backbuffer = nullptr;
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbuffer);
	DXError(hr, "Failed to get Backbuffer from Swapchain in OnResize");

	hr = device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView);
	DXError(hr, "Failed to create BackBuffer RenderTarget in OnResize");
	backbuffer->Release();

	context->OMSetRenderTargets(1, &renderTargetView, nullptr);
	setViewport(0, 0, Width, Height);
}

void DirectX11::Clear(float R, float G, float B, float A) {
	float clearColor[] = {R, G, B, A};
	//context->ClearRenderTargetView(renderTargetView, clearColor);

	if (activeRenderTarget == nullptr)
		context->ClearRenderTargetView(renderTargetView, clearColor);
	else
		context->ClearRenderTargetView(((DX11RenderTexture*)activeRenderTarget)->target, clearColor);
}

void DirectX11::Present() {
	if (!window->isMinimized())
		swapchain->Present(window->getVsync() ? 1 : 0, 0);
	else {
		if (window->getVsync())
			Sleep(1); //Max Framerate should be 1000 when minimized;
		swapchain->Present(0, DXGI_PRESENT_TEST);
	}
		//Sleep(8); //Max Framerate should be 120 than
}

RenderTexture* DirectX11::createRenderTarget(int Width, int Height) {
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = Width;
	desc.Height = Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	ID3D11Texture2D* texture = NULL;
	device->CreateTexture2D(&desc, nullptr, &texture);

	ID3D11RenderTargetView* rendertarget;
	device->CreateRenderTargetView(texture, nullptr, &rendertarget);

	ID3D11ShaderResourceView* shaderResourceView;
	device->CreateShaderResourceView(texture, nullptr, &shaderResourceView);

	RenderTexture* target = (RenderTexture*)new DX11RenderTexture(rendertarget, texture, shaderResourceView, Width, Height);
	renderTargets.push_back(target);
	return target;
}

void DirectX11::setRenderTarget(RenderTexture* temptarget) {
	if (temptarget == nullptr) {
		activeRenderTarget = nullptr;
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);
		setViewport(0, 0, window->getWidth(), window->getHeight());
		return;
	}

	DX11RenderTexture* target = (DX11RenderTexture*)temptarget;
	activeRenderTarget = target;
	context->OMSetRenderTargets(1, &target->target, nullptr);
	setViewport(0, 0, target->Width, target->Height);
}

void DirectX11::setViewport(int X, int Y, int Width, int Height) {
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = X;
	viewport.TopLeftY = Y;
	viewport.Width = (float)Width;
	viewport.Height = (float)Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	Viewport = Rect(X, Y, Width, Height);
}

SpriteBatch* DirectX11::getSpritebatch() {
	static SpriteBatch* spritebatch = (SpriteBatch*)new SpritebatchDX11(this, device, context);
	return spritebatch;
}

Shader* DirectX11::getShader(const char* name) {
	Shader* shader = (Shader*)new ShaderDX11(SpriteShader_DX11_Pixel, SpriteShader_DX11_Vertex, this, device, context);
	return shader;
}

Texture* DirectX11::createTexture(int Width, int Height, const unsigned char* data) {
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = Width;
	desc.Height = Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	
	D3D11_SUBRESOURCE_DATA texdata = {};
	texdata.pSysMem = data;
	texdata.SysMemPitch = Width * 4;

	//Creating the Texture
	ID3D11Texture2D* texture;
	HRESULT hr = device->CreateTexture2D(&desc, &texdata, &texture);
	DXError(hr, "Failed to create Texture in Create Texture");
	ID3D11ShaderResourceView* shaderResource;
	hr = device->CreateShaderResourceView(texture, nullptr, &shaderResource);
	DXError(hr, "Failed to create Shader Resource View in create Texture");
	return new DX11Texture(texture, shaderResource, Width, Height, (unsigned char*)data);
}

void DirectX11::DXError(HRESULT hr, const char* text) {
	if (!FAILED(hr))
		return;
	char* error = nullptr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&error, 0, nullptr);
	printf("%s: %s\n", text, error ? error : "");
	if (error)
		LocalFree(error);
}

void DirectX11::setWireframe(bool value) {

}