#pragma once
#include <SDL.h>

class Box
{
private:
	int x;
	int y;
	int width;
	int higth;
	SDL_Color boxColor; 
public:
	Box();
	Box(int x, int y, int width, int higth, SDL_Color boxColor);
	void RenderBox();//torendr all of the boxes 
	~Box();


};

