#pragma once
#include "Piece.h"
class Bishop :
	public Piece
{


public:
	Bishop(Box* loc, PlayerColor color);
	std::set<Box*> moveAndTake() override;
	~Bishop();
};

