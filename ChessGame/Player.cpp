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

std::set<Box*>  Player::play(Piece* pieceToPlay)
{
	//pieceToPlay->location = &Board::gameboxess[4][4];
	return pieceToPlay->moveAndTake();
}



void Player::init()
{
	int PawnYPostion = (color == PlayerColor::BLACK) ? 1 : 6;
	int restofPiecesYPostion = (color == PlayerColor::BLACK) ? 0 : 7;


	for (int x = 0; x < 8; x++) {
		Pieces.push_back(new Pawn(&(Board::gameboxess[x][PawnYPostion]), color));
		Board::gameboxess[x][PawnYPostion].setPiece(Pieces.back());
	}
	
	Pieces.push_back(new Bishop(&(Board::gameboxess[2][restofPiecesYPostion]), color));    //Bishop1 ///box <---> piece point to each other 
	Board::gameboxess[2][restofPiecesYPostion].setPiece(Pieces.back());
	Pieces.push_back(new Bishop(&(Board::gameboxess[5][restofPiecesYPostion]), color)); //Bishop2
	Board::gameboxess[5][restofPiecesYPostion].setPiece(Pieces.back());

	Pieces.push_back(new Knight(&(Board::gameboxess[1][restofPiecesYPostion]), color));//Knight1
	Board::gameboxess[1][restofPiecesYPostion].setPiece(Pieces.back());
	Pieces.push_back(new Knight(&(Board::gameboxess[6][restofPiecesYPostion]), color));//Knight2
	Board::gameboxess[6][restofPiecesYPostion].setPiece(Pieces.back());

	Pieces.push_back(new Rook(&(Board::gameboxess[0][restofPiecesYPostion]), color));//Rook1
	Board::gameboxess[0][restofPiecesYPostion].setPiece(Pieces.back());
	Pieces.push_back(new Rook(&(Board::gameboxess[7][restofPiecesYPostion]), color));//Rook2
	Board::gameboxess[7][restofPiecesYPostion].setPiece(Pieces.back());


	Pieces.push_back(new Queen(&(Board::gameboxess[3][restofPiecesYPostion]), color));//Queen
	Board::gameboxess[3][restofPiecesYPostion].setPiece(Pieces.back());
	Pieces.push_back(new King(&(Board::gameboxess[4][restofPiecesYPostion]), color));//King
	Board::gameboxess[4][restofPiecesYPostion].setPiece(Pieces.back());
	



}

Player::~Player()
{

	for (auto p : Pieces)
	{
		delete p;
	}
	Pieces.clear();
}


