#pragma once
#include <Utils/Math.h>
#include <utility>

struct Texture {
	const int Width;
	const int Height;

	const Rect bounds;

	unsigned char* data;

	Texture(int Width, int Height, unsigned char* data = nullptr) : Width(Width), Height(Height), data(data), bounds(Rect(0, 0, Width, Height)) {}
	virtual ~Texture() = default;
};

//A Implemented RenderTexture must inherit this Struct and it's implemented Texture Struct
struct RenderTexture : public virtual Texture {
	RenderTexture(int Width, int Height) : Texture(Width, Height) {}
	virtual ~RenderTexture() = default;
};

