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
	std::vector<Piece*> PiecesOptions;

public:
	Player(PlayerColor color );
	std::vector<Piece*> getPieces();
	std::set<Box*> play( Piece * const pieceToPlay, bool* checkmate);
	std::set<Box*> ClacThreatMap(bool* checkmate);
	void updateVectorPieces(Piece* p);
	void setopponentThreatMap(std::set<Box*> opponentThreatMap);
	void init();
	void handle_promotion(bool promotionmenu);
	void dopromotion(Piece* selsectedPiece, int cor_x, int cor_y);
	std::vector<Piece*> getPiecesOptions();
	~Player();
	King* getKing();

//private :
//	King* getKing();

};

