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

std::set<Box*> Piece::checkpinned(std::set<Box*> legalMoves) {
	std::set<Box*> pinnedMoves;
	bool threatInPath = false, kingInPath = false;

	// Check for diagonal threats
	std::set<Box*> diag1Part1 = checkDiagonal(x, y, 1, 1, &threatInPath, &kingInPath);
	std::set<Box*> diag1Part2 = checkDiagonal(x, y, -1, 1, &threatInPath, &kingInPath);
	diag1Part1.insert(diag1Part2.begin(), diag1Part2.end());

	// If a threat is detected, and the king is also in the path, check for pinned piece
	if (threatInPath && kingInPath) {
		std::set_intersection(legalMoves.begin(), legalMoves.end(), diag1Part1.begin(), diag1Part1.end(),
			std::inserter(pinnedMoves, pinnedMoves.begin()));
		return pinnedMoves;
	}

	// Reset flags and check the other diagonal
	kingInPath = threatInPath = false;
	std::set<Box*> diag2Part1 = checkDiagonal(x, y, 1, -1, &threatInPath, &kingInPath);
	std::set<Box*> diag2Part2 = checkDiagonal(x, y, -1, -1, &threatInPath, &kingInPath);
	diag2Part1.insert(diag2Part2.begin(), diag2Part2.end());

	if (threatInPath && kingInPath) {
		std::set_intersection(legalMoves.begin(), legalMoves.end(), diag2Part1.begin(), diag2Part1.end(),
			std::inserter(pinnedMoves, pinnedMoves.begin()));
		return pinnedMoves;
	}

	// Reset flags and check for row threats
	kingInPath = threatInPath = false;
	std::set<Box*> rowPart1 = rowMovs(x, y, 1, &threatInPath, &kingInPath);
	std::set<Box*> rowPart2 = rowMovs(x, y, -1, &threatInPath, &kingInPath);
	rowPart1.insert(rowPart2.begin(), rowPart2.end());

	if (threatInPath && kingInPath) {
		return pinnedMoves; // No legal moves if the piece is pinned in the row
	}

	// Reset flags and check for column threats
	kingInPath = threatInPath = false;
	std::set<Box*> colPart1 = colMovs(x, y, 1, &threatInPath, &kingInPath);
	std::set<Box*> colPart2 = colMovs(x, y, -1, &threatInPath, &kingInPath);
	colPart1.insert(colPart2.begin(), colPart2.end());

	if (threatInPath && kingInPath) {
		std::set_intersection(legalMoves.begin(), legalMoves.end(), colPart1.begin(), colPart1.end(),
			std::inserter(pinnedMoves, pinnedMoves.begin()));
		return pinnedMoves;
	}

	return legalMoves; // Return all legal moves if not pinned
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
