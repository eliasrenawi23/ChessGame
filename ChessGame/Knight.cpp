#include "Knight.h"
#include "Window.h"

Knight::Knight(Box* loc, PlayerColor color)
{
	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BN.svg");
	}
	else {
		surface = IMG_Load("texture/WN.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}



std::vector<Box*> Knight::moveAndTake()
{
	return std::vector<Box*>();

}

Knight::~Knight()
{
}
