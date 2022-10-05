#include "Queen.h"
#include "Window.h"



Queen::Queen(Box* loc, PlayerColor color)
{
	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BQ.svg");
	}
	else {
		surface = IMG_Load("texture/WQ.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}



std::vector<Box*> Queen::moveAndTake()
{
	std::cout << "Queen clicked" << std::endl;
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	std::vector<Box*> legalMoves;
	legalMoves.push_back(location);

	std::vector<Box*> Diag1part1;
	std::vector<Box*> Diag1part2;

	Diag1part1 = checkDiagonal(x, y, 1, 1);
	Diag1part2 = checkDiagonal(x, y, -1, 1);
	Diag1part1.insert(Diag1part1.end(), Diag1part2.begin(), Diag1part2.end());

	std::vector<Box*> Diag2part1;
	std::vector<Box*> Diag2part2;

	Diag2part1 = checkDiagonal(x, y, 1, -1);
	Diag2part2 = checkDiagonal(x, y, -1, -1);
	Diag2part1.insert(Diag2part1.end(), Diag2part2.begin(), Diag2part2.end());

	legalMoves.insert(legalMoves.end(), Diag1part1.begin(), Diag1part1.end());
	legalMoves.insert(legalMoves.end(), Diag2part1.begin(), Diag2part1.end());

	std::vector<Box*> rowPart1;
	std::vector<Box*> rowPart2;

	rowPart1 = Row(x, y, 1);
	rowPart2 = Row(x, y, -1);
	rowPart1.insert(rowPart1.end(), rowPart2.begin(), rowPart2.end());

	std::vector<Box*> colPart1;
	std::vector<Box*> colPart2;

	colPart1 = col(x, y, 1);
	colPart2 = col(x, y, -1);
	colPart1.insert(colPart1.end(), colPart2.begin(), colPart2.end());

	legalMoves.insert(legalMoves.end(), rowPart1.begin(), rowPart1.end());
	legalMoves.insert(legalMoves.end(), colPart1.begin(), colPart1.end());



	return legalMoves;

}



std::vector<Box*> Queen::checkDiagonal(int x, int y, int direction, int Idirection)
{
	int n = Board::rowBoxNmbersandCols;
	std::vector<Box*> DlegalMoves;

	for (int i = x, j = y; i < n && j < n && i >= 0 && j >= 0; i = i + direction * Idirection, j = j + direction)
	{
		if (Board::gameboxess[i][j].getPiece() == NULL) {
			DlegalMoves.push_back(&Board::gameboxess[i][j]);
		}
		else if (Board::gameboxess[i][j].getPiece()->color != color) {
			DlegalMoves.push_back(&Board::gameboxess[i][j]);
			break;
		}
		else if (Board::gameboxess[i][j].getPiece()->color == color && (Board::gameboxess[i][j].getPiece() != this)) {
			break;
		}
	}
	return DlegalMoves;
}

std::vector<Box*> Queen::Row(int x, int y, int direction)
{
	std::vector<Box*> ClegalMoves;
	int n = Board::rowBoxNmbersandCols;
	for (int i = x; i < n && i >= 0; i = i + direction)
	{
		if (Board::gameboxess[i][y].getPiece() == NULL) {
			ClegalMoves.push_back(&Board::gameboxess[i][y]);
		}
		else if (Board::gameboxess[i][y].getPiece()->color != color) {
			ClegalMoves.push_back(&Board::gameboxess[i][y]);
			break;
		}
		else if (Board::gameboxess[i][y].getPiece()->color == color && (Board::gameboxess[i][y].getPiece() != this)) {
			break;
		}
	}
	return ClegalMoves;
}

std::vector<Box*> Queen::col(int x, int y, int direction)
{
	std::vector<Box*> ClegalMoves;
	int n = Board::rowBoxNmbersandCols;
	for (int i = y; i < n && i >= 0; i = i + direction)
	{
		if (Board::gameboxess[x][i].getPiece() == NULL) {
			ClegalMoves.push_back(&Board::gameboxess[x][i]);
		}
		else if (Board::gameboxess[x][i].getPiece()->color != color) {
			ClegalMoves.push_back(&Board::gameboxess[i][y]);
			break;
		}
		else if (Board::gameboxess[x][i].getPiece()->color == color && (Board::gameboxess[x][i].getPiece() != this)) {
			break;
		}
	}
	return ClegalMoves;
}
Queen::~Queen()
{
}
