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
	std::set<Box*> opponentThreatMap;
public:
	Player(PlayerColor color );
	std::vector<Piece*> getPieces();
	std::set<Box*> play(Piece* pieceToPlay);
	std::set<Box*> ClacThreatMap();
	void updateVectorPieces(Piece* p);
	void setopponentThreatMap(std::set<Box*> opponentThreatMap);
	void init();
	~Player();

};

