#pragma once
#include "Box.h"
#include <vector>
#include "PlayerColor.h"
#include <SDL_image.h>
#include <iostream>
#include <set>



class Piece
{

public:

	Box *location;
	PlayerColor color;
	SDL_Texture* texture;
	bool firstMove;
public:
	Piece();
	Box* getLocation();
	void setLocation(Box* loc);
	virtual std::set<Box*> moveAndTake() = 0;
	void renderPiece();
	virtual ~Piece() ;
protected:
	std::set<Box*>colMovs(int x, int y, int direction);
	std::set<Box*> rowMovs(int x, int y, int direction);
	std::set<Box*> checkDiagonal(int x, int y, int direction, int Idirection);
	bool ifInvokeCheckmate(int x, int y);

};



