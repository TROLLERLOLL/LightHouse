#include <Utils/Camera.h>

void Camera::Update(int Width, int Height) {
	matrix = Matrix::CreateTranslation(-Position.X, -Position.Y) * Matrix::CreateScale(Zoom, Zoom) * Matrix::CreateTranslation(Width / 2, Height / 2);
}

Matrix Camera::getMatrix() {
	return matrix;
}