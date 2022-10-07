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
#include <set>


class Player
{

private:
	PlayerColor color;
	std::vector<Piece*> Pieces;
	static std::vector<Box*> ThreatMap;
public:
	Player(PlayerColor color );
	std::vector<Piece*> getPieces();
	std::set<Box*> play(Piece* pieceToPlay);
	void init();
	~Player();

};

