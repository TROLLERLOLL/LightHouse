#include <Graphics/DX11/Shader.DX11.h>
#include <Graphics/DX11/DX11.h>
#include <Graphics/DX11/Graphics.DX11.h>

ShaderDX11::ShaderDX11(const char* code, DirectX11* graphics, ID3D11Device* device, ID3D11DeviceContext* context) : ShaderDX11(code, code, graphics, device, context) {}

ShaderDX11::ShaderDX11(const char* PixelShader, const char* VertexShader, DirectX11* graphics, ID3D11Device* device, ID3D11DeviceContext* context) {
	this->graphics = graphics;
	this->device = device;
	this->context = context;
	ID3DBlob* vsBlob;
	ID3DBlob* psBlob;
	CompileShader(VertexShader, "vs_main", "vs_5_0", vsBlob);
	CompileShader(PixelShader, "ps_main", "ps_5_0", psBlob);
	HRESULT hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);
	graphics->DXError(hr, "Failed to create Vertex Shader");
	hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);
	graphics->DXError(hr, "Failed to create Pixel Shader");


	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"INSTANCEPOSSCALE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
	};
	hr = device->CreateInputLayout(layout, ARRAYSIZE(layout), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
	graphics->DXError(hr, "Failed to create Input Layout");
}

void ShaderDX11::CompileShader(const char* src, const char* entryPoint, const char* target, ID3DBlob*& blob) {
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* errorblob;
	HRESULT hr = D3DCompile(src, strlen(src), "File", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, target, flags, 0, &blob, &errorblob);
	graphics->DXError(hr, "Failed to compile Shader");
}

void ShaderDX11::use() {
	context->IASetInputLayout(inputLayout);
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);
}

void ShaderDX11::setBool(const std::string& name, bool value) {
	
}

void ShaderDX11::setInt(const std::string& name, int value) {
	
}

void ShaderDX11::setFloat(const std::string& name, float value) {
	
}

void ShaderDX11::setVector2(const std::string& name, Vector2 value) {

}

void ShaderDX11::setMatrix(const std::string& name, Matrix value) {
	ID3D11Buffer* constantBuffer;
	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(value);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = &value;

	device->CreateBuffer(&desc, &initData, &constantBuffer);
	context->VSSetConstantBuffers(0, 1, &constantBuffer);
	constantBuffer->Release();
}