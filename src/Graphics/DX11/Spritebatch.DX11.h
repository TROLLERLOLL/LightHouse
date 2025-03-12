#pragma once

#include <Graphics/Spritebatch.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Buffer;
struct ID3D11SamplerState;

class SpritebatchDX11 : public SpriteBatch {
	protected:
		void flush() override;
		inline bool needsToBeFlipped(Texture* texture) override { return false; }
		void BeginInternal() override;
		void EndInternal() override;
		void BindTexture() override;
	private:
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		ID3D11Buffer* vertexBuffer = nullptr;
		ID3D11Buffer* indexBuffer = nullptr;
		ID3D11Buffer* instanceBuffer = nullptr;
		ID3D11SamplerState* sampler = nullptr;

	public:
		SpritebatchDX11(Graphics* graphics, ID3D11Device* device, ID3D11DeviceContext* context);
		~SpritebatchDX11();
};