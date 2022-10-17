#include "King.h"
#include "Window.h"



King::King(Box* loc, PlayerColor color)
{
	this->location = loc;
	this->x = (location->x) / Board::BoxWidthandHigth;
	this->y = (location->y) / Board::BoxWidthandHigth;
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
			else if (Board::gameboxess[i][j].getPiece()->getColor() != color) {
				legalMoves.insert(&Board::gameboxess[i][j]);
			}
		}
	}
	std::set<Box*>  CastleMoves= checkCastle();
	legalMoves.insert(CastleMoves.begin(), CastleMoves.end());

	return legalMoves;
}

std::set<Box*> King::PieceThreatMap(bool* checkmate)
{

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

	bool thretInPath = false, kingInPath = false;//to check if the pice is pinned
	std::set<Box*>  CoverPath;

	std::set<Box*>  Diag1part1, Diag1part2;

	Diag1part1 = checkDiagonal(x, y, 1, 1, &thretInPath, &kingInPath);
	if (thretInPath)return Diag1part1;
	thretInPath = false;
	Diag1part2 = checkDiagonal(x, y, -1, 1, &thretInPath, &kingInPath);
	if (thretInPath)return Diag1part2;
	thretInPath = false;


	std::set<Box*>  Diag2part1, Diag2part2;

	Diag2part1 = checkDiagonal(x, y, 1, -1, &thretInPath, &kingInPath);
	if (thretInPath)return Diag2part1;
	thretInPath = false;
	Diag2part2 = checkDiagonal(x, y, -1, -1, &thretInPath, &kingInPath);
	if (thretInPath)return Diag2part2;
	thretInPath = false;

	std::set<Box*>  rowPart1, rowPart2;

	rowPart1 = rowMovs(x, y, 1, &thretInPath, &kingInPath);
	if (thretInPath)return rowPart1;
	thretInPath = false;

	rowPart2 = rowMovs(x, y, -1, &thretInPath, &kingInPath);
	if (thretInPath)return rowPart2;
	thretInPath = false;


	std::set<Box*>  colPart1, colPart2;

	colPart1 = colMovs(x, y, 1, &thretInPath, &kingInPath);
	if (thretInPath)return colPart1;
	thretInPath = false;
	colPart2 = colMovs(x, y, -1, &thretInPath, &kingInPath);

	if (thretInPath)return colPart2;
	thretInPath = false;

	CoverPath.insert(checkKinghtCheckmate());
	//must add the knight also

	return CoverPath;
}

Box* King::checkKinghtCheckmate()
{
	const int n = Board::rowBoxNmbersandCols;
	//the Knight has 8 possible move at most

	//1
	if (x - 1 >= 0 && y - 2 >= 0 &&
		((Board::gameboxess[x - 1][y - 2].getPiece() != NULL) || (Board::gameboxess[x - 1][y - 2].getPiece()->getColor() != color))) {
		if (Knight* t = dynamic_cast<Knight*>(Board::gameboxess[x - 1][y - 2].getPiece())) {//  opponent  king in our path
			return &Board::gameboxess[x - 1][y - 2];

		}
	}
	//2

	if (x + 1 < n && y - 2 >= 0 &&
		((Board::gameboxess[x + 1][y - 2].getPiece() == NULL) || (Board::gameboxess[x + 1][y - 2].getPiece()->getColor() != color))) {
		if (Knight* t = dynamic_cast<Knight*>(Board::gameboxess[x + 1][y - 2].getPiece())) {//  opponent  king in our path
			return &Board::gameboxess[x + 1][y - 2];

		}
	}
	//3
	if (x - 1 >= 0 && y + 2 < n &&
		((Board::gameboxess[x - 1][y + 2].getPiece() == NULL) || (Board::gameboxess[x - 1][y + 2].getPiece()->getColor() != color))) {
		if (Knight* t = dynamic_cast<Knight*>(Board::gameboxess[x - 1][y + 2].getPiece())) {//  opponent  king in our path
			return &Board::gameboxess[x - 1][y + 2];

		}
	}
	//4
	if (x + 1 < n && y + 2 < n &&
		((Board::gameboxess[x + 1][y + 2].getPiece() == NULL) || (Board::gameboxess[x + 1][y + 2].getPiece()->getColor() != color))) {
		if (Knight* t = dynamic_cast<Knight*>(Board::gameboxess[x + 1][y + 2].getPiece())) {//  opponent  king in our path
			return &Board::gameboxess[x + 1][y + 2];

		}
	}
	//////---------
	//5
	if (x - 2 >= 0 && y - 1 >= 0 &&
		((Board::gameboxess[x - 2][y - 1].getPiece() == NULL) || (Board::gameboxess[x - 2][y - 1].getPiece()->getColor() != color))) {
		if (Knight* t = dynamic_cast<Knight*>(Board::gameboxess[x - 2][y - 1].getPiece())) {//  opponent  king in our path
			return &Board::gameboxess[x - 2][y - 1];

		}
	}
	//6
	if (x - 2 >= 0 && y + 1 < n &&
		((Board::gameboxess[x - 2][y + 1].getPiece() == NULL) || (Board::gameboxess[x - 2][y + 1].getPiece()->getColor() != color))) {
		if (Knight* t = dynamic_cast<Knight*>(Board::gameboxess[x - 2][y + 1].getPiece())) {//  opponent  king in our path
			return &Board::gameboxess[x - 2][y + 1];

		}
	}
	//7
	if (x + 2 < n && y - 1 >= 0 &&
		((Board::gameboxess[x + 2][y - 1].getPiece() == NULL) || (Board::gameboxess[x + 2][y - 1].getPiece()->getColor() != color))) {
		if (Knight* t = dynamic_cast<Knight*>(Board::gameboxess[x + 1][y - 1].getPiece())) {//  opponent  king in our path
			return &Board::gameboxess[x + 2][y - 1];

		}
	}
	//8
	if (x + 2 < n && y + 1 < n &&
		((Board::gameboxess[x + 2][y + 1].getPiece() == NULL) || (Board::gameboxess[x + 2][y + 1].getPiece()->getColor() != color))) {
		if (Knight* t = dynamic_cast<Knight*>(Board::gameboxess[x + 2][y + 1].getPiece())) {//  opponent  king in our path
			return &Board::gameboxess[x + 2][y + 1];

		}
	}
	return nullptr;
}

std::set<Box*> King::checkCastle()
{
	if (!firstMove) return std::set<Box*>();

	std::set<Box*>  CastleMoves; //chec the two rooks

	int y_cor = (color == PlayerColor::WHITE) ? 0 : 7; //get the y cordinate for the rooks 

	Piece* p = Board::gameboxess[0][y].getPiece();
	if (p != NULL && p->getColor() == color && (dynamic_cast<Rook*>(p)) && p->firstMove) { //first rook 
		if (Board::gameboxess[1][y].getPiece() == NULL && Board::gameboxess[2][y].getPiece() == NULL && Board::gameboxess[3][y].getPiece() == NULL) {
			CastleMoves.insert(&Board::gameboxess[2][y]);
		}
	}
     p = Board::gameboxess[7][y].getPiece();
	if (p != NULL && p->getColor() == color && (dynamic_cast<Rook*>(p)) && p->firstMove) { //second rook
		if (Board::gameboxess[5][y].getPiece() == NULL && Board::gameboxess[6][y].getPiece() == NULL) {
			CastleMoves.insert(&Board::gameboxess[6][y]);
		}
	}



	return CastleMoves;

}


King::~King()
{
}
