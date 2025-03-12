#include <Graphics/OpenGL/Spritebatch.GL.h>
#include <Graphics/OpenGL/GL.h>
#include <cstring>

SpritebatchGL::SpritebatchGL(Graphics* graphics) : SpriteBatch(graphics) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//we calculate the UVs using the Position because it's static and never changes
	float vertices[] = {
		//pos	tex
		1, 0,//	1, 1,	//Top Right
		0, 0,//	0, 1,	//Top Left
		0, 1,//	0, 0,	//Bottom Left
		1, 1,//	1, 0,	//Bottom Right
	};

	unsigned char indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);


	glGenBuffers(1, &VBOinstance);
	glBindBuffer(GL_ARRAY_BUFFER, VBOinstance);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sprite) * MaxSprites, nullptr, GL_DYNAMIC_DRAW);
}

void SpritebatchGL::BeginInternal() {
	shader->use();
	shader->setMatrix("perspective", matrix * DefaultMatrix);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBOinstance);
}

void SpritebatchGL::EndInternal() {
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glBindVertexArray(0);
}

void SpritebatchGL::BindTexture() {
	GLTexture* texture = dynamic_cast<GLTexture*>(currentTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->ID);
}

SpritebatchGL::~SpritebatchGL() {
	unsigned int buffers[] = { VBO, EBO, VBOinstance };
	glDeleteBuffers(3, buffers);
	glDeleteVertexArrays(1, &VAO);
}

void SpritebatchGL::flush() {
	Sprite* InstancedSprites = (Sprite*)glMapBufferRange(GL_ARRAY_BUFFER, 0, SpriteCount * sizeof(Sprite), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	if (InstancedSprites) {
		//for (int i = 0; i < SpriteCount; i++)
		//	InstancedSprites[i] = sprites[i];
		memcpy(InstancedSprites, sprites.data(), SpriteCount * sizeof(Sprite));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	//Position and Size
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)0);
	glEnableVertexAttribArray(1);

	//UV TL and BR
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)(sizeof(Vector2) * 2));
	glEnableVertexAttribArray(2);

	//Color
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)(sizeof(Vector2) * 4));
	glEnableVertexAttribArray(3);

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, SpriteCount);
	
	SpriteCount = 0;
	sprites.clear();
}

bool SpritebatchGL::needsToBeFlipped(Texture* texture) {
	return dynamic_cast<GLTexture*>(texture)->needsToBeFlipped;
}