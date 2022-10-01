#include "Pawn.h"
#include "Window.h"


Pawn::Pawn(Box* loc, PlayerColor color)
{
	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BP.svg");
	}
	else {
		surface = IMG_Load("texture/WP.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);

}
void Pawn::moveAndTake()
{

}

Pawn::~Pawn()
{

}
