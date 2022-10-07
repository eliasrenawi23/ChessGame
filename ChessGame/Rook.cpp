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


std::set<Box*>  Rook::moveAndTake()
{
	std::cout << "Bishop clicked" << std::endl;
	int x = (location->x) / Board::BoxWidthandHigth;
	int y = (location->y) / Board::BoxWidthandHigth;
	std::set<Box*>  legalMoves;
	legalMoves.insert(location);
	std::set<Box*>  rowPart1;
	std::set<Box*>  rowPart2;

	rowPart1 = rowMovs(x, y, 1);
	rowPart2 = rowMovs(x, y, -1);


	rowPart1.insert(rowPart2.begin(), rowPart2.end());

	std::set<Box*>  colPart1;
	std::set<Box*>  colPart2;

	colPart1 = colMovs(x, y, 1);
	colPart2 = colMovs(x, y, -1);
	colPart1.insert(colPart2.begin(), colPart2.end());

	legalMoves.insert( rowPart1.begin(), rowPart1.end());
	legalMoves.insert( colPart1.begin(), colPart1.end());
	return legalMoves;

}


Rook::~Rook()
{
}

