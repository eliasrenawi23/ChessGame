#include "Player.h"
#include "Box.h"
#include "Board.h"
#include <iostream>



Player::Player(PlayerColor color)
{
	this->color = color;
}

std::vector<Piece*> Player::getPieces()
{
	return this->Pieces;
}

void Player::init()
{
	int PawnYPostion = (color == PlayerColor::BLACK) ? 6 : 1;
	int restofPiecesYPostion = (color == PlayerColor::BLACK) ? 7 : 0;


	for (int x = 0; x < 8; x++) {
		Pieces.push_back(new Pawn(&(Board::gameboxess[x][PawnYPostion])));
		//std::cout << Board::gameboxess[PawnYPostion][2].x << " " << Board::gameboxess[PawnYPostion][2].y << std::endl;

	}
	/*Pieces.push_back(new Bishop(&(Board::gameboxess[restofPiecesYPostion][2])));
	Pieces.push_back(new Bishop(&(Board::gameboxess[restofPiecesYPostion][5])));

	Pieces.push_back(new Knight(&(Board::gameboxess[restofPiecesYPostion][1])));
	Pieces.push_back(new Knight(&(Board::gameboxess[restofPiecesYPostion][6])));

	Pieces.push_back(new Rook(&(Board::gameboxess[restofPiecesYPostion][0])));
	Pieces.push_back(new Rook(&(Board::gameboxess[restofPiecesYPostion][7])));

	Pieces.push_back(new Queen(&(Board::gameboxess[restofPiecesYPostion][3])));
	Pieces.push_back(new King(&(Board::gameboxess[restofPiecesYPostion][4])));*/
}

Player::~Player()
{

	for (auto p : Pieces)
	{
		delete p;
	}
	Pieces.clear();
}


