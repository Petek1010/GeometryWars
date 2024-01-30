#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransfrom
{
public:

	Vec2 pos = { 0.0,0.0 };
	Vec2 velocity = { 0.0,0.0 };
	float angle = 0;

	CTransfrom(Vec2 p, Vec2 v, float a) :
		pos(p),
		velocity(v),
		angle(a)
	{

	}
};

class CShape
{
public:
	sf::CircleShape shape;

	CShape(float radius, int points, const sf::Color & fill, const sf::Color & outline, float thickness) :
		shape(radius, points)
	{
		shape.setFillColor(fill);
		shape.setOrigin(radius, radius);
		shape.setOutlineThickness(thickness);
		shape.setOutlineColor(outline);
	}

};

class CCollision
{
public:
	float radius = 0.f;
	CCollision(float r) : radius(r){}
};

class CInput
{
public:

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;

	CInput() {}

};

class CScore
{
public:
	int score = 0;
	CScore(int s) : score(s) {}

};

class CLifespan
{
public:
	int remaining = 0;
	int total = 0;
	CLifespan(int rem, int tot) : remaining(rem), total(tot) {}

};