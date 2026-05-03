#include "Main.h" 
#include "Food.h"

void Food::draw(Gdiplus::Graphics& g) {
	int x = position[0] * CELL_SIZE;
	int y = position[1] * CELL_SIZE;

	Gdiplus::Color foodColor(255, 243, 228, 201);
	Gdiplus::SolidBrush brushOfFood(foodColor);

	Gdiplus::Color foodBorderColor(255, 138, 95, 65);
	Gdiplus::Pen penOfFood(foodBorderColor, 2);

	g.FillEllipse(&brushOfFood, x, y, CELL_SIZE, CELL_SIZE);
	g.DrawEllipse(&penOfFood, x, y, CELL_SIZE, CELL_SIZE);
}
void Food::respon() {
	position[0] = randInt(0, CELL_COUNT - 1);
	position[1] = randInt(0, CELL_COUNT - 1);
}