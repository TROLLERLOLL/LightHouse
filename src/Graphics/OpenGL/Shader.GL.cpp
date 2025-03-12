#include <Graphics/OpenGL/Shader.GL.h>
#include <Graphics/OpenGL/GL.h>

ShaderGL::ShaderGL(const char* fragmentcode, const char* vertexcode) {
	int success;
	char infoLog[512];

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexcode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR OpenGl Vertex Shader: %s", infoLog);
	}

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentcode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR OpenGl Fragment Shader: %s", infoLog);
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetShaderiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		printf("ERROR OpenGl Program Linking: %s", infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

ShaderGL::~ShaderGL() {
	glDeleteProgram(ID);
}

void ShaderGL::use() {
	glUseProgram(ID);
}

void ShaderGL::setBool(const std::string& name, bool value) {
	setInt(name, value ? 1 : 0);
}

void ShaderGL::setInt(const std::string& name, int value) {
	int loc = getLocation(name);
	if (loc != -1)
		glUniform1i(loc, value);
}

void ShaderGL::setFloat(const std::string& name, float value) {
	int loc = getLocation(name);
	if (loc != -1)
		glUniform1f(loc, value);
}

void ShaderGL::setVector2(const std::string& name, Vector2 value) {
	int loc = getLocation(name);
	if (loc != -1)
		glUniform2f(loc, value.X, value.Y);
}

void ShaderGL::setMatrix(const std::string& name, Matrix value) {
	int loc = getLocation(name);
	if (loc != -1)
		glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)&value);
}

int ShaderGL::getLocation(const std::string& name) {
	return glGetUniformLocation(ID, name.c_str());
}