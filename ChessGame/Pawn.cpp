#include "Pawn.h"
#include "Window.h"
#include <algorithm> 


Pawn::Pawn(Box* loc, PlayerColor color) :PoosblieEnPassant(false)
{
	location = loc;
	x = (location->x) / Board::BoxWidthandHigth;
	y = (location->y) / Board::BoxWidthandHigth;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BP.svg");
	}
	else {
		surface = IMG_Load("texture/WP.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);

}
std::set<Box*>  Pawn::moveAndTake()
{
	std::cout << "Pawn clicked" << std::endl;
	int diraction = (color == PlayerColor::WHITE) ? 1 : -1;//if white move -1 id black move 1
	int n = Board::rowBoxNmbersandCols;

	std::set<Box*>  legalMoves;

	//to do (must check for en passant) and pawn promotion
	if (firstMove && (Board::gameboxess[x][y - 2 * diraction].getPiece() == NULL)) { //forword 2 boxes  
		legalMoves.insert(&Board::gameboxess[x][y - 2 * diraction]);
	}
	if (y - diraction >= 0 && y - diraction < n && (Board::gameboxess[x][y - diraction].getPiece() == NULL)) {
		legalMoves.insert(&Board::gameboxess[x][y - diraction]); // forword
	}
	if (x - 1 >= 0 && y - diraction >= 0 && y - diraction < n && (Board::gameboxess[x - 1][y - diraction].getPiece() != NULL)) {
		if ((&Board::gameboxess[x - 1][y - diraction])->getPiece()->getColor() != color) {
			legalMoves.insert(&Board::gameboxess[x - 1][y - diraction]); //left corner
		}
	}
	if (x + 1 < n && y - diraction >= 0 && y - diraction < n && (Board::gameboxess[x + 1][y - diraction].getPiece() != NULL)) {
		if ((&Board::gameboxess[x + 1][y - diraction])->getPiece()->getColor() != color) {
			legalMoves.insert(&Board::gameboxess[x + 1][y - diraction]); //right corner
		}
	}
	Box* EnPassant = CheckEnPassant();
	if (EnPassant != NULL)legalMoves.insert(EnPassant);
	legalMoves = checkpinned(legalMoves);

	return legalMoves;
}
Box* Pawn::CheckEnPassant()
{
	int diraction = (color == PlayerColor::WHITE) ? 1 : -1;//if white move -1 id black move 1
	int n = Board::rowBoxNmbersandCols;

	Piece* p = (&Board::gameboxess[x - 1][y])->getPiece();
	if (x - 1 >= 0 && y - diraction >= 0 && y - diraction < n && (Board::gameboxess[x - 1][y - diraction].getPiece() == NULL)) {
		if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
			if (pawn->getColor() != color && pawn->PoosblieEnPassant) {
				return(&Board::gameboxess[x - 1][y - diraction]); //left corner
			}
		}
	}
	p = (&Board::gameboxess[x + 1][y])->getPiece();
	if (x + 1 < n && y - diraction >= 0 && y - diraction < n && (Board::gameboxess[x + 1][y - diraction].getPiece() == NULL)) {
		if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
			if (pawn->getColor() != color && pawn->PoosblieEnPassant) {
				return (&Board::gameboxess[x + 1][y - diraction]); //right corner
			}
		}
	}




	return NULL;
}



std::set<Box*> Pawn::PieceThreatMap(bool* checkmateh)
{
	int diraction = (color == PlayerColor::WHITE) ? 1 : -1;//if white move -1 id black move 1

	int n = Board::rowBoxNmbersandCols;
	std::set<Box*>  legalMoves;


	if (x - 1 >= 0 && y - diraction >= 0 && y - diraction < n) {
		if (King* t = dynamic_cast<King*>(Board::gameboxess[x - 1][y - diraction].getPiece())) {//  opponent  king in our path
			if (t->getColor() != color) {
				*checkmateh = true;
			}
		}
		legalMoves.insert(&Board::gameboxess[x - 1][y - diraction]); //left corner	
	}
	if (x + 1 < n && y - diraction >= 0 && y - diraction < n) {
		if (King* t = dynamic_cast<King*>(Board::gameboxess[x + 1][y - diraction].getPiece())) {//  opponent  king in our path
			if (t->getColor() != color) {
				*checkmateh = true;
			}
		}
		legalMoves.insert(&Board::gameboxess[x + 1][y - diraction]); //right corner
	}
	return legalMoves;




}


Pawn::~Pawn()
{

}
