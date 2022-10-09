#include "Knight.h"
#include "Window.h"

Knight::Knight(Box* loc, PlayerColor color)
{
	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BN.svg");
	}
	else {
		surface = IMG_Load("texture/WN.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);
}



std::set<Box*>  Knight::moveAndTake()
{
	std::cout << "Knight clicked" << std::endl;
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	int n = Board::rowBoxNmbersandCols;
	//the Knight has 8 possible move at most
	std::set<Box*>  legalMoves;
	//1
	if (x - 1 >= 0 && y - 2 >= 0 &&
		((Board::gameboxess[x - 1][y - 2].getPiece() == NULL) || (Board::gameboxess[x - 1][y - 2].getPiece()->color != color))) {
		legalMoves.insert(&Board::gameboxess[x - 1][y - 2]); // L
	}
	//2

	if (x + 1 < n && y - 2 >= 0 &&
		((Board::gameboxess[x + 1][y - 2].getPiece() == NULL) || (Board::gameboxess[x + 1][y - 2].getPiece()->color != color))) {
		legalMoves.insert(&Board::gameboxess[x + 1][y - 2]); // L backword
	}
	//3
	if (x - 1 >= 0 && y + 2 < n &&
		((Board::gameboxess[x - 1][y + 2].getPiece() == NULL) || (Board::gameboxess[x - 1][y + 2].getPiece()->color != color))) {
		legalMoves.insert(&Board::gameboxess[x - 1][y + 2]); // L down
	}
	//4
	if (x + 1 < n && y + 2 < n &&
		((Board::gameboxess[x + 1][y + 2].getPiece() == NULL) || (Board::gameboxess[x + 1][y + 2].getPiece()->color != color))) {
		legalMoves.insert(&Board::gameboxess[x + 1][y + 2]); // L backword down
	}
	//////---------
	//5
	if (x - 2 >= 0 && y - 1 >= 0 &&
		((Board::gameboxess[x - 2][y - 1].getPiece() == NULL) || (Board::gameboxess[x - 2][y - 1].getPiece()->color != color))) {
		legalMoves.insert(&Board::gameboxess[x - 2][y - 1]); // L 90dig
	}
	//6
	if (x - 2 >= 0 && y + 1 <n  &&
		((Board::gameboxess[x - 2][y + 1].getPiece() == NULL) || (Board::gameboxess[x - 2][y + 1].getPiece()->color != color))) {
		legalMoves.insert(&Board::gameboxess[x - 2][y + 1]); // L 90dig
	}
	//7
	if (x + 2 < n && y - 1 >= 0 &&
		((Board::gameboxess[x + 2][y - 1].getPiece() == NULL) || (Board::gameboxess[x + 2][y - 1].getPiece()->color != color))) {
		legalMoves.insert(&Board::gameboxess[x + 2][y - 1]); // L backword
	}
	//8
	if (x + 2 < n && y + 1 < n &&
		((Board::gameboxess[x + 2][y + 1].getPiece() == NULL) || (Board::gameboxess[x + 2][y + 1].getPiece()->color != color))) {
		legalMoves.insert(&Board::gameboxess[x + 2][y + 1]); // L backword
	}

	return legalMoves;

}

std::set<Box*> Knight::PieceThreatMap()
{
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	int n = Board::rowBoxNmbersandCols;
	//the Knight has 8 possible move at most
	std::set<Box*>  legalMoves;
	//1
	if (x - 1 >= 0 && y - 2 >= 0 ) {
		legalMoves.insert(&Board::gameboxess[x - 1][y - 2]); // L
	}
	//2
	if (x + 1 < n && y - 2 >= 0) {
		legalMoves.insert(&Board::gameboxess[x + 1][y - 2]); // L backword
	}
	//3
	if (x - 1 >= 0 && y + 2 < n) {
		legalMoves.insert(&Board::gameboxess[x - 1][y + 2]); // L down
	}
	//4
	if (x + 1 < n && y + 2 < n ) {
		legalMoves.insert(&Board::gameboxess[x + 1][y + 2]); // L backword down
	}
	//////---------
	//5
	if (x - 2 >= 0 && y - 1 >= 0 ) {
		legalMoves.insert(&Board::gameboxess[x - 2][y - 1]); // L 90dig
	}
	//6
	if (x - 2 >= 0 && y + 1 < n) {
		legalMoves.insert(&Board::gameboxess[x - 2][y + 1]); // L 90dig
	}
	//7
	if (x + 2 < n && y - 1 >= 0 ) {
		legalMoves.insert(&Board::gameboxess[x + 2][y - 1]); // L backword
	}
	//8
	if (x + 2 < n && y + 1 < n) {
		legalMoves.insert(&Board::gameboxess[x + 2][y + 1]); // L backword
	}

	return legalMoves;
}

Knight::~Knight()
{
}
