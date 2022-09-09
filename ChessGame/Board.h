#pragma once
#include <SDL.h>
#include "Box.h"
#include <vector>


class Board
{
public:
	const static int rowBoxNmbersandCols = 8;   //the board is 8x8 boxes grid  
	const SDL_Color Color1 = { 0,0,0,SDL_ALPHA_OPAQUE };  //white (0,0,0)
	const SDL_Color Color2 = { 118,150, 86, SDL_ALPHA_OPAQUE }; //green(118,150,86)
private:

	//std::vector<std::vector<Box>> gameboard;
	Box** gameboard;


public:
	Board();
	void RenderBoard();//torendr all of the boxes 
	void init();//init all of the boxes 
	~Board();

};

