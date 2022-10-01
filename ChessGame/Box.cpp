#include "Box.h"
#include"Window.h"
#include "Piece.h"



Box::Box()  {

}

Box::Box(int x, int y, int size, SDL_Color boxColor)
	:x(x), y(y), size(size), boxColor(boxColor), piece(NULL) {
}
void Box::setPiece(Piece* p) {
	piece = p;
}

void Box::resize(int x, int y, int size)
{
	this->x = x;
	this->y = y;
	this->size = size;
}
Box::~Box() {


}