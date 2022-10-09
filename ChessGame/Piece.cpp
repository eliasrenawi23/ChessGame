#include "Piece.h"
#include "Window.h"
Piece::Piece() :location(NULL), texture(NULL), firstMove(true)
{
}

Box* Piece::getLocation()
{
	return location;
}

void Piece::setLocation(Box* loc)
{
	location = loc;
	firstMove = false;
}


void Piece::renderPiece()
{
	int w;
	SDL_QueryTexture(texture, NULL, NULL, &w, &w);
	SDL_Rect fromRect, toRect;
	fromRect.w = w;
	fromRect.h = w;
	fromRect.x = fromRect.y = 0;

	toRect.w = location->size;
	toRect.h = location->size;

	toRect.x = location->x;
	toRect.y = location->y;
	SDL_RenderCopy(Window::m_renderer, texture, &fromRect, &toRect);

}

Piece::~Piece()
{
	location = NULL;
	SDL_DestroyTexture(texture);
}

std::set<Box*> Piece::colMovs(int x, int y, int direction)
{
	std::set<Box*> ClegalMoves;
	int n = Board::rowBoxNmbersandCols;
	for (int i = y; i < n && i >= 0; i = i + direction)
	{
		if (Board::gameboxess[x][i].getPiece() == NULL) {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
		}
		else if (Board::gameboxess[x][i].getPiece()->color != color) {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
			break;
		}
		else if (Board::gameboxess[x][i].getPiece()->color == color && (Board::gameboxess[x][i].getPiece() != this)) {
			break;
		}
	}
	return ClegalMoves;
}

std::set<Box*> Piece::colThreatMap(int x, int y, int direction)
{
	std::set<Box*> ClegalMoves;
	int n = Board::rowBoxNmbersandCols;
	for (int i = y; i < n && i >= 0; i = i + direction)
	{
		if (Board::gameboxess[x][i].getPiece() == NULL) {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
		}
		else  {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
			break;
		}
		
	}
	return ClegalMoves;
}

std::set<Box*> Piece::rowMovs(int x, int y, int direction)
{
	std::set<Box*> RlegalMoves;
	int n = Board::rowBoxNmbersandCols;
	for (int i = x; i < n && i >= 0; i = i + direction)
	{
		if (Board::gameboxess[i][y].getPiece() == NULL) {
			RlegalMoves.insert(&Board::gameboxess[i][y]);
		}
		else if (Board::gameboxess[i][y].getPiece()->color != color) {
			RlegalMoves.insert(&Board::gameboxess[i][y]);
			break;
		}
		else if (Board::gameboxess[i][y].getPiece()->color == color && (Board::gameboxess[i][y].getPiece() != this)) {
			break;
		}
	}
	return RlegalMoves;
}

std::set<Box*> Piece::rowThreatMap(int x, int y, int direction)
{
	std::set<Box*> RlegalMoves;
	int n = Board::rowBoxNmbersandCols;
	for (int i = x; i < n && i >= 0; i = i + direction)
	{
		if (Board::gameboxess[i][y].getPiece() == NULL) {
			RlegalMoves.insert(&Board::gameboxess[i][y]);
		}
		else{
			RlegalMoves.insert(&Board::gameboxess[i][y]);
			break;
		}
	}
	return RlegalMoves;
}

std::set<Box*> Piece::checkDiagonal(int x, int y, int direction, int Idirection)
{
	int n = Board::rowBoxNmbersandCols;
	std::set<Box*> DlegalMoves;

	for (int i = x, j = y; i < n && j < n && i >= 0 && j >= 0; i = i + direction * Idirection, j = j + direction)
	{
		if (Board::gameboxess[i][j].getPiece() == NULL) {
			DlegalMoves.insert(&Board::gameboxess[i][j]);
		}
		else if (Board::gameboxess[i][j].getPiece()->color != color) {
			DlegalMoves.insert(&Board::gameboxess[i][j]);
			break;
		}
		else if (Board::gameboxess[i][j].getPiece()->color == color && (Board::gameboxess[i][j].getPiece() != this)) {
			break;
		}
	}
	return DlegalMoves;
}

std::set<Box*> Piece::DiagonalThreatMap(int x, int y, int direction, int Idirection)
{
	int n = Board::rowBoxNmbersandCols;
	std::set<Box*> DlegalMoves;

	for (int i = x, j = y; i < n && j < n && i >= 0 && j >= 0; i = i + direction * Idirection, j = j + direction)
	{
		if (Board::gameboxess[i][j].getPiece() == NULL) {
			DlegalMoves.insert(&Board::gameboxess[i][j]);
		}
		else {
			DlegalMoves.insert(&Board::gameboxess[i][j]);
			break;
		}
	}
	return DlegalMoves;
}
