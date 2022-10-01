#pragma once
#include <SDL.h>

class Piece;// forward declaration

class Box
{
public:
	int x;
	int y;
	int size;
	SDL_Color boxColor; 

private:
	Piece* piece;
public:
	Box();
	void setPiece(Piece* p);
	Piece* getPiece();
	Box(int x, int y, int size, SDL_Color boxColor);
	void resize(int x, int y, int size);
	~Box();


};



