#pragma once
class Rectangle
{
public:
	Rectangle();
	Rectangle(int l, int r, int t, int b) : left(l), right(r), top(t), bottom(b) {}
	Rectangle(int w, int h) : left(0), right(w), top(0), bottom(h) {}
	~Rectangle();

	bool IsFullyContainedIn(const Rectangle &other) const;
	bool IsFullyOutsideOf(const Rectangle &other) const;
	void ClipTo(const Rectangle &other);
	void Translate(int posX, int posY);

	int GetWidth() const { return right - left; }
	int GetHeight() const { return bottom - top; }
	int GetCentreWidth() const { return (right - left) / 2; }
	int GetCentreHeight() const { return (bottom - top) / 2; }
	Rectangle GetCentre() const { Rectangle r(GetWidth() - GetCentreWidth(), GetHeight() - GetCentreHeight() );  return r; }

	int left{ 0 }, right{ 0 }, top{ 0 }, bottom{ 0 };
};

