#include "Bishop.h"
#include "Window.h"
#include <algorithm>



Bishop::Bishop(Box* loc, PlayerColor color)
{

	this->location = loc;
	this->x = (location->x) / Board::BoxWidthandHigth;
	this->y = (location->y) / Board::BoxWidthandHigth;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BB.svg");
	}
	else {
		surface = IMG_Load("texture/WB.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_DestroySurface(surface);
}


std::set<Box*> Bishop::moveAndTake()
{
    std::cout << "Bishop clicked" << std::endl;

    // Helper lambda to combine all diagonal moves.
    auto combineDiag = [&]() -> std::set<Box*> {
        bool d1a = false, d1b = false;
        std::set<Box*> diag1 = checkDiagonal(x, y, 1, 1, &d1a, &d1b);   // bottom-right
        bool d2a = false, d2b = false;
        std::set<Box*> diag2 = checkDiagonal(x, y, -1, 1, &d2a, &d2b);  // top-left
        bool d3a = false, d3b = false;
        std::set<Box*> diag3 = checkDiagonal(x, y, 1, -1, &d3a, &d3b);  // top-right
        bool d4a = false, d4b = false;
        std::set<Box*> diag4 = checkDiagonal(x, y, -1, -1, &d4a, &d4b); // bottom-left

        std::set<Box*> result;
        result.insert(diag1.begin(), diag1.end());
        result.insert(diag2.begin(), diag2.end());
        result.insert(diag3.begin(), diag3.end());
        result.insert(diag4.begin(), diag4.end());
        return result;
        };

	// Helper lambda to check horizontal pinning.
	auto isPinnedHorizontally = [&]() -> bool {
		bool threatRowPos = false, kingRowPos = false;
		std::set<Box*> rowPos = rowMovs(x, y, 1, &threatRowPos, &kingRowPos);
		bool threatRowNeg = false, kingRowNeg = false;
		std::set<Box*> rowNeg = rowMovs(x, y, -1, &threatRowNeg, &kingRowNeg);
		// For a horizontal pin, the enemy threat must be on one side and the friendly king on the opposite side.
		return (threatRowPos && kingRowNeg) || (threatRowNeg && kingRowPos);
		};

	// Helper lambda to check vertical pinning.
	auto isPinnedVertically = [&]() -> bool {
		bool threatColPos = false, kingColPos = false;
		std::set<Box*> colPos = colMovs(x, y, 1, &threatColPos, &kingColPos);
		bool threatColNeg = false, kingColNeg = false;
		std::set<Box*> colNeg = colMovs(x, y, -1, &threatColNeg, &kingColNeg);
		// For a vertical pin, the enemy threat must be on one side and the king on the opposite side.
		return (threatColPos && kingColNeg) || (threatColNeg && kingColPos);
		};
	std::set<Box*> bishopDiagonalMoves = combineDiag();
	// If the bishop is pinned (horizontally or vertically), no diagonal move is legal.
	if (isPinnedHorizontally() || isPinnedVertically())
	{
		// Return an empty set to prohibit any move.
		return std::set<Box*>();
	}
	else
	{
		return bishopDiagonalMoves;
	}

}


std::set<Box*> Bishop::PieceThreatMap(bool* checkmate)
{
	
	std::set<Box*>  legalMoves;
	std::set<Box*>  Diag1part1;
	std::set<Box*>  Diag1part2;

	Diag1part1 = DiagonalThreatMap(x, y, 1, 1, checkmate);
	Diag1part2 = DiagonalThreatMap(x, y, -1, 1, checkmate);
	Diag1part1.insert(Diag1part2.begin(), Diag1part2.end());

	std::set<Box*>  Diag2part1;
	std::set<Box*>  Diag2part2;

	Diag2part1 = DiagonalThreatMap(x, y, 1, -1, checkmate);
	Diag2part2 = DiagonalThreatMap(x, y, -1, -1, checkmate);
	Diag2part1.insert(Diag2part2.begin(), Diag2part2.end());

	legalMoves.insert(Diag1part1.begin(), Diag1part1.end());
	legalMoves.insert(Diag2part1.begin(), Diag2part1.end());

	return legalMoves;
}

Bishop::~Bishop()
{
}



