#include "Rook.h"
#include "Window.h"



Rook::Rook(Box* loc, PlayerColor color)
{
	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BR.svg");
	}
	else {
		surface = IMG_Load("texture/WR.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}


std::vector<Box*> Rook::moveAndTake()
{
	std::cout << "Bishop clicked" << std::endl;
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	std::vector<Box*> legalMoves;
	legalMoves.push_back(location);
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


Rook::~Rook()
{
}

std::vector<Box*> Rook::col(int x, int y, int direction)
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

std::vector<Box*> Rook::Row(int x, int y, int direction)
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
