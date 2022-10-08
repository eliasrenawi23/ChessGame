#include "Pawn.h"
#include "Window.h"


Pawn::Pawn(Box* loc, PlayerColor color)
{
	location = loc;
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
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	std::set<Box*>  legalMoves;
	legalMoves.insert(location); // put the pice in the first elemnt

	//to do (must check for en passant) and pawn promotion
	if (firstMove && (Board::gameboxess[x][y - 2 * diraction].getPiece() == NULL)) { //forword 2 boxes  
		legalMoves.insert(&Board::gameboxess[x][y - 2 * diraction]);
	}
	if (y - diraction >= 0 && y - diraction < Board::rowBoxNmbersandCols && (Board::gameboxess[x][y - diraction].getPiece() == NULL)) {
		legalMoves.insert(&Board::gameboxess[x][y - diraction]); // forword
	}
	if (x - 1 >= 0 && y - diraction >= 0 && y - diraction < Board::rowBoxNmbersandCols && (Board::gameboxess[x - 1][y - diraction].getPiece() != NULL)) {
		if ((&Board::gameboxess[x - 1][y - diraction])->getPiece()->color != color) {
			legalMoves.insert(&Board::gameboxess[x - 1][y - diraction]); //left corner
		}
	}
	if (x + 1 < Board::rowBoxNmbersandCols && y - diraction >= 0 && y - diraction < Board::rowBoxNmbersandCols && (Board::gameboxess[x + 1][y - diraction].getPiece() != NULL)) {
		if ((&Board::gameboxess[x + 1][y - diraction])->getPiece()->color != color) {
			legalMoves.insert(&Board::gameboxess[x + 1][y - diraction]); //right corner
		}
	}

	return legalMoves;
}



Pawn::~Pawn()
{

}
