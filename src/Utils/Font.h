#pragma once

#include <Graphics/Texture.h>
#include <unordered_map>


struct Glyph {
	int xOffset;
	int yOffset;
	int width;
	int height;
};

struct Font {
	Texture* texture;
	std::unordered_map<unsigned char, Glyph> glyphs;
	const int TextSize;
	Font(Texture* texture, int TextSize) : texture(texture), TextSize(TextSize) {}

	~Font() {
//		delete texture;
		glyphs.clear();
	}
};