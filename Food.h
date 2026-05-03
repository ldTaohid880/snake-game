#include "Main.h"
#include "helper.h"
#pragma once

class Food
{

public:
	int position[2];
	Food() {
		position[0] = randInt(0, CELL_COUNT-1);
		position[1] = randInt(0, CELL_COUNT-1);
	}
	void draw(Gdiplus::Graphics& g);
	void respon();
};

