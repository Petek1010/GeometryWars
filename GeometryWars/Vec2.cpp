#include "Vec2.h"
#include <iostream>

Vec2::Vec2()
{
}

Vec2::Vec2(float xin, float yin) 
	: x(xin)
	, y(yin)
{
}

bool Vec2::operator==(const Vec2& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return (x != rhs.x && y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float val) const
{
	return Vec2(x * val, y * val);
}

Vec2 Vec2::operator/(const float val) const
{
	return Vec2(x / val, y/ val);
}

void Vec2::operator+=(const Vec2& rhs)
{
	x = x + rhs.x;
	y = y + rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	x = x - rhs.x;
	y = y - rhs.y;
}

void Vec2::operator*=(const float val)
{
	x = x * val;
	y = y * val;
}

void Vec2::operator/=(const float val)
{
	x = x / val;
	y = y / val;
}

float Vec2::length() const
{
	return (std::sqrt((x * x) + (y * y)));
}

void Vec2::normalize()
{
	float invLength = 1.0f / length();
	x *= invLength;
	y *= invLength;
}

float Vec2::distanceTo(const Vec2& rhs) const
{
	//TO DO (OPTIONAL)
	return 0.f;
}

