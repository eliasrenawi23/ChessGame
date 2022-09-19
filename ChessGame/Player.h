#pragma once
#include "PlayerColor.h"
#include "Piece.h"
#include "Pawn.h"
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Queen.h"
#include "Rook.h"

#include <vector>

class Player
{

private:
	PlayerColor color;
	std::vector<Piece*> Pieces;

public:
	Player() {};
	Player(PlayerColor color );
	void init();
	~Player();

};

