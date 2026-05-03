#pragma once
#include "Main.h"
class Snake
{
public:
	int currentSize = 3;
	int body[3][2] = { {0,0},{1,0},{2,0} };
	enum MovingDir {
		RIGHT,
		DOWN,
		LEFT,
		UP
	};
	MovingDir dir = RIGHT;
	void draw(Gdiplus::Graphics& g);
	void move();
	BOOL isEatingFood(Food f);
};

