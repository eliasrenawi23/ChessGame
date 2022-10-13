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
	bool firstMove;

protected:
	PlayerColor color;
	Box* location;
	int x, y;
	SDL_Texture* texture;

public:
	Piece();
	Box* getLocation();
	void setLocation(Box* loc);
	virtual std::set<Box*> moveAndTake() = 0;
	virtual std::set<Box*>PieceThreatMap(bool* checkmate) = 0;
	PlayerColor getColor();
	void renderPiece();
	virtual ~Piece();
protected:
	std::set<Box*>colMovs(int x, int y, int direction, bool* thretInPath, bool* kingInPath);
	std::set<Box*> rowMovs(int x, int y, int direction, bool* thretInPath, bool* kingInPath);
	std::set<Box*> checkDiagonal(int x, int y, int direction, int Idirection, bool* thretInPath, bool* kingInPath);

	std::set<Box*>colThreatMap(int x, int y, int direction, bool* checkmate);
	std::set<Box*> rowThreatMap(int x, int y, int direction, bool* checkmate);
	std::set<Box*> DiagonalThreatMap(int x, int y, int direction, int Idirection, bool* checkmate);


	std::set<Box*> checkpinned(std::set<Box*> legalMoves);
	bool changecheckmateInThreatMap(int x, int y, bool* checkmate);

};



