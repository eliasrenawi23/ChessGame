#include "Bishop.h"
#include "Window.h"


Bishop::Bishop(Box* loc, PlayerColor color)
{

	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BB.svg");
	}
	else {
		surface = IMG_Load("texture/WB.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}


std::set<Box*>  Bishop::moveAndTake()
{
	std::cout << "Bishop clicked" << std::endl;
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;

	bool thretInPath = false, kingInPath = false;//to check if the pice is pinned

	std::set<Box*>  legalMoves;
	std::set<Box*>  Diag1part1;
	std::set<Box*>  Diag1part2;

	Diag1part1 = checkDiagonal(x, y, 1, 1, &thretInPath, &kingInPath); //i++ j++
	Diag1part2 = checkDiagonal(x, y, -1, 1, &thretInPath, &kingInPath);//i-- j--
	Diag1part1.insert( Diag1part2.begin(), Diag1part2.end());

	if (thretInPath && kingInPath)return Diag1part1;
	thretInPath = false;
	kingInPath = false;

	std::set<Box*>  Diag2part1;
	std::set<Box*>  Diag2part2;

	Diag2part1 = checkDiagonal(x, y, 1, -1, &thretInPath, &kingInPath);//i-- j++
	Diag2part2 = checkDiagonal(x, y, -1, -1, &thretInPath, &kingInPath);//i++ j--
	Diag2part1.insert( Diag2part2.begin(), Diag2part2.end());

	if (thretInPath && kingInPath)return Diag2part1;


	legalMoves.insert( Diag1part1.begin(), Diag1part1.end());
	legalMoves.insert( Diag2part1.begin(), Diag2part1.end());

	return legalMoves;

}

std::set<Box*> Bishop::PieceThreatMap()
{
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	std::set<Box*>  legalMoves;
	std::set<Box*>  Diag1part1;
	std::set<Box*>  Diag1part2;

	Diag1part1 = DiagonalThreatMap(x, y, 1, 1);
	Diag1part2 = DiagonalThreatMap(x, y, -1, 1);
	Diag1part1.insert(Diag1part2.begin(), Diag1part2.end());

	std::set<Box*>  Diag2part1;
	std::set<Box*>  Diag2part2;

	Diag2part1 = DiagonalThreatMap(x, y, 1, -1);
	Diag2part2 = DiagonalThreatMap(x, y, -1, -1);
	Diag2part1.insert(Diag2part2.begin(), Diag2part2.end());

	legalMoves.insert(Diag1part1.begin(), Diag1part1.end());
	legalMoves.insert(Diag2part1.begin(), Diag2part1.end());

	return legalMoves;
}

Bishop::~Bishop()
{
}



