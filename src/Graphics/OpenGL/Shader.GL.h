#pragma once

#include <Graphics/Shader.h>

class ShaderGL : public Shader {
	private:
		unsigned int ID;
		int getLocation(const std::string& name);
	public:
		~ShaderGL();
		ShaderGL(const char* fragmentcode, const char* vertexcode);
		void use() override;
		void setBool(const std::string& name, bool value) override;
		void setInt(const std::string& name, int value) override;
		void setFloat(const std::string& name, float value) override;
		void setVector2(const std::string& name, Vector2 value) override;
		void setMatrix(const std::string& name, Matrix value) override;
};