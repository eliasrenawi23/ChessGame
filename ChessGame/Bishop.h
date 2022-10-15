#pragma once
#include "Piece.h"
class Bishop :
	public Piece
{


public:
	Bishop(Box* loc, PlayerColor color);
	std::set<Box*> moveAndTake() override;
	std::set<Box*>PieceThreatMap(bool* checkmate)override;
	~Bishop();

};

