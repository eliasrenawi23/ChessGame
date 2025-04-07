#include "Queen.h"
#include "Window.h"



Queen::Queen(Box* loc, PlayerColor color)
{


	this->location = loc;
	this->x = (location->x) / Board::BoxWidthandHigth;
	this->y = (location->y) / Board::BoxWidthandHigth;
	this->color = color;

	SDL_Surface* surface = IMG_Load((color == PlayerColor::BLACK) ? "texture/BQ.svg" : "texture/WQ.svg");
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_DestroySurface(surface);
}



std::set<Box*>  Queen::moveAndTake()
{
	std::cout << "Queen clicked" << std::endl;
	
	bool threatInPath = false, kingInPath = false;//to check if the pice is pinned
	std::set<Box*> legalMoves;

	// Diagonal moves
	std::set<Box*> diag1a = checkDiagonal(x, y, 1, 1, &threatInPath, &kingInPath);
	std::set<Box*> diag1b = checkDiagonal(x, y, -1, 1, &threatInPath, &kingInPath);
	diag1a.insert(diag1b.begin(), diag1b.end());
	if (threatInPath && kingInPath) return diag1a;

	threatInPath = kingInPath = false;
	std::set<Box*> diag2a = checkDiagonal(x, y, 1, -1, &threatInPath, &kingInPath);
	std::set<Box*> diag2b = checkDiagonal(x, y, -1, -1, &threatInPath, &kingInPath);
	diag2a.insert(diag2b.begin(), diag2b.end());
	if (threatInPath && kingInPath) return diag2a;

	legalMoves.insert(diag1a.begin(), diag1a.end());
	legalMoves.insert(diag2a.begin(), diag2a.end());

	// Row moves
	threatInPath = kingInPath = false;
	std::set<Box*> row1 = rowMovs(x, y, 1, &threatInPath, &kingInPath);
	std::set<Box*> row2 = rowMovs(x, y, -1, &threatInPath, &kingInPath);
	row1.insert(row2.begin(), row2.end());
	if (threatInPath && kingInPath) return row1;

	// Column moves
	threatInPath = kingInPath = false;
	std::set<Box*> col1 = colMovs(x, y, 1, &threatInPath, &kingInPath);
	std::set<Box*> col2 = colMovs(x, y, -1, &threatInPath, &kingInPath);
	col1.insert(col2.begin(), col2.end());
	if (threatInPath && kingInPath) return col1;

	legalMoves.insert(row1.begin(), row1.end());
	legalMoves.insert(col1.begin(), col1.end());


	return checkpinned(legalMoves);

}

std::set<Box*> Queen::PieceThreatMap(bool* checkmate)
{
	std::set<Box*> legalMoves;

	std::set<Box*> diag1 = DiagonalThreatMap(x, y, 1, 1, checkmate);
	std::set<Box*> diag2 = DiagonalThreatMap(x, y, -1, 1, checkmate);
	std::set<Box*> diag3 = DiagonalThreatMap(x, y, 1, -1, checkmate);
	std::set<Box*> diag4 = DiagonalThreatMap(x, y, -1, -1, checkmate);

	legalMoves.insert(diag1.begin(), diag1.end());
	legalMoves.insert(diag2.begin(), diag2.end());
	legalMoves.insert(diag3.begin(), diag3.end());
	legalMoves.insert(diag4.begin(), diag4.end());

	std::set<Box*> row1 = rowThreatMap(x, y, 1, checkmate);
	std::set<Box*> row2 = rowThreatMap(x, y, -1, checkmate);
	std::set<Box*> col1 = colThreatMap(x, y, 1, checkmate);
	std::set<Box*> col2 = colThreatMap(x, y, -1, checkmate);

	legalMoves.insert(row1.begin(), row1.end());
	legalMoves.insert(row2.begin(), row2.end());
	legalMoves.insert(col1.begin(), col1.end());
	legalMoves.insert(col2.begin(), col2.end());

	return legalMoves;
}

Queen::~Queen()
{
}
