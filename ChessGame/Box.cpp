#include "Box.h"
#include"Window.h"


Box::Box() {

}

Box::Box(int x, int y, int width, int higth, SDL_Color boxColor):x(x),y(y), width(width), higth(higth), boxColor(boxColor)
{
}

void Box::RenderBox()
{

	SDL_Rect highlightRect;
	highlightRect.w = width;
	highlightRect.h = width;
	SDL_SetRenderDrawColor(Window::m_renderer, boxColor.r, boxColor.g, boxColor.b, boxColor.a);

	highlightRect.x =x;
	highlightRect.y =y;
	SDL_RenderFillRect(Window::m_renderer, &highlightRect);
}



Box::~Box() {

}