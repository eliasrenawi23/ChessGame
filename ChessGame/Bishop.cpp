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


std::vector<Box*> Bishop::moveAndTake()
{
	std::cout << "Bishop clicked" << std::endl;
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	std::vector<Box*> legalMoves;
	legalMoves.push_back(location);
	std::vector<Box*> Diag1part1;
	std::vector<Box*> Diag1part2;

	Diag1part1 = checkDiagonal(x, y , 1,1);
	Diag1part2 = checkDiagonal(x,y, -1,1);
	Diag1part1.insert(Diag1part1.end(), Diag1part2.begin(), Diag1part2.end());

	std::vector<Box*> Diag2part1;
	std::vector<Box*> Diag2part2;

	Diag2part1 = checkDiagonal(x,y, 1,-1);
	Diag2part2 = checkDiagonal(x,y, -1,-1);
	Diag2part1.insert(Diag2part1.end(), Diag2part2.begin(), Diag2part2.end());

	legalMoves.insert(legalMoves.end(), Diag1part1.begin(), Diag1part1.end());
	legalMoves.insert(legalMoves.end(), Diag2part1.begin(), Diag2part1.end());

	return legalMoves;

}
std::vector<Box*> Bishop::checkDiagonal(int x, int y, int direction, int Idirection)
{
	std::vector<Box*> DlegalMoves;
	int n = Board::rowBoxNmbersandCols;
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


Bishop::~Bishop()
{
}



