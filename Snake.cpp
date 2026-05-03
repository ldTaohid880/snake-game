#include "Food.h"
#include "Snake.h"
#include "Main.h"
#include "helper.h"

void Snake::draw(Gdiplus::Graphics& g) {
	Gdiplus::SolidBrush snakeBodyDarkBrush(Gdiplus::Color(255, 84, 107, 65));
	Gdiplus::SolidBrush snakeBodyLightBrush(Gdiplus::Color(255, 153, 173, 122));
	Gdiplus::Pen outlinePen(Gdiplus::Color(255, 84, 107, 65), 2);

	for (int i = 0;i < 3;i++) {
		int padd = i == currentSize - 1 ? 0 : 2;

		Gdiplus::GraphicsPath path;
		CreateRundedRect(path, Snake::body[i][0] * CELL_SIZE + padd, Snake::body[i][1] * CELL_SIZE + padd, CELL_SIZE - (2 * padd), CELL_SIZE - (2 * padd), 8);
		if (i == currentSize - 1) {
			//head segment
			g.FillPath(&snakeBodyDarkBrush, &path);
		}
		else {
			//body segment
			//fill body
			g.FillPath(&snakeBodyLightBrush, &path);

			//draw a slight outline
			g.DrawPath(&outlinePen, &path);
		}
	}
}

void Snake::move() {

	for (int i = 0;i < currentSize-1;i++) {
		body[i][0] = body[i + 1][0];
		body[i][1] = body[i + 1][1];
	};

	if (dir == RIGHT) {
		body[currentSize-1][0]++;
	}
	if (dir == LEFT) {
		body[currentSize-1][0]--;
	}
	if (dir == UP) {
		body[currentSize-1][1]--;
	}
	if (dir == DOWN) {
		body[currentSize-1][1]++;
	}


	if (body[currentSize -1 ][0] > 19) {
		body[currentSize -1 ][0] = 0;
	}
	if (body[currentSize -1 ][0] < 0) {
		body[currentSize -1 ][0] = 19;
	}
	if (body[currentSize -1 ][1] > 19) {
		body[currentSize -1 ][1] = 0;
	}		 
	if (body[currentSize -1 ][1] < 0) {
		body[currentSize -1 ][1] = 19;
	}

}

BOOL Snake::isEatingFood(Food f) {
	if (f.position[0] == body[currentSize-1][0] && f.position[1] == body[currentSize - 1][1]) {
		//TODO:increament point
		//update the food position
		return TRUE;
	}
	return FALSE;
}