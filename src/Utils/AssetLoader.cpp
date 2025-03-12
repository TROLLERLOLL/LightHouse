#include <Utils/AssetLoader.h>
#include <Graphics/Graphics.h>

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

Texture* AssetLoader::loadTexture(const char* path) {
	if (textures.find(path) != textures.end())
		return textures[path];

	int Width, Height, nrChannels;
	stbi_set_flip_vertically_on_load(graphics->backend == GraphicsBackend::OpenGL);
	unsigned char* data = stbi_load(path, &Width, &Height, &nrChannels, STBI_rgb_alpha);
	Texture* texture = graphics->createTexture(Width, Height, data);

	//stbi_image_free(data);

	textures.emplace(path, texture);

	return texture;
}

Texture* AssetLoader::loadTexture(const char* name, unsigned char* bytes, int length) {
	if (textures.find(name) != textures.end())
		return textures[name];

	int Width, Height, nrChannels;
	stbi_set_flip_vertically_on_load(graphics->backend == GraphicsBackend::OpenGL);
	unsigned char* data = stbi_load_from_memory(bytes, length, &Width, &Height, &nrChannels, STBI_rgb_alpha);

	Texture* texture = graphics->createTexture(Width, Height, data);

	//Don't free the data we are might activly use that data when accessing the data of a texture
	//stbi_image_free(data);

	textures.emplace(name, texture);

	return texture;
}

Font* AssetLoader::loadFont(const char* path) {
	if (fonts.find(path) != fonts.end())
		return fonts[path];

	FILE* fontFile = fopen(path, "rb");
	if (!fontFile) { return nullptr; }
	fseek(fontFile, 0, SEEK_END);
	long fontSize = ftell(fontFile);
	fseek(fontFile, 0, SEEK_SET);
	std::vector<unsigned char> bytes(fontSize);
	fread(bytes.data(), 1, fontSize, fontFile);
	fclose(fontFile);
	return loadFont(path, bytes.data());
}

Font* AssetLoader::loadFont(const char* name, unsigned char* bytes) {
	if (fonts.find(name) != fonts.end())
		return fonts[name];

	stbtt_fontinfo fontInfo;
	if (!stbtt_InitFont(&fontInfo, bytes, stbtt_GetFontOffsetForIndex(bytes, 0))) {
		//Failed
		return nullptr;
	}

	int offsetFromBorder = 2;
	int FontPixelSize = 64;
	int padding = 2;
	float Scale = stbtt_ScaleForPixelHeight(&fontInfo, FontPixelSize);
	const int firstGlyph = 32;
	const int lastGlyph = 256;

	int atlasWidth = 512;
	int atlasHeight = 512;

	std::vector<unsigned char> atlas(atlasWidth * atlasHeight, 0);
	int xOffset = offsetFromBorder;
	int yOffset = offsetFromBorder;
	int maxHeightInRow = 0;

	Font* font = new Font(0, FontPixelSize);

	for (int i = firstGlyph; i <= lastGlyph; i++) {
		int width = 0, height = 0, xOff = 0, yOff = 0;
		unsigned char* bitmap = stbtt_GetCodepointBitmap(&fontInfo, Scale, Scale, i, &width, &height, &xOff, &yOff);
		if (0 == height && width == 0) {
			stbtt_FreeBitmap(bitmap, nullptr);
			continue;
		}

		if (xOffset + width > atlasWidth) {
			xOffset = offsetFromBorder;
			yOffset += maxHeightInRow + padding;
			maxHeightInRow = 0;
		}
		bool success = false;
		bool flipped = graphics->backend == GraphicsBackend::OpenGL;
		int tempAtlasIndex = ((flipped ? atlasHeight - 1 - (yOffset + height) : yOffset + height) * atlasWidth + (xOffset + width));
		if (tempAtlasIndex < atlasWidth * atlasHeight) {
			success = true;
			for (int row = 0; row < height; row++) {
				for (int col = 0; col < width; col++) {
					int atlasIndex = ((flipped ? atlasHeight - 1 - (yOffset + row) : yOffset + row) * atlasWidth + (xOffset + col));
					if (atlasIndex >= (atlasWidth * atlasHeight) || atlasIndex < 0) {
						success = false;
						row = height;
						break;
					}
					int bitmapIndex = (row * width + col);
					unsigned char color = bitmap[bitmapIndex];
					atlas[atlasIndex + 0] = color;
				}
			}
		}
		

		if (success)
			font->glyphs.emplace((unsigned char)i, Glyph{ xOffset, yOffset, width, height });
		xOffset += width + padding;
		maxHeightInRow = std::max(maxHeightInRow, height);

		stbtt_FreeBitmap(bitmap, nullptr);
	}
	std::vector<unsigned char> tempdata;
	int length = atlasHeight * atlasHeight;
	for (int i = 0; i < length; i++) {
		tempdata.push_back(atlas[i]);
		tempdata.push_back(atlas[i]);
		tempdata.push_back(atlas[i]);
		tempdata.push_back(atlas[i]);
	}

	font->texture = graphics->createTexture(atlasHeight, atlasHeight, tempdata.data());
	fonts.emplace(name, font);
	return font;
}

void AssetLoader::unloadTexture(const char* path) {
	if (textures.find(path) == textures.end())
		return;
	Texture* texture = textures[path];
	if (texture != NULL)
		delete texture;
}

void AssetLoader::unloadTextures() {
	for (auto texture : textures)
		unloadTexture(texture.first.c_str());
}

AssetLoader::~AssetLoader() {
	for (auto texture : textures)
		delete texture.second;

	textures.clear();

	for (auto font : fonts)
		delete font.second;
	fonts.clear();
}