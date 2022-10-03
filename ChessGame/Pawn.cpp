#include "Pawn.h"
#include "Window.h"


Pawn::Pawn(Box* loc, PlayerColor color):firstMove(true)
{
	location = loc;
	this->color = color;
	SDL_Surface* surface;

	if (color == PlayerColor::BLACK) {
		surface = IMG_Load("texture/BP.svg");
	}
	else {
		surface = IMG_Load("texture/WP.svg");
	}
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_FreeSurface(surface);

}
std::vector<Box*> Pawn::moveAndTake()
{
	std::cout << "Pawn clicked" << std::endl;
	int diraction = (color == PlayerColor::WHITE) ? 1 : -1;//if white move -1 id black move 1
	//std::vector<Box*> legalMoves;
	if (firstMove) {

		legalMoves.push_back(& Board::gameboxess[(location->x) / Board::BoxWidthandHigth][((location->y) / Board::BoxWidthandHigth) - 2*diraction]);
		legalMoves.push_back(& Board::gameboxess[(location->x) / Board::BoxWidthandHigth][((location->y) / Board::BoxWidthandHigth) - diraction]);

	}
	else
	{
		legalMoves.push_back(&Board::gameboxess[(location->x) / Board::BoxWidthandHigth][((location->y) / Board::BoxWidthandHigth) - diraction]);
	}
	//location = &Board::gameboxess[(location->x) / Board::BoxWidthandHigth][((location->y) / Board::BoxWidthandHigth)- diraction];
	return legalMoves;
}



Pawn::~Pawn()
{

}
