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
		Pieces.push_back(new Pawn(&(Board::gameboxess[x][PawnYPostion]), color));
		//Piece *p = (Pieces.back());
		//std::cout << p->location->x << " " << p->location->y << std::endl;
		std::cout << Pieces.back()->location->x << " " << Pieces.back()->location->y << std::endl;

		//Board::gameboxess[x][restofPiecesYPostion].setPiece(p);

		//std::cout << (Board::gameboxess[x][PawnYPostion].x) << " " << (Board::gameboxess[x][PawnYPostion].y) << std::endl;

	}
	Pieces.push_back(new Bishop(&(Board::gameboxess[2][restofPiecesYPostion]), color));    ///box <---> piece point to each other 
	Board::gameboxess[2][restofPiecesYPostion].piece = Pieces[Pieces.size() - 1];
	Pieces.push_back(new Bishop(&(Board::gameboxess[5][restofPiecesYPostion]), color));
	Board::gameboxess[5][restofPiecesYPostion].piece = Pieces[Pieces.size() - 1];

	Pieces.push_back(new Knight(&(Board::gameboxess[1][restofPiecesYPostion]), color));
	Board::gameboxess[1][restofPiecesYPostion].piece = Pieces[Pieces.size() - 1];
	Pieces.push_back(new Knight(&(Board::gameboxess[6][restofPiecesYPostion]), color));
	Board::gameboxess[6][restofPiecesYPostion].piece = Pieces[Pieces.size() - 1];

	Pieces.push_back(new Rook(&(Board::gameboxess[0][restofPiecesYPostion]), color));
	Board::gameboxess[0][restofPiecesYPostion].piece = Pieces[Pieces.size() - 1];
	Pieces.push_back(new Rook(&(Board::gameboxess[7][restofPiecesYPostion]), color));
	Board::gameboxess[7][restofPiecesYPostion].piece = Pieces[Pieces.size() - 1];


	Pieces.push_back(new Queen(&(Board::gameboxess[3][restofPiecesYPostion]), color));
	Board::gameboxess[3][restofPiecesYPostion].piece = Pieces[Pieces.size() - 1];
	Pieces.push_back(new King(&(Board::gameboxess[4][restofPiecesYPostion]), color));
	Board::gameboxess[4][restofPiecesYPostion].piece = Pieces[Pieces.size() - 1];




}

Player::~Player()
{

	for (auto p : Pieces)
	{
		delete p;
	}
	Pieces.clear();
}


