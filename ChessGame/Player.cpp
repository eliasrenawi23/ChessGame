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

std::set<Box*>  Player::play(Piece* const pieceToPlay, bool* checkmate)
{

	std::set<Box*> legalmoves = pieceToPlay->moveAndTake();
	bool gameover = false;

	int s = Board::BoxWidthandHigth;

	if (dynamic_cast<King*>(pieceToPlay) != nullptr) {
		//std::cout << "player.cpp king must remove locations " << std::endl;
		std::set<Box*>::iterator itr;
		for (const auto& itr : opponentThreatMap) {
			legalmoves.erase(itr);
		}
	}

	else if ((*checkmate)) {
		// If it's checkmate, filter moves for the king based on cover paths
		King* k = getKing();
		if (k != nullptr) {
			std::set<Box*> kingCoverPath = k->getCoverPath();
			std::set<Box*> validMoves;

			// Need the intersection between the legal moves and king's cover path
			std::set_intersection(legalmoves.begin(), legalmoves.end(), kingCoverPath.begin(), kingCoverPath.end(),
				std::inserter(validMoves, validMoves.begin()));

			return validMoves; // The piece can protect the king
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

	// Initialize pawns
	for (int x = 0; x < 8; x++) {
		Pieces.push_back(new Pawn(&(Board::gameboxess[x][PawnYPostion]), color));
		Board::gameboxess[x][PawnYPostion].setPiece(Pieces.back());
	}

	// Initialize other pieces (Bishops, Knights, Rooks, Queen, King)
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

void Player::handle_promotion(bool promotionMenu)
{
	//std::vector<Box*>PiecesOptions;
	 // Handle promotion menu display or cleanup
	if (promotionMenu) {
		PiecesOptions.push_back(new Queen(&(Board::gameboxess[2][4]), color));//Queen
		Board::gameboxess[2][4].setPiece(PiecesOptions.back());

		PiecesOptions.push_back(new Knight(&(Board::gameboxess[3][4]), color));//Knight
		Board::gameboxess[3][4].setPiece(PiecesOptions.back());

		PiecesOptions.push_back(new Rook(&(Board::gameboxess[4][4]), color));//Rook
		Board::gameboxess[4][4].setPiece(PiecesOptions.back());

		PiecesOptions.push_back(new Bishop(&(Board::gameboxess[5][4]), color));    //Bishop
		Board::gameboxess[5][4].setPiece(PiecesOptions.back());

	}
	else {
		// Clean up promotion options
		for (Piece* piece : PiecesOptions) {
			delete piece;
		}
		PiecesOptions.clear();
	}

}

void Player::dopromotion(Piece* selsectedPiece, int cor_x, int cor_y)
{

	//selsectedPiece Queen ? bishop rook?...
	// int cor x cor y is to where to put it
	// Perform the promotion by replacing the pawn with the selected piece

	if (dynamic_cast<Queen*>(selsectedPiece)) {
		Pieces.push_back(new Queen(&(Board::gameboxess[cor_x][cor_y]), color));    
	}
	else if (dynamic_cast<Bishop*>(selsectedPiece)) {
		Pieces.push_back(new Bishop(&(Board::gameboxess[cor_x][cor_y]), color));   
	}
	else if (dynamic_cast<Rook*>(selsectedPiece)) {
		Pieces.push_back(new Rook(&(Board::gameboxess[cor_x][cor_y]), color));    
	}
	else if (dynamic_cast<Knight*>(selsectedPiece)) {
		Pieces.push_back(new Knight(&(Board::gameboxess[cor_x][cor_y]), color));   
	}
	else {
		return;
	}
	Board::gameboxess[cor_x][cor_y].setPiece(Pieces.back());
	handle_promotion(false); //delete the list
}

std::vector<Piece*> Player::getPiecesOptions()
{
	return PiecesOptions;
}

Player::~Player()
{

	// Clean up all dynamically allocated pieces
	for (Piece* piece : Pieces) {
		delete piece;
	}
	Pieces.clear();
}

King* Player::getKing()
{

	// Find and return the player's king
	for (Piece* piece : Pieces) {
		if (King* k = dynamic_cast<King*>(piece)) {
			return k;
		}
	}
	return nullptr;
}


