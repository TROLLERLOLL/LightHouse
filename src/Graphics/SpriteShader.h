#pragma once

//Shaders are currently hardcoded
#ifdef GL
static const char* SpriteShader_GL_Fragment = R"(
#version 330 core

out vec4 color;
in vec2 uv;
in vec4 col;

uniform sampler2D tex;

void main()
{
	color = texture(tex, uv) * col;
}
)";

static const char* SpriteShader_GL_Vertex = R"(
#version 330 core
layout (location = 0) in vec2 vertexPosUV;

layout (location = 1) in vec4 positionSize;
layout (location = 2) in vec4 UV;
layout (location = 3) in vec4 color;

uniform mat4 perspective;

out vec2 uv;
out vec4 col;

void main()
{
	//UV starting pos + width/height * original offset by vertexUV
	uv = vec2((1 - vertexPosUV.x) * UV.x + vertexPosUV.x * UV.z, 1-(vertexPosUV.y * UV.w + (1 - vertexPosUV.y) * UV.y));
	gl_Position = perspective * vec4(positionSize.xy + (vertexPosUV * positionSize.zw), 0.0, 1.0);
	col = color;
}
)";
#endif

#ifdef DX11
static const char* SpriteShader_DX11_Vertex = R"(
struct vs_in {
    float2 vertexPos : POSITION;
	float4 positionSize : INSTANCEPOSSCALE;
	float4 UV : UV;
	float4 color : COLOR;
};

cbuffer Perspective : register(b0) 
{
  float4x4 perspective;
}

struct vs_out {
    float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR;
};

vs_out vs_main(vs_in input) {
  vs_out output = (vs_out)0; // zero the memory first
  output.position = mul(perspective, float4(input.positionSize.xy + (input.vertexPos.xy * input.positionSize.zw), 0.0f, 1.0));
  output.uv = float2((1 - input.vertexPos.x) * input.UV.x + input.vertexPos.x * input.UV.z, input.vertexPos.y * input.UV.w + (1 - input.vertexPos.y) * input.UV.y);
  output.color = input.color;
  return output;
}
)";

static const char* SpriteShader_DX11_Pixel = R"(
struct vs_out {
    float4 position : SV_POSITION; // required output of VS
	float2 uv : TEXCOORD0;
	float4 color : COLOR;
};

Texture2D tex : register(t0);
SamplerState texsampler : register(s0);

float4 ps_main(vs_out input) : SV_TARGET {
  return tex.Sample(texsampler, input.uv) * input.color; // must return an RGBA colour
}
)";
#endif