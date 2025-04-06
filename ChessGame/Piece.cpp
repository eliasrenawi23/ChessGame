#include "Piece.h"
#include "Window.h"
#include <algorithm> 

Piece::Piece() : location(nullptr), texture(nullptr), firstMove(true) {}


Box* Piece::getLocation() const
{
	return location;
}

void Piece::setLocation(Box* loc) {
	location = loc;
	x = location->x / Board::BoxWidthandHigth;
	y = location->y / Board::BoxWidthandHigth;
	firstMove = false;
}


PlayerColor Piece::getColor() const {
	return color;
}


void Piece::renderPiece() const {

	if (!texture) {
		std::cout << "Texture not initialized!" << std::endl;
		return;
	}
	float w, h;
	if (!SDL_GetTextureSize(texture, &w, &h)) {
		std::cout << "SDL_GetTextureSize failed: " << SDL_GetError() << std::endl;
		return;
	}

	SDL_FRect fromRect = { 0, 0,w, h };
	SDL_FRect toRect = { location->x, location->y, location->size, location->size };

	if (!SDL_RenderTexture(Window::m_renderer, texture, &fromRect, &toRect)) {
		std::cout << "SDL_RenderTexture failed: " << SDL_GetError() << std::endl;
	}
}

Piece::~Piece() {
	location = nullptr;
	if (texture) {
		SDL_DestroyTexture(texture);
	}
}

std::set<Box*> Piece::colMovs(int x, int y, int direction, bool* thretInPath, bool* kingInPath) {
	std::set<Box*> legalMoves;
	int n = Board::rowBoxNmbersandCols;

	for (int i = y; i < n && i >= 0; i += direction) {
		Piece* p = Board::gameboxess[x][i].getPiece();
		if (p == nullptr) {
			legalMoves.insert(&Board::gameboxess[x][i]);
		}
		else if (p->color != color) {
			if (dynamic_cast<Queen*>(p) || dynamic_cast<Rook*>(p)) {
				*thretInPath = true;
			}
			legalMoves.insert(&Board::gameboxess[x][i]);
			break;
		}
		else if (p->color == color && p != this) {
			if (dynamic_cast<King*>(p)) {
				*kingInPath = true;
			}
			break;
		}
	}

	return legalMoves;
}

std::set<Box*> Piece::colThreatMap(int x, int y, int direction, bool* checkmate) {
	std::set<Box*> legalMoves;
	int n = Board::rowBoxNmbersandCols;

	for (int i = y; i < n && i >= 0; i += direction) {
		Piece* p = Board::gameboxess[x][i].getPiece();
		if (p == nullptr) {
			legalMoves.insert(&Board::gameboxess[x][i]);
		}
		else if (changecheckmateInThreatMap(x, i, checkmate)) {
			legalMoves.insert(&Board::gameboxess[x][i]);
		}
		else if (p != this) {
			legalMoves.insert(&Board::gameboxess[x][i]);
			break;
		}
	}

	return legalMoves;
}

std::set<Box*> Piece::rowMovs(int x, int y, int direction, bool* thretInPath, bool* kingInPath) {
	std::set<Box*> legalMoves;
	int n = Board::rowBoxNmbersandCols;

	for (int i = x; i < n && i >= 0; i += direction) {
		Piece* p = Board::gameboxess[i][y].getPiece();
		if (p == nullptr) {
			legalMoves.insert(&Board::gameboxess[i][y]);
		}
		else if (p->color != color) {
			if (dynamic_cast<Queen*>(p) || dynamic_cast<Rook*>(p)) {
				*thretInPath = true;
			}
			legalMoves.insert(&Board::gameboxess[i][y]);
			break;
		}
		else if (p->color == color && p != this) {
			if (dynamic_cast<King*>(p)) {
				*kingInPath = true;
			}
			break;
		}
	}

	return legalMoves;
}

// --- ROW THREAT MAP ---
std::set<Box*> Piece::rowThreatMap(int x, int y, int direction, bool* checkmate) {
	std::set<Box*> legalMoves;
	int n = Board::rowBoxNmbersandCols;

	for (int i = x; i < n && i >= 0; i += direction) {
		Piece* p = Board::gameboxess[i][y].getPiece();
		if (p == nullptr) {
			legalMoves.insert(&Board::gameboxess[i][y]);
		}
		else if (changecheckmateInThreatMap(i, y, checkmate)) {
			legalMoves.insert(&Board::gameboxess[i][y]);
		}
		else if (p != this) {
			legalMoves.insert(&Board::gameboxess[i][y]);
			break;
		}
	}
	return legalMoves;
}

