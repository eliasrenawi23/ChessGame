#include "Queen.h"
#include "Window.h"



Queen::Queen(Box* loc, PlayerColor color)
{
	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BQ.svg");
	}
	else {
		surface = IMG_Load("texture/WQ.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}



std::vector<Box*> Queen::moveAndTake()
{
	return std::vector<Box*>();

}

Queen::~Queen()
{
}
