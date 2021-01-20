#pragma once
using namespace std;

class Vector2D
{
public:
	//Constructors
	Vector2D() = default;
	Vector2D(float xPos, float yPos) : x(xPos), y(yPos) {}
	Vector2D(const Vector2D& other) { x = other.x; y = other.y; }
	~Vector2D() {}

	//Variables
	float x{ 0 };
	float y{ 0 };

	//Functions
	float Length() { return (float)sqrt(x*x + y * y); }
	Vector2D Normalize();
	float Magnitude();
	
	//Operator Overloading
	Vector2D& operator+=(const Vector2D& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vector2D& operator-=(const Vector2D& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
};

//Operator Overloading
//Addition. Vector and float
inline Vector2D operator+(Vector2D lhs, const Vector2D& rhs) {

	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

inline Vector2D operator+(Vector2D lhs, const float rhs) {

	lhs.x += rhs;
	lhs.y += rhs;
	return lhs;
}

//Substraction. Vector and Vector
inline Vector2D operator-(Vector2D lhs, const Vector2D& rhs) {

	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

//Multiplication. Vector and float
inline Vector2D operator*(float lhs, Vector2D rhs) {

	rhs.x = rhs.x * lhs;
	rhs.y = rhs.y * lhs;
	return rhs;
}

inline Vector2D operator*(Vector2D lhs, float rhs) {

	lhs.x = lhs.x * rhs;
	lhs.y = lhs.y * rhs;
	return lhs;
}

//Division. Float.
inline Vector2D operator/(Vector2D lhs, float rhs) {

	lhs.x = lhs.x / rhs;
	lhs.y = lhs.y / rhs;
	return lhs;
}

inline ostream& operator<< (std::ostream& os, const Vector2D& rhs) {

	os << "X: " << rhs.x << ", Y: " << rhs.y << "." << endl;
	return os;
}

//Comparisons
inline bool operator<(Vector2D lhs, Vector2D rhs) {
	if (lhs.x < rhs.x)
		if (lhs.y < rhs.y)
			return true;

	return false;
}

inline bool operator>(Vector2D lhs, Vector2D rhs) {
	if (lhs.x > rhs.x)
		if (lhs.y > rhs.y)
			return true;

	return false;
}

inline bool operator<=(Vector2D lhs, Vector2D rhs) {
	if (lhs.x <= rhs.x)
		if (lhs.y <= rhs.y)
			return true;

	return false;
}

inline bool operator>=(Vector2D lhs, Vector2D rhs) {
	if (lhs.x >= rhs.x)
		if (lhs.y >= rhs.y)
			return true;

	return false;
}