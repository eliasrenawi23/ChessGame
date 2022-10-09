#include "Pawn.h"
#include "Window.h"
#include <algorithm> 


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



std::set<Box*> Pawn::PieceThreatMap()
{
	int diraction = (color == PlayerColor::WHITE) ? 1 : -1;//if white move -1 id black move 1
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	int n = Board::rowBoxNmbersandCols;
	std::set<Box*>  legalMoves;


	if (x - 1 >= 0 && y - diraction >= 0 && y - diraction < n) {
			legalMoves.insert(&Board::gameboxess[x - 1][y - diraction]); //left corner	
	}
	if (x + 1 < n && y - diraction >= 0 && y - diraction < n ) {

		legalMoves.insert(&Board::gameboxess[x + 1][y - diraction]); //right corner
	}
	return legalMoves;
}

Pawn::~Pawn()
{

}

void Pawn::checkpinned(std::set<Box*> legalMoves)
{
	//std::set<Box*>  Diag1part1;
	//std::set<Box*>  Diag1part2;
	//int x = (location->x) / Board::BoxWidthandHigth;
	//int y = (location->y) / Board::BoxWidthandHigth;
	//bool thretInPath = false, kingInPath = false;//to check if the pice is pinned

	//Diag1part1 = checkDiagonal(x, y, 1, 1, &thretInPath, &kingInPath);
	//Diag1part2 = checkDiagonal(x, y, -1, 1, &thretInPath, &kingInPath);
	//Diag1part1.insert(Diag1part2.begin(), Diag1part2.end());
	//if (thretInPath && kingInPath) {
	//	std::set_intersection(legalMoves.begin(), legalMoves.end(), Diag1part1.begin(), Diag1part1.end(),
	//		std::inserter(legalMoves, legalMoves.begin()));
	//}

	//thretInPath = false;
	//kingInPath = false;

	//std::set<Box*>  Diag2part1;
	//std::set<Box*>  Diag2part2;

	//Diag2part1 = checkDiagonal(x, y, 1, -1, &thretInPath, &kingInPath);
	//Diag2part2 = checkDiagonal(x, y, -1, -1, &thretInPath, &kingInPath);
	//Diag2part1.insert(Diag2part2.begin(), Diag2part2.end());

	//if (thretInPath && kingInPath)return Diag2part1;
	//thretInPath = false;
	//kingInPath = false;



	//legalMoves.insert(Diag1part1.begin(), Diag1part1.end());
	//legalMoves.insert(Diag2part1.begin(), Diag2part1.end());

	//std::set<Box*>  rowPart1;
	//std::set<Box*>  rowPart2;

	//rowPart1 = rowMovs(x, y, 1, &thretInPath, &kingInPath);
	//rowPart2 = rowMovs(x, y, -1, &thretInPath, &kingInPath);
	//rowPart1.insert(rowPart2.begin(), rowPart2.end());

	//if (thretInPath && kingInPath)return rowPart1;
	//thretInPath = false;
	//kingInPath = false;

	//std::set<Box*>  colPart1;
	//std::set<Box*>  colPart2;

	//colPart1 = colMovs(x, y, 1, &thretInPath, &kingInPath);
	//colPart2 = colMovs(x, y, -1, &thretInPath, &kingInPath);
	//colPart1.insert(colPart2.begin(), colPart2.end());

	//if (thretInPath && kingInPath)return colPart1;

	//legalMoves.insert(rowPart1.begin(), rowPart1.end());
	//legalMoves.insert(colPart1.begin(), colPart1.end());
	//
	
}
