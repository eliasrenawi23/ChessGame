#pragma once
#include "Box.h"
class Piece
{

protected:

	Box *location;
public:
	Piece();
	virtual void moveAndTake() = 0;
	virtual void render() = 0;
	~Piece();
protected:
	void renderPiece(SDL_Texture* texture);
};

