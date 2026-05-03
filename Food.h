#pragma once
#include "Main.h"
#include "helper.h"

class Snake;

class Food
{

public:
	int position[2];
	Food(Snake &s);
	void draw(Gdiplus::Graphics& g);
	void respon(Snake &s);
};