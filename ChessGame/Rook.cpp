#include "Rook.h"
#include "Window.h"



Rook::Rook(Box* loc, PlayerColor color)
{
	this->location = loc;
	this->x = (location->x) / Board::BoxWidthandHigth;
	this->y = (location->y) / Board::BoxWidthandHigth;
	this->color = color;


	SDL_Surface* surface = IMG_Load((color == PlayerColor::BLACK) ? "texture/BR.svg" : "texture/WR.svg");
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_DestroySurface(surface);
	
}


std::set<Box*> Rook::moveAndTake() {
	std::cout << "Rook clicked" << std::endl;

	bool threatInPath = false, kingInPath = false;
	std::set<Box*> legalMoves;

	std::set<Box*> row1 = rowMovs(x, y, 1, &threatInPath, &kingInPath);
	std::set<Box*> row2 = rowMovs(x, y, -1, &threatInPath, &kingInPath);
	row1.insert(row2.begin(), row2.end());
	if (threatInPath && kingInPath) return row1;

	threatInPath = kingInPath = false;
	std::set<Box*> col1 = colMovs(x, y, 1, &threatInPath, &kingInPath);
	std::set<Box*> col2 = colMovs(x, y, -1, &threatInPath, &kingInPath);
	col1.insert(col2.begin(), col2.end());
	if (threatInPath && kingInPath) return col1;

	legalMoves.insert(row1.begin(), row1.end());
	legalMoves.insert(col1.begin(), col1.end());

	return checkpinned(legalMoves);
}


std::set<Box*> Rook::PieceThreatMap(bool* checkmate) {
	std::set<Box*> legalMoves;

	std::set<Box*> row1 = rowThreatMap(x, y, 1, checkmate);
	std::set<Box*> row2 = rowThreatMap(x, y, -1, checkmate);
	row1.insert(row2.begin(), row2.end());

	std::set<Box*> col1 = colThreatMap(x, y, 1, checkmate);
	std::set<Box*> col2 = colThreatMap(x, y, -1, checkmate);
	col1.insert(col2.begin(), col2.end());

	legalMoves.insert(row1.begin(), row1.end());
	legalMoves.insert(col1.begin(), col1.end());

	return legalMoves;
}



Rook::~Rook()
{
}

