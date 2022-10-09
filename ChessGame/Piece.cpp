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

std::set<Box*> Piece::colMovs(int x, int y, int direction,bool * thretInPath,bool * kingInPath)
{
	std::set<Box*> ClegalMoves;
	//bool thretInPath = false, kingInPath = false;
	int n = Board::rowBoxNmbersandCols;
	for (int i = y; i < n && i >= 0; i = i + direction)
	{
		Piece* p = Board::gameboxess[x][i].getPiece();

		if (p == NULL) {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
		}
		else if (p->color != color) {

			if (Queen* t = dynamic_cast<Queen*>(p)) {//  opponent  Queen in our path
				*thretInPath = true;
				
			}
			if (Rook* t = dynamic_cast<Rook*>(p)) {//  opponent  Rook in our path
				*thretInPath = true;
			}
			ClegalMoves.insert(&Board::gameboxess[x][i]);
			break;
		}
		else if (p->color == color && (p != this)) {
			if (King* k = dynamic_cast<King*>(p)) {
				*kingInPath = true;
			}
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
		if (Board::gameboxess[x][i].getPiece() == NULL || dynamic_cast<King*>(Board::gameboxess[x][i].getPiece()) != nullptr) {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
		}
		else if (Board::gameboxess[x][i].getPiece() != this) {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
			break;
		}

	}
	return ClegalMoves;
}

std::set<Box*> Piece::rowMovs(int x, int y, int direction, bool* thretInPath, bool* kingInPath)
{
	std::set<Box*> RlegalMoves;
	int n = Board::rowBoxNmbersandCols;
	for (int i = x; i < n && i >= 0; i = i + direction)
	{
		Piece* p = Board::gameboxess[i][y].getPiece();

		if (p == NULL) {
			RlegalMoves.insert(&Board::gameboxess[i][y]);
		}
		else if (p->color != color) {
			if (Queen* t = dynamic_cast<Queen*>(p)) {//  opponent  Queen in our path
				*thretInPath = true;

			}
			if (Rook* t = dynamic_cast<Rook*>(p)) {//  opponent  Rook in our path
				*thretInPath = true;
			}
			RlegalMoves.insert(&Board::gameboxess[i][y]);
			break;
		}
		else if (p->color == color && (p != this)) {
			if (King* k = dynamic_cast<King*>(p)) {
				*kingInPath = true;
			}
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
		if (Board::gameboxess[i][y].getPiece() == NULL || dynamic_cast<King*>(Board::gameboxess[i][y].getPiece()) != nullptr) {
			RlegalMoves.insert(&Board::gameboxess[i][y]);
		}
		else if (Board::gameboxess[i][y].getPiece() != this) {
			RlegalMoves.insert(&Board::gameboxess[i][y]);
			break;
		}
	}
	return RlegalMoves;
}

std::set<Box*> Piece::checkDiagonal(int x, int y, int direction, int Idirection, bool* thretInPath, bool* kingInPath)
{
	int n = Board::rowBoxNmbersandCols;
	std::set<Box*> DlegalMoves;

	for (int i = x, j = y; i < n && j < n && i >= 0 && j >= 0; i = i + direction * Idirection, j = j + direction)
	{
		Piece* p = Board::gameboxess[i][j].getPiece();

		if (p == NULL) {
			DlegalMoves.insert(&Board::gameboxess[i][j]);
		}
		else if (p->color != color) {
			if (Queen* t = dynamic_cast<Queen*>(p)) {//  opponent  Queen in our path
				*thretInPath = true;
			}
			if (Rook* t = dynamic_cast<Rook*>(p)) {//  opponent  Rook in our path
				*thretInPath = true;
			}
			DlegalMoves.insert(&Board::gameboxess[i][j]);
			break;
		}
		else if (p->color == color && (p != this)) {
			if (King* k = dynamic_cast<King*>(p)) {
				*kingInPath = true;
			}
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
		if (Board::gameboxess[i][j].getPiece() == NULL || dynamic_cast<King*>(Board::gameboxess[i][j].getPiece()) != nullptr) {
			DlegalMoves.insert(&Board::gameboxess[i][j]);
		}
		else if (Board::gameboxess[i][j].getPiece() != this) {
			DlegalMoves.insert(&Board::gameboxess[i][j]);
			break;
		}
	}
	return DlegalMoves;
}

std::set<Box*> Piece::checkIfPinned(int x, int y)
{
	std::set<Box*> ClegalMoves;
	/*int direction = 1;
	int n = Board::rowBoxNmbersandCols;
	bool kingInPath = false;
	bool threatInPath = false;

	for (int i = y; i < n && i >= 0; i = i + direction)
	{
		Piece* p = Board::gameboxess[x][i].getPiece();

		if (p != NULL) {
			if (King* k = dynamic_cast<King*>(p)) {
				if (k->color == color) { // our king in our path
					kingInPath = true;
					direction = direction * (-1);
					i = y;
				}
			}
			if (Queen* t = dynamic_cast<Queen*>(p)) {
				if (t->color != color) { //  opponent  Queen in our path
					threatInPath = true;
					ClegalMoves.insert(&Board::gameboxess[x][i]);
					break;
				}
			}
			else if (Rook* t = dynamic_cast<Rook*>(p)) {
				if (t->color != color) { // opponent  Rook in our path
					threatInPath = true;
					ClegalMoves.insert(&Board::gameboxess[x][i]);
					break;
				}
			}
		}
		else if (p != this&&p!=NULL) {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
			break;

		}
		else if (p==NULL) {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
		}

	}*/
	return ClegalMoves;
}
