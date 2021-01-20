#include "pch.h"
#include "Vector2D.h"


Vector2D Vector2D::Normalize()
{
	Vector2D v;
	float l = this->Length();
	if (l != 0) {
		v.x = x / l;
		v.y = y / l;
	}
	const Vector2D v2(v.x, v.y);
	return v2;
}

float Vector2D::Magnitude()
{
	return std::sqrt((x*x) + (y*y));
}
