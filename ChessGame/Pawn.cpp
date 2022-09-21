#include "Pawn.h"
#include <SDL_image.h>
#include "Window.h"



SDL_Texture* Pawn::whitePawnTexture=NULL;
Pawn::Pawn(Box* loc)
{
	location = loc;
	SDL_Surface* surface;

	surface = IMG_Load("texture/pawnPic.png");
	whitePawnTexture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}
void Pawn::moveAndTake()
{

}
void Pawn::render()
{
	renderPiece(whitePawnTexture);

}

Pawn::~Pawn()
{

}
