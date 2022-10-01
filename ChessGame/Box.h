#pragma once
#include <SDL.h>

class Piece;// forward declaration

struct Box
{
	int x;
	int y;
	int size;
	SDL_Color boxColor; 
	Piece* piece;
	Box();
	void setPiece(Piece* p);
	Box(int x, int y, int size, SDL_Color boxColor);
	void resize(int x, int y, int size);
	~Box();


};



