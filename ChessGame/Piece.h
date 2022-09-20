#pragma once
#include "Box.h"
class Piece
{

protected:

	Box *location;
public:

	Piece();
	virtual void moveAndTake() = 0;
	~Piece();

};

