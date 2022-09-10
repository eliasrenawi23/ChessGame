#pragma once
#include <SDL.h>

class Box
{
public:
	int x;
	int y;
	int size;
	SDL_Color boxColor; 
public:
	Box();
	Box(int x, int y, int size, SDL_Color boxColor);
	void resize(int x, int y, int size);
	void RenderBox();//torendr all of the boxes 
	~Box();


};

