#include "pch.h"
#include "Rectangle.h"

Rectangle::Rectangle()
{
}

Rectangle::~Rectangle()
{
}

bool Rectangle::IsFullyContainedIn(const Rectangle & other) const
{
		if (right < other.right && left > other.left && top > other.top && bottom < other.bottom)
			return true;
		else
			return false;
}

bool Rectangle::IsFullyOutsideOf(const Rectangle & other) const
{
	if (right < other.left || left > other.right || top > other.bottom || bottom < other.top)
		return true;
	else
		return false;
	
}

void Rectangle::ClipTo(const Rectangle & other)
{
	left = std::max(left, other.left); //left becomes the higher value of left and other.left
	right = std::min(right, other.right); //right becomes the lower value of right and other.right
	bottom = std::min(bottom, other.bottom);
	top = std::max(top, other.top);
}

void Rectangle::Translate(int posX, int posY)
{
	left += posX;
	right += posX;
	top += posY;
	bottom += posY;
}
