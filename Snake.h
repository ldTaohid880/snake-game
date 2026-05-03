#pragma once
#include "Main.h"
#include "helper.h"

class Food;

struct BodySegment {
	int x;
	int y;
};

class Snake
{
public:

	std::vector<BodySegment> body;
	enum MovingDir {
		RIGHT,
		DOWN,
		LEFT,
		UP
	};
	Snake();
	MovingDir dir = RIGHT;
	void draw(Gdiplus::Graphics& g);
	void move();
	void grow();
	BOOL isEatingFood(Food &f);
	BOOL isColideWithOwnBody();
};

