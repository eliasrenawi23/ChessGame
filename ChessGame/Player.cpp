#include "Player.h"


Player::Player(PlayerColor color )
{
	this->color = color;
}

void Player::init()
{
	int PawnYPostion = (color == PlayerColor::BLACK) ? 6 : 1;
	int restofPiecesYPostion = (color == PlayerColor::BLACK) ? 7 : 0;



	for (int x = 0; x < 8; x++) {
		Pieces.push_back(new Pawn(x, PawnYPostion));
	}

	Pieces.push_back(new  Bishop(2, restofPiecesYPostion));
	Pieces.push_back(new Bishop(5, restofPiecesYPostion));


	Pieces.push_back(new  Knight(1, restofPiecesYPostion));
	Pieces.push_back(new Knight(6, restofPiecesYPostion));

	Pieces.push_back(new  Rook(0, restofPiecesYPostion));
	Pieces.push_back(new Rook(7, restofPiecesYPostion));


	Pieces.push_back(new  Queen(3, restofPiecesYPostion));
	Pieces.push_back(new King(4, restofPiecesYPostion));
}

Player::~Player()
{

	for (auto p : Pieces)
	{
		delete p;
	}
	Pieces.clear();
}


