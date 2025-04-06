#include "Queen.h"
#include "Window.h"



Queen::Queen(Box* loc, PlayerColor color)
{


	this->location = loc;
	this->x = (location->x) / Board::BoxWidthandHigth;
	this->y = (location->y) / Board::BoxWidthandHigth;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BQ.svg");
	}
	else {
		surface = IMG_Load("texture/WQ.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_DestroySurface(surface);
}



std::set<Box*>  Queen::moveAndTake()
{
	std::cout << "Queen clicked" << std::endl;
	
	bool thretInPath = false, kingInPath = false;//to check if the pice is pinned
	std::set<Box*>  legalMoves;

	std::set<Box*>  Diag1part1,Diag1part2;

	Diag1part1 = checkDiagonal(x, y, 1, 1, &thretInPath, &kingInPath);
	Diag1part2 = checkDiagonal(x, y, -1, 1, &thretInPath, &kingInPath);
	Diag1part1.insert( Diag1part2.begin(), Diag1part2.end());
	if (thretInPath && kingInPath)return Diag1part1;
	kingInPath= thretInPath = false;

	std::set<Box*>  Diag2part1, Diag2part2;

	Diag2part1 = checkDiagonal(x, y, 1, -1, &thretInPath, &kingInPath);
	Diag2part2 = checkDiagonal(x, y, -1, -1, &thretInPath, &kingInPath);
	Diag2part1.insert(Diag2part2.begin(), Diag2part2.end());

	if (thretInPath && kingInPath)return Diag2part1;
	kingInPath = thretInPath = false;




	legalMoves.insert( Diag1part1.begin(), Diag1part1.end());
	legalMoves.insert( Diag2part1.begin(), Diag2part1.end());

	std::set<Box*>  rowPart1, rowPart2;

	rowPart1 = rowMovs(x, y, 1, &thretInPath, &kingInPath);
	rowPart2 = rowMovs(x, y, -1, &thretInPath, &kingInPath);
	rowPart1.insert( rowPart2.begin(), rowPart2.end());

	if (thretInPath && kingInPath)return rowPart1;
	kingInPath = thretInPath = false;


	std::set<Box*>  colPart1, colPart2;

	colPart1 = colMovs(x, y, 1, &thretInPath, &kingInPath);
	colPart2 = colMovs(x, y, -1, &thretInPath, &kingInPath);
	colPart1.insert( colPart2.begin(), colPart2.end());

	if (thretInPath && kingInPath)return colPart1;

	legalMoves.insert( rowPart1.begin(), rowPart1.end());
	legalMoves.insert( colPart1.begin(), colPart1.end());



	return legalMoves;

}

std::set<Box*> Queen::PieceThreatMap(bool* checkmate)
{

	std::set<Box*>  legalMoves;
	std::set<Box*>  Diag1part1,Diag1part2;

	Diag1part1 = DiagonalThreatMap(x, y, 1, 1, checkmate);
	Diag1part2 = DiagonalThreatMap(x, y, -1, 1, checkmate);
	Diag1part1.insert(Diag1part2.begin(), Diag1part2.end());

	std::set<Box*>  Diag2part1,Diag2part2;

	Diag2part1 = DiagonalThreatMap(x, y, 1, -1, checkmate);
	Diag2part2 = DiagonalThreatMap(x, y, -1, -1, checkmate);
	Diag2part1.insert(Diag2part2.begin(), Diag2part2.end());

	legalMoves.insert(Diag1part1.begin(), Diag1part1.end());
	legalMoves.insert(Diag2part1.begin(), Diag2part1.end());

	std::set<Box*>  rowPart1, rowPart2;

	rowPart1 = rowThreatMap(x, y, 1, checkmate);
	rowPart2 = rowThreatMap(x, y, -1, checkmate);
	rowPart1.insert(rowPart2.begin(), rowPart2.end());

	std::set<Box*>  colPart1,colPart2;

	colPart1 = colThreatMap(x, y, 1, checkmate);
	colPart2 = colThreatMap(x, y, -1, checkmate);
	colPart1.insert(colPart2.begin(), colPart2.end());

	legalMoves.insert(rowPart1.begin(), rowPart1.end());
	legalMoves.insert(colPart1.begin(), colPart1.end());



	return legalMoves;
}

Queen::~Queen()
{
}
