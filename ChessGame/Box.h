#pragma once
#include <SDL.h>

struct Box
{
	int x;
	int y;
	int size;
	SDL_Color boxColor; 
	Box();
	Box(int x, int y, int size, SDL_Color boxColor);
	void resize(int x, int y, int size);
	~Box();


};

