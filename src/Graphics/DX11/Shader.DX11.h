#pragma once

#include <Graphics/Shader.h>
#ifdef DX11
#include <Graphics/DX11/Graphics.DX11.h>
#include <Graphics/DX11/DX11.h>
#endif

class ShaderDX11 : public Shader {
	private:
#ifdef DX11
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11InputLayout* inputLayout;
		void CompileShader(const char* src, const char* entryPoint, const char* target, ID3DBlob*& blob);
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		DirectX11* graphics;
#endif
	public:
		ShaderDX11(const char* PixelShader, const char* VertexShader, DirectX11* graphics, ID3D11Device* device, ID3D11DeviceContext* context);
		ShaderDX11(const char* CombinedShader, DirectX11* graphics, ID3D11Device* device, ID3D11DeviceContext* context);
		void use() override;
		void setBool(const std::string& name, bool value) override;
		void setInt(const std::string& name, int value) override;
		void setFloat(const std::string& name, float value) override;
		void setVector2(const std::string& name, Vector2 value) override;
		void setMatrix(const std::string& name, Matrix value) override;
};