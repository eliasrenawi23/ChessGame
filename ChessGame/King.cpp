#include "King.h"
#include "Window.h"
#include <algorithm> 




King::King(Box* loc, PlayerColor color)
{
	this->location = loc;
	this->x = (location->x) / Board::BoxWidthandHigth;
	this->y = (location->y) / Board::BoxWidthandHigth;
	this->color = color;
	SDL_Surface* surface = IMG_Load((color == PlayerColor::BLACK) ? "texture/BK.svg" : "texture/WK.svg");

	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_DestroySurface(surface);
}



std::set<Box*>  King::moveAndTake()
{
	std::cout << "King clicked" << std::endl;

	int n = Board::rowBoxNmbersandCols;
	std::set<Box*> legalMoves{ location };


	for (int i = x - 1; i < n && i < (x + 2); i++) {
		if (i < 0)continue;
		for (int j = y - 1; j < n && j < (y + 2); j++) {
			if (j < 0)continue;
			 Piece* piece = Board::gameboxess[i][j].getPiece();
            if (!piece || piece->getColor() != color) {
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

std::set<Box*> King::getCoverPath() {
	std::cout << "King::getCoverPath" << std::endl;
	bool thretInPath = false, kingInPath = false;
	std::set<Box*> coverPath;

	std::vector<std::set<Box*>> directions = {
		checkDiagonal(x, y, 1, 1, &thretInPath, &kingInPath),
		checkDiagonal(x, y, -1, 1, &thretInPath, &kingInPath),
		checkDiagonal(x, y, 1, -1, &thretInPath, &kingInPath),
		checkDiagonal(x, y, -1, -1, &thretInPath, &kingInPath),
		rowMovs(x, y, 1, &thretInPath, &kingInPath),
		rowMovs(x, y, -1, &thretInPath, &kingInPath),
		colMovs(x, y, 1, &thretInPath, &kingInPath),
		colMovs(x, y, -1, &thretInPath, &kingInPath)
	};

	for (const auto& dir : directions) {
		if (thretInPath) return dir;
		thretInPath = false;
	}

	if (Box* knightThreat = checkKinghtCheckmate()) {
		coverPath.insert(knightThreat);
	}

	return coverPath;
}

Box* King::checkKinghtCheckmate() {
	const int n = Board::rowBoxNmbersandCols;
	const std::vector<std::pair<int, int>> knightMoves = {
		{-1, -2}, {1, -2}, {-1, 2}, {1, 2},
		{-2, -1}, {-2, 1}, {2, -1}, {2, 1}
	};

	for (const auto& move : knightMoves) {
		int nx = x + move.first, ny = y + move.second;
		if (nx >= 0 && ny >= 0 && nx < n && ny < n) {
			Piece* p = Board::gameboxess[nx][ny].getPiece();
			if (p && p->getColor() != color && dynamic_cast<Knight*>(p)) {
				return &Board::gameboxess[nx][ny];
			}
		}
	}
	return nullptr;
}

std::set<Box*> King::checkCastle() {
	if (!firstMove) return {};
	std::set<Box*> castleMoves;
	int y_cor = (color == PlayerColor::WHITE) ? 0 : 7;

	auto checkRook = [&](int rookX, std::vector<int> emptyXs, int targetX) {
		Piece* rook = Board::gameboxess[rookX][y].getPiece();
		if (rook && rook->getColor() == color && dynamic_cast<Rook*>(rook) && rook->firstMove) {
			bool pathClear = std::all_of(emptyXs.begin(), emptyXs.end(), [&](int xPos) {
				return Board::gameboxess[xPos][y].getPiece() == nullptr;
				});
			if (pathClear) castleMoves.insert(&Board::gameboxess[targetX][y]);
		}
		};

	checkRook(0, { 1, 2, 3 }, 2);
	checkRook(7, { 5, 6 }, 6);

	return castleMoves;

}


King::~King()
{
}
