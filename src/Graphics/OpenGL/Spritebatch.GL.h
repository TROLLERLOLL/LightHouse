#pragma once
#include <Graphics/Spritebatch.h>

class SpritebatchGL : public SpriteBatch {
	protected:
		void flush() override;
		bool needsToBeFlipped(Texture* texture) override;
		void BeginInternal() override;
		void EndInternal() override;
		void BindTexture() override;

	private:
		unsigned int VBO;
		unsigned int VBOinstance;
		unsigned int VAO;
		unsigned int EBO;

	public:
		SpritebatchGL(Graphics* graphics);
		~SpritebatchGL();
};