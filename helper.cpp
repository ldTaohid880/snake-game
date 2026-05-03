#include <random>
#include "Main.h"

int randInt(int min, int max) {
	// 1. Seed with a real random value from hardware if available
	std::random_device rd;

	//2. Choose a generator engine (Mersenne Twister is standard)
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> distr(min, max);

	return distr(gen);
};


void CreateRundedRect( Gdiplus::GraphicsPath &path,int x, int y, int width, int height, int radius) {

	//reset the path first;
	path.Reset();

	int diameter = radius * 2;

	//corners
	path.AddArc(x + width - diameter, y + height - diameter, diameter, diameter, 0, 90);
	path.AddArc(x, y + height - diameter, diameter, diameter, 90, 90);
	path.AddArc(x, y, diameter, diameter, 180, 90);
	path.AddArc(x + width - diameter, y, diameter, diameter, 270, 90);

	path.CloseFigure();
}
