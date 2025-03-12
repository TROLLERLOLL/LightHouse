#pragma once

struct Vector2 {
	float X;
	float Y;

	Vector2(float X = 0, float Y = 0) : X(X), Y(Y) {}

	inline Vector2 operator-(Vector2 vec) {
		return Vector2(X - vec.X, Y - vec.Y);
	}

	inline Vector2 operator+(Vector2 vec) {
		return Vector2(X + vec.X, Y + vec.Y);
	}

	inline void operator+=(Vector2 vec) {
		X += vec.X; Y += vec.Y;
	}

	inline void operator-=(Vector2 vec) {
		X -= vec.X; Y -= vec.Y;
	}

	inline void operator*=(Vector2 vec) {
		X *= vec.X; Y *= vec.Y;
	}

	inline void operator/=(Vector2 vec) {
		X /= vec.X; Y /= vec.Y;
	}
};

struct Vector4 {
	float X;
	float Y;
	float Z;
	float W;

	Vector4(float X = 0, float Y = 0, float Z = 0, float W = 0) : X(X), Y(Y), Z(Z), W(W) {}
};

struct Point {
	int X;
	int Y;

	Point(int X = 0, int Y = 0) : X(X), Y(Y) {}
};

struct Rect {
	Point Location;
	Point Size;

	Rect(Point Location, Point Size) : Location(Location), Size(Size) {}
	Rect(int X, int Y, int W, int H) : Location(Point(X, Y)), Size(Point(W, H)) {}
	Rect() {}

	inline bool intersects(Rect rect) {
		return (Location.X < rect.Location.X + rect.Size.X && Location.X + Size.X > rect.Location.X && Location.Y < rect.Location.Y + rect.Size.Y && Location.Y + Size.Y > rect.Location.Y);
	}
};


struct Matrix {
	float M11 = 0;
	float M12 = 0;
	float M13 = 0;
	float M14 = 0;

	float M21 = 0;
	float M22 = 0;
	float M23 = 0;
	float M24 = 0;

	float M31 = 0;
	float M32 = 0;
	float M33 = 0;
	float M34 = 0;

	float M41 = 0;
	float M42 = 0;
	float M43 = 0;
	float M44 = 0;

	Matrix() {
		M11 = 1;
		M22 = 1;
		M33 = 1;
		M44 = 1;
	}

	static Matrix Identity() {
		Matrix mat;
		mat.M11 = 1;
		mat.M22 = 1;
		mat.M33 = 1;
		mat.M44 = 1;
		return mat;
	}

	static Matrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
		Matrix mat;
		mat.M11 = (float)(2.0f / ((double)right - (double)left));
		mat.M22 = (float)(2.0f / ((double)top - (double)bottom));
		mat.M33 = (float)(1.0f / ((double)nearPlane - (double)farPlane));
		mat.M41 = (float)(((double)left + (double)right) / ((double)left - (double)right));
		mat.M42 = (float)(((double)top + (double)bottom) / ((double)bottom - (double)top));
		mat.M43 = (float)((double)nearPlane / ((double)nearPlane - (double)farPlane));
		mat.M44 = 1.0f;

		return mat;
	}

	static Matrix CreateOrthographic(float width, float height, float nearPlane, float farPlane) {
		Matrix mat = Identity();
		mat.M11 = 2.0f / width;
		mat.M22 = 2.0f / height;
		mat.M33 = 1.0f / (nearPlane - farPlane);
		mat.M43 = nearPlane / (nearPlane - farPlane);
		mat.M44 = 1.0f;
		return mat;
	}

	static Matrix CreateTranslation(float X, float Y) {
		Matrix mat = Identity();
		mat.M41 = X;
		mat.M42 = Y;

		return mat;
	}

	static Matrix CreateScale(float X, float Y) {
		Matrix mat = Identity();
		mat.M11 = X;
		mat.M22 = Y;
		return mat;
	}

	inline Matrix operator*(Matrix matrix) {
		Matrix mat;
		mat.M11 = M11 * matrix.M11 + M12 * matrix.M21 + M13 * matrix.M31 + M14 * matrix.M41;
		mat.M12 = M11 * matrix.M12 + M12 * matrix.M22 + M13 * matrix.M32 + M14 * matrix.M42;
		mat.M13 = M11 * matrix.M13 + M12 * matrix.M23 + M13 * matrix.M33 + M14 * matrix.M43;
		mat.M14 = M11 * matrix.M14 + M12 * matrix.M24 + M13 * matrix.M34 + M14 * matrix.M44;

		mat.M21 = M21 * matrix.M11 + M22 * matrix.M21 + M23 * matrix.M31 + M24 * matrix.M41;
		mat.M22 = M21 * matrix.M12 + M22 * matrix.M22 + M23 * matrix.M32 + M24 * matrix.M42;
		mat.M23 = M21 * matrix.M13 + M22 * matrix.M23 + M23 * matrix.M33 + M24 * matrix.M43;
		mat.M24 = M21 * matrix.M14 + M22 * matrix.M24 + M23 * matrix.M34 + M24 * matrix.M44;

		mat.M31 = M31 * matrix.M11 + M32 * matrix.M21 + M33 * matrix.M31 + M34 * matrix.M41;
		mat.M32 = M31 * matrix.M12 + M32 * matrix.M22 + M33 * matrix.M32 + M34 * matrix.M42;
		mat.M33 = M31 * matrix.M13 + M32 * matrix.M23 + M33 * matrix.M33 + M34 * matrix.M43;
		mat.M34 = M31 * matrix.M14 + M32 * matrix.M24 + M33 * matrix.M34 + M34 * matrix.M44;

		mat.M41 = M41 * matrix.M11 + M42 * matrix.M21 + M43 * matrix.M31 + M44 * matrix.M41;
		mat.M42 = M41 * matrix.M12 + M42 * matrix.M22 + M43 * matrix.M32 + M44 * matrix.M42;
		mat.M43 = M41 * matrix.M13 + M42 * matrix.M23 + M43 * matrix.M33 + M44 * matrix.M43;
		mat.M44 = M41 * matrix.M14 + M42 * matrix.M24 + M43 * matrix.M34 + M44 * matrix.M44;

		return mat;
	}

	inline Vector4 operator*(Vector4 matrix) {
		float X = M11 * matrix.X + M12 * matrix.Y + M13 * matrix.Z + M14 * matrix.W;
		float Y = M21 * matrix.X + M22 * matrix.Y + M23 * matrix.Z + M24 * matrix.W;
		float Z = M31 * matrix.X + M32 * matrix.Y + M33 * matrix.Z + M34 * matrix.W;
		float W = M41 * matrix.X + M42 * matrix.Y + M43 * matrix.Z + M44 * matrix.W;

		return Vector4(X, Y, Z, W);
	}
};