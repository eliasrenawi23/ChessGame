#include "Box.h"
#include"Window.h"


Box::Box() {

}

Box::Box(int x, int y, int size, SDL_Color boxColor):x(x),y(y), size(size), boxColor(boxColor)
{
}

void Box::resize(int x, int y, int size)
{
	this-> x=x;
	this->y=y;
	this->size=size;
}

void Box::RenderBox()
{
	SDL_SetRenderDrawColor(Window::m_renderer, 128,128, 128, 255);
	SDL_Rect highlightRect;
	highlightRect.w = size;
	highlightRect.h = size;
	SDL_SetRenderDrawColor(Window::m_renderer, boxColor.r, boxColor.g, boxColor.b, boxColor.a);
	highlightRect.x =x;
	highlightRect.y =y;
	SDL_RenderFillRect(Window::m_renderer, &highlightRect);
}



Box::~Box() {

}