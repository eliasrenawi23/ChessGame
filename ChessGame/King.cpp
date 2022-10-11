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

	legalMoves.insert(location);


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

std::set<Box*> King::PieceThreatMap(bool* checkmate)
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

std::set<Box*> King::getCoverPath()
{
	std::cout << "King::getCoverPath" << std::endl;
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	bool thretInPath = false, kingInPath = false;//to check if the pice is pinned
	std::set<Box*>  CoverPath;

	std::set<Box*>  Diag1part1;
	std::set<Box*>  Diag1part2;

	Diag1part1 = checkDiagonal(x, y, 1, 1, &thretInPath, &kingInPath);
	if (thretInPath)return Diag1part1;
	thretInPath = false;
	Diag1part2 = checkDiagonal(x, y, -1, 1, &thretInPath, &kingInPath);
	if (thretInPath)return Diag1part2;
	thretInPath = false;


	std::set<Box*>  Diag2part1;
	std::set<Box*>  Diag2part2;

	Diag2part1 = checkDiagonal(x, y, 1, -1, &thretInPath, &kingInPath);
	if (thretInPath)return Diag2part1;
	thretInPath = false;
	Diag2part2 = checkDiagonal(x, y, -1, -1, &thretInPath, &kingInPath);
	if (thretInPath)return Diag2part2;
	thretInPath = false;

	std::set<Box*>  rowPart1;
	std::set<Box*>  rowPart2;

	rowPart1 = rowMovs(x, y, 1, &thretInPath, &kingInPath);
	if (thretInPath)return rowPart1;
	thretInPath = false;

	rowPart2 = rowMovs(x, y, -1, &thretInPath, &kingInPath);
	if (thretInPath)return rowPart2;
	thretInPath = false;


	std::set<Box*>  colPart1;
	std::set<Box*>  colPart2;

	colPart1 = colMovs(x, y, 1, &thretInPath, &kingInPath);
	if (thretInPath)return colPart1;
	thretInPath = false;
	colPart2 = colMovs(x, y, -1, &thretInPath, &kingInPath);

	if (thretInPath)return colPart2;
	thretInPath = false;

	return CoverPath;
}


King::~King()
{
}
