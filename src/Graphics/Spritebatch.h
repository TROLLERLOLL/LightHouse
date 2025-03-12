#pragma once

#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <Utils/Font.h>
#include <vector>
#include <Graphics/Graphics.h>
#undef Rectangle
#include <Utils/Math.h>

class Graphics;

class SpriteBatch {
	protected:
		virtual void flush() = 0;
		struct Sprite {
			Vector2 Position;
			Vector2 Size;

			Vector4 UV;
			Vector4 Color;
		};

		std::vector<Sprite> sprites;
		Shader* shader;
		Matrix matrix;
		Matrix DefaultMatrix;
		Texture* currentTexture;
		virtual bool needsToBeFlipped(Texture* texture) = 0;

		Graphics* graphics = nullptr;
		virtual void BeginInternal() = 0;
		virtual void EndInternal() = 0;
		virtual void BindTexture() = 0;

		int SpriteCount = 0;

		const int MaxSprites = 12000;
	private:
		void Draw(Texture* texture, Vector2 position, Vector2 size, Vector2 UVTL, Vector2 UVBR, Vector4 Color);
		Vector2 topleft;
		Vector2 bottomright;

	public:
		SpriteBatch(Graphics* graphics) : graphics(graphics) {}
		void Begin(Shader* shader, Matrix matrix);
		void Begin(Shader* shader);
		void DrawString(Font* font, std::string text, Vector2 pos, Vector4 Color);
		void DrawString(Font* font, std::string text, Vector2 pos, Vector4 Color, float Scale);
		void Draw(Texture* texture, Vector2 position, Vector2 size, Rect source, Vector4 Color, bool flipX, bool flipY);
		void Draw(Texture* texture, Vector2 position, Vector2 size, Rect source, Vector4 Color, bool flipX);
		void Draw(Texture* texture, Vector2 position, Vector2 size, Rect source, Vector4 Color);
		void Draw(Texture* texture, Vector2 position, Vector4 Color);
		void Draw(Texture* texture, Rect destination, Rect source, Vector4 Color);
		void Draw(Texture* texture, Rect destination, Vector4 Color);
		void Draw(Texture* texture, Vector2 position, float Scale, Vector4 Color);
		void Draw(Texture* texture, Vector2 position, Vector2 scale, Vector4 Color);
		void End();
};