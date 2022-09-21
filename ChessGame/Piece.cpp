#include "Piece.h"
#include "Window.h"

Piece::Piece() :location(NULL)/*, surface(NULL)*/
{
}


void Piece::renderPiece(SDL_Texture* texture)
{
	int w;
	SDL_QueryTexture(texture, NULL, NULL, &w, &w);
	SDL_Rect fromRect, toRect;
	fromRect.w = w;
	fromRect.h = w;
	fromRect.x = fromRect.y = 0;

	toRect.w = location->size * 0.8;
	toRect.h = location->size * 0.8;

	toRect.x = location->x;
	toRect.y = location->y;
	SDL_RenderCopy(Window::m_renderer, texture, &fromRect, &toRect);

}

Piece::~Piece()
{

}