std::set<Box*> Piece::checkDiagonal(int x, int y, int direction, int Idirection, bool* thretInPath, bool* kingInPath) {
	std::set<Box*> legalMoves;
	int n = Board::rowBoxNmbersandCols;

	for (int i = x, j = y; i < n && j < n && i >= 0 && j >= 0; i += direction * Idirection, j += direction) {
		Piece* p = Board::gameboxess[i][j].getPiece();
		if (p == nullptr) {
			legalMoves.insert(&Board::gameboxess[i][j]);
		}
		else if (p->color != color) {
			if (dynamic_cast<Queen*>(p) || dynamic_cast<Bishop*>(p)) {
				*thretInPath = true;
			}
			legalMoves.insert(&Board::gameboxess[i][j]);
			break;
		}
		else if (p->color == color && p != this) {
			if (dynamic_cast<King*>(p)) {
				*kingInPath = true;
			}
			break;
		}
	}

	return legalMoves;
}

// --- DIAGONAL THREAT MAP ---
std::set<Box*> Piece::DiagonalThreatMap(int x, int y, int direction, int Idirection, bool* checkmate) {
	std::set<Box*> legalMoves;
	int n = Board::rowBoxNmbersandCols;
	for (int i = x, j = y; i < n && j < n && i >= 0 && j >= 0; i += direction * Idirection, j += direction) {
		Piece* p = Board::gameboxess[i][j].getPiece();
		if (p == nullptr) {
			legalMoves.insert(&Board::gameboxess[i][j]);
		}
		else if (changecheckmateInThreatMap(i, j, checkmate)) {
			legalMoves.insert(&Board::gameboxess[i][j]);
		}
		else if (p != this) {
			legalMoves.insert(&Board::gameboxess[i][j]);
			break;
		}
	}
	return legalMoves;
}


// --- CHECK PINNED ---
// This function returns only the moves that lie on the line between the piece and its own king 
// if it is pinned by an enemy piece.
std::set<Box*> Piece::checkpinned(std::set<Box*> legalMoves) {
	std::set<Box*> intersection;

	// Check Diagonals (both directions)
	{
		bool threat1 = false, king1 = false;
		std::set<Box*> diagA = checkDiagonal(x, y, 1, 1, &threat1, &king1);
		bool threat2 = false, king2 = false;
		std::set<Box*> diagB = checkDiagonal(x, y, -1, 1, &threat2, &king2);
		if ((threat1 && king1) || (threat2 && king2)) {
			std::set<Box*> diagCombined = diagA;
			diagCombined.insert(diagB.begin(), diagB.end());
			std::set_intersection(legalMoves.begin(), legalMoves.end(),
				diagCombined.begin(), diagCombined.end(),
				std::inserter(intersection, intersection.begin()));
			if (!intersection.empty())
				return intersection;
		}
	}

	// Check Row
	{
		bool threatPos = false, kingPos = false;
		std::set<Box*> rowPos = rowMovs(x, y, 1, &threatPos, &kingPos);
		bool threatNeg = false, kingNeg = false;
		std::set<Box*> rowNeg = rowMovs(x, y, -1, &threatNeg, &kingNeg);
		if ((threatPos && kingPos) || (threatNeg && kingNeg)) {
			std::set<Box*> rowCombined = rowPos;
			rowCombined.insert(rowNeg.begin(), rowNeg.end());
			std::set_intersection(legalMoves.begin(), legalMoves.end(),
				rowCombined.begin(), rowCombined.end(),
				std::inserter(intersection, intersection.begin()));
			if (!intersection.empty())
				return intersection;
		}
	}

	// Check Column
	{
		bool threatPos = false, kingPos = false;
		std::set<Box*> colPos = colMovs(x, y, 1, &threatPos, &kingPos);
		bool threatNeg = false, kingNeg = false;
		std::set<Box*> colNeg = colMovs(x, y, -1, &threatNeg, &kingNeg);
		if ((threatPos && kingPos) || (threatNeg && kingNeg)) {
			std::set<Box*> colCombined = colPos;
			colCombined.insert(colNeg.begin(), colNeg.end());
			std::set_intersection(legalMoves.begin(), legalMoves.end(),
				colCombined.begin(), colCombined.end(),
				std::inserter(intersection, intersection.begin()));
			if (!intersection.empty())
				return intersection;
		}
	}

	// If no pin detected in any direction, return the original legal moves.

	return legalMoves;
}




bool Piece::changecheckmateInThreatMap(int x, int y, bool* checkmate) {
	if (King* king = dynamic_cast<King*>(Board::gameboxess[x][y].getPiece())) {
		if (king->getColor() != color) {
			*checkmate = true;
		}
		return true;
	}
	return false;
}
