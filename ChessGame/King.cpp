#include "King.h"
#include "Window.h"



King::King(Box* loc, PlayerColor color)
{
	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BK.svg");
	}
	else {
		surface = IMG_Load("texture/WK.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}



std::set<Box*>  King::moveAndTake()
{
	std::cout << "King clicked" << std::endl;
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	int n = Board::rowBoxNmbersandCols;
	std::set<Box*>  legalMoves;



	for (int i = x - 1; i < n && i < (x + 2); i++) {
		if (i < 0)continue;
		for (int j = y - 1; j < n && j < (y + 2); j++) {
			if (j < 0)continue;
			if (Board::gameboxess[i][j].getPiece() == NULL) {
				legalMoves.insert(&Board::gameboxess[i][j]);
			}
			else if (Board::gameboxess[i][j].getPiece()->color != color) {
				legalMoves.insert(&Board::gameboxess[i][j]);
			}
		}
	}


	return legalMoves;
}

std::set<Box*> King::PieceThreatMap()
{
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	int n = Board::rowBoxNmbersandCols;
	std::set<Box*>  legalMoves;

	for (int i = x - 1; i < n && i < (x + 2); i++) {
		if (i < 0)continue;
		for (int j = y - 1; j < n && j < (y + 2); j++) {
			if (j < 0)continue;
			legalMoves.insert(&Board::gameboxess[i][j]);
		}
	}


	return legalMoves;
}


King::~King()
{
}
