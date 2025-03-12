#include <Graphics/DX11/Spritebatch.DX11.h>
#include <Graphics/DX11/DX11.h>
#include <Graphics/DX11/Graphics.DX11.h>
#include <Graphics/DX11/Shader.DX11.h>

SpritebatchDX11::SpritebatchDX11(Graphics* graphics, ID3D11Device* device, ID3D11DeviceContext* context) : SpriteBatch(graphics) {
	this->device = device;
	this->context = context;

	//We calculate the UV using the Position which is static
	float vertices[] = {
		//pos	tex
		1, 0,//	1, 1,	//Top Right
		0, 0,//	0, 1,	//Top Left
		0, 1,//	0, 0,	//Bottom Left
		1, 1,//	1, 0,	//Bottom Right
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices;

	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices;

	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);


	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Sprite) * MaxSprites;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA instancedata = {};
	std::vector<Sprite> tempSprite(MaxSprites);
	instancedata.pSysMem = tempSprite.data();

	HRESULT hr = device->CreateBuffer(&desc, &instancedata, &instanceBuffer);
	((DirectX11*)graphics)->DXError(hr, "Failed to create Instanced Data Buffer");
}

void SpritebatchDX11::flush() {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = context->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	((DirectX11*)graphics)->DXError(hr, "Failed to map Instance Buffer");

	memcpy(mappedResource.pData, sprites.data(), SpriteCount * sizeof(Sprite));
	context->Unmap(instanceBuffer, 0);

	context->DrawIndexedInstanced(6, (UINT)SpriteCount, 0, 0, 0);
	
	sprites.clear();
	SpriteCount = 0;
}

void SpritebatchDX11::BeginInternal() {
	D3D11_SAMPLER_DESC samplerdesc = {};
	samplerdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerdesc.MipLODBias = 0.0f;
	samplerdesc.MaxAnisotropy = 1;
	samplerdesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerdesc.BorderColor[0] = 0.0f;
	samplerdesc.BorderColor[1] = 0.0f;
	samplerdesc.BorderColor[2] = 0.0f;
	samplerdesc.BorderColor[3] = 0.0f;
	samplerdesc.MinLOD = 0.0f;
	samplerdesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&samplerdesc, &sampler);
	((DirectX11*)graphics)->DXError(hr, "Failed to create Sampler");

	shader->use();
	shader->setMatrix("perspective", (matrix * DefaultMatrix));
	context->PSSetSamplers(0, 1, &sampler);

	UINT stride = sizeof(float) * 2;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	stride = sizeof(Sprite);
	offset = 0;
	context->IASetVertexBuffers(1, 1, &instanceBuffer, &stride, &offset);
}

void SpritebatchDX11::EndInternal() {
	ID3D11ShaderResourceView* emptyView = nullptr;
	context->PSSetShaderResources(0, 1, &emptyView);
	sampler->Release();
}


void SpritebatchDX11::BindTexture() {
	DX11Texture* texture = dynamic_cast<DX11Texture*>(currentTexture);
	context->PSSetShaderResources(0, 1, &texture->shaderResource);
}

SpritebatchDX11::~SpritebatchDX11() {
	instanceBuffer->Release();
	vertexBuffer->Release();
	indexBuffer->Release();
}