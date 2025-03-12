#include <Graphics/Spritebatch.h>
#include <Graphics/Graphics.h>

void SpriteBatch::Begin(Shader* shader, Matrix matrix) {
	this->shader = shader;
	this->matrix = matrix;
	currentTexture = 0;
	this->DefaultMatrix = Matrix::CreateOrthographicOffCenter(0, graphics->getViewport().Size.X, graphics->getViewport().Size.Y, 0, 0.0f, -10.0f);

	BeginInternal();
}

void SpriteBatch::Begin(Shader* shader) {
	Begin(shader, Matrix::Identity());
}

void SpriteBatch::DrawString(Font* font, std::string text, Vector2 pos, Vector4 Color) {
	DrawString(font, text, pos, Color, 1);
}

void SpriteBatch::DrawString(Font* font, std::string text, Vector2 pos, Vector4 Color, float Scale) {
	Scale *= 16.0f / font->TextSize;
	float XOffset = 0;
	int padding = 4;
	for (int i = 0; i < text.length(); i++) {
		char letter = text[i];
		if (letter == ' ') {
			XOffset += (font->TextSize / 4 + padding) * Scale;
			continue;
		}

		if (font->glyphs.find(letter) != font->glyphs.end()) {
			Glyph glyph = font->glyphs[letter];
			Draw(font->texture, Vector2(pos.X + XOffset, pos.Y), Vector2(glyph.width * Scale, glyph.height * Scale), Rect((int)glyph.xOffset, (int)glyph.yOffset, (int)glyph.width, (int)glyph.height), Color);
			XOffset += (glyph.width + padding) * Scale;
		}
	}
}

void SpriteBatch::Draw(Texture* texture, Vector2 pos, Vector2 Size, Vector2 UVTL, Vector2 UVBR, Vector4 Color) {
	if (Color.W == 0.0f) //If it's transparent you can just not draw it; might cause problems but doubt it
		return;

	if (currentTexture == nullptr) {
		currentTexture = texture;
		BindTexture();
	}
	if (currentTexture != texture) {
		flush();
		currentTexture = texture;
		BindTexture();
	}

	if (SpriteCount >= MaxSprites)
		flush();

	SpriteCount++;
	sprites.push_back({ pos, Size, Vector4(UVTL.X, UVTL.Y, UVBR.X, UVBR.Y), Color});
}

void SpriteBatch::Draw(Texture* texture, Vector2 pos, Vector2 Size, Rect Source, Vector4 Color, bool flipX, bool flipY) {
	float UVX = (float)Source.Location.X / (float)texture->Width;
	float UVY = (float)Source.Location.Y / (float)texture->Height;
	float UVWidth = (float)(Source.Size.X + Source.Location.X) / (float)texture->Width;
	float UVHeight = (float)(Source.Size.Y + Source.Location.Y) / (float)texture->Height;

	float UVup = flipY ? UVHeight : UVY;
	float UVdown = flipY ? UVY : UVHeight;
	float UVleft = flipX ? UVWidth : UVX;
	float UVright = flipX ? UVX : UVWidth;
	
	Draw(texture, pos, Size, Vector2(UVleft, UVup), Vector2(UVright, UVdown), Color);
}

void SpriteBatch::Draw(Texture* texture, Vector2 pos, Vector2 Size, Rect Source, Vector4 Color, bool flipX) {
	Draw(texture, pos, Size, Source, Color, flipX, false);
}

void SpriteBatch::Draw(Texture* texture, Vector2 pos, Vector2 Size, Rect Source, Vector4 Color) {
	Draw(texture, pos, Size, Source, Color, false, false);
}

void SpriteBatch::Draw(Texture* texture, Vector2 position, Vector2 Scale, Vector4 Color) {
	Draw(texture, position, Vector2(texture->Width * Scale.X, texture->Height * Scale.Y), Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), Color);
}

void SpriteBatch::Draw(Texture* texture, Vector2 position, float Scale, Vector4 Color) {
	Draw(texture, position, Vector2(Scale, Scale), Color);
}

void SpriteBatch::Draw(Texture* texture, Vector2 position, Vector4 Color) {
	Draw(texture, position, 1.0f, Color);
}

void SpriteBatch::Draw(Texture* texture, Rect destination, Rect source, Vector4 Color) {
	Draw(texture, Vector2(destination.Location.X, destination.Location.Y), Vector2(destination.Size.X, destination.Size.Y), source, Color);
}

void SpriteBatch::Draw(Texture* texture, Rect destination, Vector4 Color) {
	Draw(texture, destination, Rect(0, 0, texture->Width, texture->Height), Color);
}

void SpriteBatch::End() {
	if (sprites.size() != 0)
		flush();

	currentTexture = nullptr;
	EndInternal();
}