#include "Box.h"
#include"Window.h"
#include "Piece.h"



Box::Box()  {

}

Box::Box(int x, int y, int size, SDL_Color originalColor)
	:x(x), y(y), size(size), originalColor(originalColor), piece(NULL) {
	boxColor = originalColor;
}
void Box::setPiece(Piece* p) {
	piece = p;
	//piece->location = p->location;
}

Piece* Box::getPiece()
{
	return piece;
}

void Box::resize(int x, int y, int size)
{
	this->x = x;
	this->y = y;
	this->size = size;
}
Box::~Box() {


}