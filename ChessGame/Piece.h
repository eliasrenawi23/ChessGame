#pragma once
#include "Box.h"
#include "PlayerColor.h"
#include <SDL_image.h>



class Piece
{

protected:

	Box *location;
	PlayerColor color;
	SDL_Texture* texture;

public:
	Piece();
	virtual void moveAndTake() = 0;
	void renderPiece();
	~Piece();
};

