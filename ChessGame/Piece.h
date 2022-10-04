#pragma once
#include "Box.h"
#include <vector>
#include "PlayerColor.h"
#include <SDL_image.h>
#include <iostream>



class Piece
{

public:

	Box *location;
	PlayerColor color;
	SDL_Texture* texture;
public:
	Piece();
	Box* getLocation();
	void setLocation(Box* loc);
	virtual std::vector<Box*> moveAndTake() = 0;
	void renderPiece();
	virtual ~Piece() ;
};



