#include "King.h"
#include "Window.h"



King::King(Box* loc ,PlayerColor color)
{
	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BK.svg");
	}
	else {
		surface = IMG_Load("texture/WK.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}



std::vector<Box*> King::moveAndTake()
{
	return std::vector<Box*>();

}


King::~King()
{
}
