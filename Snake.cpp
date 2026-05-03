
#include "Snake.h"
#include "Food.h"

Snake::Snake() {
	body.push_back({ 0, 0 });
	body.push_back({ 1, 0 });
	body.push_back({ 2, 0 });
}

void Snake::draw(Gdiplus::Graphics& g) {
	Gdiplus::SolidBrush snakeBodyDarkBrush(Gdiplus::Color(255, 84, 107, 65));
	Gdiplus::SolidBrush snakeBodyLightBrush(Gdiplus::Color(255, 153, 173, 122));
	Gdiplus::Pen outlinePen(Gdiplus::Color(255, 84, 107, 65), 2);

	for (int i = 0;i < body.size();i++) {
		int padd = i == body.size() - 1 ? 0 : 2;

		Gdiplus::GraphicsPath path;
		CreateRundedRect(path, Snake::body[i].x * CELL_SIZE + padd, Snake::body[i].y * CELL_SIZE + padd, CELL_SIZE - (2 * padd), CELL_SIZE - (2 * padd), 8);
		if (i == body.size() - 1) {
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

	BodySegment newHead = body.back();

	if (dir == RIGHT) {
		newHead.x++;
	}
	if (dir == LEFT) {
		newHead.x--;
	}
	if (dir == UP) {
		newHead.y--;
	}
	if (dir == DOWN) {
		newHead.y++;
	}


	if (newHead.x >= CELL_COUNT) {
		newHead.x = 0;
	}
	if (newHead.x < 0) {
		newHead.x = CELL_COUNT - 1;
	}
	if (newHead.y >= CELL_COUNT) {
		newHead.y = 0;
	}
	if (newHead.y < 0) {
		newHead.y = CELL_COUNT - 1;
	}

	body.push_back(newHead); //add head

	body.erase(body.begin()); //remove tail
}

void Snake::grow() {
	BodySegment trail = body.front();

	body.insert(body.begin(), trail);
}

BOOL Snake::isEatingFood(Food& f) {
	if (f.position[0] == body.back().x && f.position[1] == body.back().y) {
		return TRUE;
	}
	return FALSE;
}

BOOL Snake::isColideWithOwnBody() {
	BodySegment head = body.back();
	for (int i = 0;i < body.size()-2;i++) {
		if (body[i].x == head.x && body[i].y == head.y) {
			return TRUE;
		}
	}
	return FALSE;
}