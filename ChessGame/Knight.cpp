#include "Knight.h"
#include "Window.h"

Knight::Knight(Box* loc, PlayerColor color)
{
	this->location = loc;
	this->x = (location->x) / Board::BoxWidthandHigth;
	this->y = (location->y) / Board::BoxWidthandHigth;
	this->color = color;
	SDL_Surface* surface = IMG_Load(color == PlayerColor::BLACK ? "texture/BN.svg" : "texture/WN.svg");
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_DestroySurface(surface);
}



std::set<Box*> Knight::moveAndTake() {
	std::cout << "Knight clicked" << std::endl;
	std::set<Box*> legalMoves;

	const std::vector<std::pair<int, int>> offsets = {
		{-1, -2}, {+1, -2}, {-1, +2}, {+1, +2},
		{-2, -1}, {-2, +1}, {+2, -1}, {+2, +1}
	};

	for (auto move : offsets) {
		int newX = x + move.first, newY = y + move.second;
		if (Board::isInBounds(newX, newY)) {
			Piece* target = Board::gameboxess[newX][newY].getPiece();
			if (!target || target->getColor() != color) {
				legalMoves.insert(&Board::gameboxess[newX][newY]);
			}
		}
	}

	return checkpinned(legalMoves);
}

std::set<Box*> Knight::PieceThreatMap(bool* checkmate) {
	std::set<Box*> threatMap;

	const std::vector<std::pair<int, int>> offsets = {
		{-1, -2}, {+1, -2}, {-1, +2}, {+1, +2},
		{-2, -1}, {-2, +1}, {+2, -1}, {+2, +1}
	};

	for (auto move : offsets) {
		int newX = x + move.first, newY = y + move.second;
		if (Board::isInBounds(newX, newY)) {
			changecheckmateInThreatMap(newX, newY, checkmate);
			threatMap.insert(&Board::gameboxess[newX][newY]);
		}
	}

	return threatMap;
}

Knight::~Knight()
{
}
