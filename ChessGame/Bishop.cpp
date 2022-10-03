#include "Bishop.h"
#include "Window.h"


Bishop::Bishop(Box* loc, PlayerColor color)
{

	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BB.svg");
	}
	else {
		surface = IMG_Load("texture/WB.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}


std::vector<Box*> Bishop::moveAndTake()
{
	return std::vector<Box*>();

}


Bishop::~Bishop()
{
}
