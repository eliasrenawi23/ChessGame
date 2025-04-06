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
void Box::RenderBox()
{
	SDL_FRect highlightRect;
	highlightRect.w = size;
	highlightRect.h = size;
	SDL_SetRenderDrawColor(Window::m_renderer,boxColor.r, boxColor.g,boxColor.b,boxColor.a);
	highlightRect.x = x;
	highlightRect.y = y;
	SDL_RenderFillRect(Window::m_renderer, &highlightRect);
}
Box::~Box() {


}