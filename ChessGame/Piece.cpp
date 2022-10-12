#include "Piece.h"
#include "Window.h"
#include <algorithm> 

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
	x = (location->x) / Board::BoxWidthandHigth;
	y= (location->y) / Board::BoxWidthandHigth;
	firstMove = false;
}


PlayerColor Piece::getColor()
{
	return this->color;
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

std::set<Box*> Piece::colMovs(int x, int y, int direction, bool* thretInPath, bool* kingInPath)
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

std::set<Box*> Piece::colThreatMap(int x, int y, int direction, bool* checkmate)
{
	std::set<Box*> ClegalMoves;
	int n = Board::rowBoxNmbersandCols;
	for (int i = y; i < n && i >= 0; i = i + direction)
	{
		Piece* p = Board::gameboxess[x][i].getPiece();
		if (p == NULL) {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
		}
		else if (changecheckmateInThreatMap(x, i, checkmate)) {
			ClegalMoves.insert(&Board::gameboxess[x][i]);
		}
		else if (p != this) {
			if (p->color != color && dynamic_cast<King*>(p) != nullptr) {
				*checkmate = true;
			}
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

std::set<Box*> Piece::rowThreatMap(int x, int y, int direction, bool* checkmate)
{
	std::set<Box*> RlegalMoves;
	int n = Board::rowBoxNmbersandCols;
	for (int i = x; i < n && i >= 0; i = i + direction)
	{
		Piece* p = Board::gameboxess[i][y].getPiece();

		if (p == NULL) {
			RlegalMoves.insert(&Board::gameboxess[i][y]);
		}
		else if (changecheckmateInThreatMap(i, y, checkmate)) {
			RlegalMoves.insert(&Board::gameboxess[i][y]);
		}
		else if (p != this) {
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
	bool notheing = false;
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
			if (Bishop* t = dynamic_cast<Bishop*>(p)) {//  opponent  Rook in our path
				*thretInPath = true;
			}
			if (Pawn* t = dynamic_cast<Pawn*>(p)) {//  opponent  Rook in our path
				std::set<Box*> PawnThrat= t->PieceThreatMap(&notheing);
				auto pos = PawnThrat.find(location);
				if (pos != PawnThrat.end()) {
					*thretInPath = true;
				}
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

std::set<Box*> Piece::DiagonalThreatMap(int x, int y, int direction, int Idirection, bool* checkmate)
{
	int n = Board::rowBoxNmbersandCols;
	std::set<Box*> DlegalMoves;

	for (int i = x, j = y; i < n && j < n && i >= 0 && j >= 0; i = i + direction * Idirection, j = j + direction)
	{
		Piece* p = Board::gameboxess[i][j].getPiece();

		if (p == NULL) {
			DlegalMoves.insert(&Board::gameboxess[i][j]);
		}
		else if (changecheckmateInThreatMap(i,j,checkmate)) {
			DlegalMoves.insert(&Board::gameboxess[i][j]);
		}
		else if (p != this) {
			DlegalMoves.insert(&Board::gameboxess[i][j]);
			break;
		}
	}
	return DlegalMoves;
}

std::set<Box*> Piece::checkpinned(std::set<Box*> legalMoves)
{
	std::set<Box*>  Diag1part1;
	std::set<Box*>  Diag1part2;
	
	bool thretInPath = false, kingInPath = false;//to check if the pice is pinned
	std::set<Box*> endres;
	Diag1part1 = checkDiagonal(x, y, 1, 1, &thretInPath, &kingInPath);
	Diag1part2 = checkDiagonal(x, y, -1, 1, &thretInPath, &kingInPath);
	Diag1part1.insert(Diag1part2.begin(), Diag1part2.end());
	if (thretInPath && kingInPath) {
		std::set_intersection(legalMoves.begin(), legalMoves.end(), Diag1part1.begin(), Diag1part1.end(),
			std::inserter(endres, endres.begin()));
		return endres;
	}

	kingInPath = thretInPath = false;
	endres.clear();

	std::set<Box*>  Diag2part1, Diag2part2;

	Diag2part1 = checkDiagonal(x, y, 1, -1, &thretInPath, &kingInPath);
	Diag2part2 = checkDiagonal(x, y, -1, -1, &thretInPath, &kingInPath);
	Diag2part1.insert(Diag2part2.begin(), Diag2part2.end());

	if (thretInPath && kingInPath) {
		std::set_intersection(legalMoves.begin(), legalMoves.end(), Diag2part1.begin(), Diag2part1.end(),
			std::inserter(endres, endres.begin()));
		return endres;

	}
	kingInPath = thretInPath = false;
	endres.clear();


	std::set<Box*>  rowPart1, rowPart2;

	rowPart1 = rowMovs(x, y, 1, &thretInPath, &kingInPath);
	rowPart2 = rowMovs(x, y, -1, &thretInPath, &kingInPath); //if thret in the row then cant move
	rowPart1.insert(rowPart2.begin(), rowPart2.end());

	if (thretInPath && kingInPath) {
		return endres;//return empty set
	}

	kingInPath = thretInPath = false;
	endres.clear();


	std::set<Box*>  colPart1,colPart2;

	colPart1 = colMovs(x, y, 1, &thretInPath, &kingInPath);
	colPart2 = colMovs(x, y, -1, &thretInPath, &kingInPath);
	colPart1.insert(colPart2.begin(), colPart2.end());
	if (thretInPath && kingInPath) {
		std::set_intersection(legalMoves.begin(), legalMoves.end(), colPart1.begin(), colPart1.end(),
			std::inserter(endres, endres.begin()));
		return endres;
	}
	return legalMoves;
}

bool Piece::changecheckmateInThreatMap(int x, int y, bool* checkmate)
{
	if (King* t = dynamic_cast<King*>(Board::gameboxess[x][y].getPiece())) {//  opponent  king in our path
		if (t->color != color) {
			*checkmate = true;
		}
		return true;
	}
	return false;
}

