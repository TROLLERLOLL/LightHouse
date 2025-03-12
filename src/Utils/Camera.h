#pragma once

#include <Utils/Math.h>

class Camera {
	private:
		Matrix matrix;

	public:
		Vector2 Position;
		float Zoom = 1.0f;
		void Update(int Width, int Height);
		Matrix getMatrix();
};