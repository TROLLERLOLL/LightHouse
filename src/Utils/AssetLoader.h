#pragma once

#include <Graphics/Graphics.h>
#include <Graphics/Texture.h>
#include <Utils/Font.h>
#include <unordered_map>
#include <string>

class AssetLoader {
	private:
		std::unordered_map<std::string, Texture*> textures;
		std::unordered_map<std::string, Font*> fonts;
		Graphics* graphics;
	public:
		AssetLoader(Graphics* graphics) : graphics(graphics) {}
		~AssetLoader();
		Texture* loadTexture(const char* path);
		Texture* loadTexture(const char* name, unsigned char* bytes, int length);
		Font* loadFont(const char* path);
		Font* loadFont(const char* name, unsigned char* bytes);

		void unloadTexture(const char* path);
		void unloadTextures();
};