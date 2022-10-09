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

	std::set<Box*> legalmoves = pieceToPlay->moveAndTake();

	int s = Board::BoxWidthandHigth;

	if (dynamic_cast<King*>(pieceToPlay) != nullptr) {
		std::set<Box*>::iterator itr2;
		for (itr2 = legalmoves.begin(); itr2 != legalmoves.end(); itr2++) {
			std::cout << "legalmoves (*itr2) " << (*itr2)->x / s << " " << (*itr2)->y / s << std::endl;
		}

		std::cout << "player.cpp king must remove locations " << std::endl;
		std::set<Box*>::iterator itr;
		for (itr = opponentThreatMap.begin(); itr != opponentThreatMap.end(); itr++) {
			std::cout << "opponentThreatMap (*itr) " << (*itr)->x / s << " " << (*itr)->y / s << std::endl;

			legalmoves.erase((*itr));
		}


	}
	return legalmoves;
}

std::set<Box*> Player::ClacThreatMap()
{
	std::set<Box*> ThreatMap;
	int s = Board::BoxWidthandHigth;

	for (int i = 0; i < Pieces.size(); i++) {
		if (Pieces[i] != nullptr) {
			std::set<Box*> PieceIThreatMap = Pieces[i]->PieceThreatMap();
			ThreatMap.insert(PieceIThreatMap.begin(), PieceIThreatMap.end());
		}
	}
	std::set<Box*>::iterator itr;
	for (itr = opponentThreatMap.begin(); itr != opponentThreatMap.end(); itr++) {
		std::cout << "opponentThreatMap (*itr) " << (*itr)->x / s << " " << (*itr)->y / s << std::endl;
	}


	return ThreatMap;
}

void Player::updateVectorPieces(Piece* p)
{
	Pieces.erase(std::remove(Pieces.begin(), Pieces.end(), p), Pieces.end());

}

void Player::setopponentThreatMap(std::set<Box*> opponentThreatMap)
{
	this->opponentThreatMap = opponentThreatMap;
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


