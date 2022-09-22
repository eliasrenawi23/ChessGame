#include "Rook.h"
#include "Window.h"



Rook::Rook(Box* loc, PlayerColor color)
{
	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BR.svg");
	}
	else {
		surface = IMG_Load("texture/WR.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}


void Rook::moveAndTake()
{
}


Rook::~Rook()
{
}
