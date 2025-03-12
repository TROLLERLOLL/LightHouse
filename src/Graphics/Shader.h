#pragma once
#include <string>
#include <Utils/Math.h>
#include <Graphics/SpriteShader.h>

class Shader {
	public:
		virtual void use() = 0;
		virtual void setBool(const std::string& name, bool value) = 0;
		virtual void setInt(const std::string& name, int value) = 0;
		virtual void setFloat(const std::string& name, float value) = 0;
		virtual void setVector2(const std::string& name, Vector2 value) = 0;
		virtual void setMatrix(const std::string& name, Matrix value) = 0;
};