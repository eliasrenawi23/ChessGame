#include "Pawn.h"
#include "Window.h"
#include <algorithm> 


Pawn::Pawn(Box* loc, PlayerColor color) :PossibleEnPassant(false)
{
	this->location = loc;
	this->x = (location->x) / Board::BoxWidthandHigth;
	this->y = (location->y) / Board::BoxWidthandHigth;
	this->color = color;

	SDL_Surface* surface = (color == PlayerColor::BLACK)? IMG_Load("texture/BP.svg"): IMG_Load("texture/WP.svg");
	texture = SDL_CreateTextureFromSurface(Window::m_renderer, surface);
	SDL_DestroySurface(surface);

}
std::set<Box*>  Pawn::moveAndTake()
{
	std::cout << "Pawn clicked" << std::endl;
	int direction = (color == PlayerColor::WHITE) ? -1 : 1;//if white move -1 id black move 1
	int n = Board::rowBoxNmbersandCols;

	std::set<Box*>  legalMoves;

	//to do (must check for en passant) and pawn promotion
	
	// One forward
	if (Board::isInBounds(x, y + direction) &&
		Board::gameboxess[x][y + direction].getPiece() == nullptr) {
		legalMoves.insert(&Board::gameboxess[x][y + direction]);

		// Two forward on first move
		if (firstMove && Board::isInBounds(x, y + 2 * direction) &&
			Board::gameboxess[x][y + 2 * direction].getPiece() == nullptr) {
			legalMoves.insert(&Board::gameboxess[x][y + 2 * direction]);
		}
	}

	// Captures
	for (int dx : {-1, 1}) {
		int nx = x + dx;
		int ny = y + direction;
		if (Board::isInBounds(nx, ny)) {
			Piece* target = Board::gameboxess[nx][ny].getPiece();
			if (target != nullptr && target->getColor() != color) {
				legalMoves.insert(&Board::gameboxess[nx][ny]);
			}
		}
	}



	// En Passant
	Box* enPassantBox = CheckEnPassant();
	if (enPassantBox != nullptr) {
		legalMoves.insert(enPassantBox);
	}

	legalMoves = checkpinned(legalMoves);

	return legalMoves;
}
Box* Pawn::CheckEnPassant() {
    int direction = (color == PlayerColor::WHITE) ? -1 : 1;
    int n = Board::rowBoxNmbersandCols;

    for (int dx : {-1, 1}) {
        int nx = x + dx;
        int ny = y + direction;

        if (Board::isInBounds(nx, ny) &&
            Board::gameboxess[nx][ny].getPiece() == nullptr) {
            Piece* p = Board::gameboxess[nx][y].getPiece();
            if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
                if (pawn->getColor() != color && pawn->PossibleEnPassant) {
                    return &Board::gameboxess[nx][ny];
                }
            }
        }
    }

    return nullptr;
}



std::set<Box*> Pawn::PieceThreatMap(bool* checkmateh) {
	int direction = (color == PlayerColor::WHITE) ? -1 : 1;
	int n = Board::rowBoxNmbersandCols;
	std::set<Box*> legalMoves;

	for (int dx : {-1, 1}) {
		int nx = x + dx;
		int ny = y + direction;

		if (Board::isInBounds(nx, ny)) {
			Piece* p = Board::gameboxess[nx][ny].getPiece();
			if (King* t = dynamic_cast<King*>(p)) {
				if (t->getColor() != color) {
					*checkmateh = true;
				}
			}
			legalMoves.insert(&Board::gameboxess[nx][ny]);
		}
	}

	return legalMoves;
}


Pawn::~Pawn()
{

}
