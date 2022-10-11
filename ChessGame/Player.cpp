#include "Player.h"
#include "Box.h"
#include "Board.h"
#include <iostream>
#include <algorithm> 




Player::Player(PlayerColor color)
{
	this->color = color;
}

std::vector<Piece*> Player::getPieces()
{
	return this->Pieces;
}

std::set<Box*>  Player::play(Piece* pieceToPlay, bool* checkmate)
{

	std::set<Box*> legalmoves = pieceToPlay->moveAndTake();

	int s = Board::BoxWidthandHigth;

	if (dynamic_cast<King*>(pieceToPlay) != nullptr) {
	/*	std::set<Box*>::iterator itr2;
		for (itr2 = legalmoves.begin(); itr2 != legalmoves.end(); itr2++) {
			std::cout << "legalmoves (*itr2) " << (*itr2)->x / s << " " << (*itr2)->y / s << std::endl;
		}*/

		std::cout << "player.cpp king must remove locations " << std::endl;
		std::set<Box*>::iterator itr;
		for (itr = opponentThreatMap.begin(); itr != opponentThreatMap.end(); itr++) {
			//std::cout << "Player::play (*itr) " << (*itr)->x / s << " " << (*itr)->y / s << std::endl;
			legalmoves.erase((*itr));
		}
	}

	else if ((*checkmate)) {
		//check if checkmate and erace the other moves form the king // and finale put check mate to false
		King* k = getKing();
		if (k != NULL) {
			std::set<Box*>kingcoverPath = k->getCoverPath();
			std::set<Box*>endres;
			//need the intersection between two sets
			std::set_intersection(legalmoves.begin(), legalmoves.end(), kingcoverPath.begin(), kingcoverPath.end(),
				std::inserter(endres, endres.begin()));
			return endres;
		}
	}


	return legalmoves;
}

std::set<Box*> Player::ClacThreatMap(bool* checkmate)
{
	std::set<Box*> ThreatMap;
	int s = Board::BoxWidthandHigth;

	for (int i = 0; i < Pieces.size(); i++) {
		if (Pieces[i] != nullptr) {
			std::set<Box*> PieceIThreatMap = Pieces[i]->PieceThreatMap(checkmate);
			ThreatMap.insert(PieceIThreatMap.begin(), PieceIThreatMap.end());
		}
	}
	//std::set<Box*>::iterator itr;
	//for (itr = ThreatMap.begin(); itr != ThreatMap.end(); itr++) {
	//	std::cout << "Player::ClacThreatMap() (*itr) " << (*itr)->x / s << " " << (*itr)->y / s << std::endl;
	//}


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

King* Player::getKing()
{

	for (int i = 0; i < Pieces.size(); i++) {
		if (King *k=dynamic_cast<King*>(Pieces[i])) {
			return k;
		}
	}
	return nullptr;
}


