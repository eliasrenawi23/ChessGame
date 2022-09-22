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
	int PawnYPostion = (color == PlayerColor::BLACK) ? 1 : 6;
	int restofPiecesYPostion = (color == PlayerColor::BLACK) ? 0 : 7;


	for (int x = 0; x < 8; x++) {
		Pieces.push_back(new Pawn(&(Board::gameboxess[x][PawnYPostion]),color));
		//std::cout << Board::gameboxess[PawnYPostion][2].x << " " << Board::gameboxess[PawnYPostion][2].y << std::endl;

	}
	Pieces.push_back(new Bishop(&(Board::gameboxess[2][restofPiecesYPostion]), color));
	Pieces.push_back(new Bishop(&(Board::gameboxess[5][restofPiecesYPostion]), color));

	Pieces.push_back(new Knight(&(Board::gameboxess[1][restofPiecesYPostion]), color));
	Pieces.push_back(new Knight(&(Board::gameboxess[6][restofPiecesYPostion]), color));

	Pieces.push_back(new Rook(&(Board::gameboxess[0][restofPiecesYPostion]), color));
	Pieces.push_back(new Rook(&(Board::gameboxess[7][restofPiecesYPostion]), color));

	Pieces.push_back(new Queen(&(Board::gameboxess[3][restofPiecesYPostion]), color));
	Pieces.push_back(new King(&(Board::gameboxess[4][restofPiecesYPostion]), color));
}

Player::~Player()
{

	for (auto p : Pieces)
	{
		delete p;
	}
	Pieces.clear();
}


