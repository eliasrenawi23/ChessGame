#pragma once
#include <SDL.h>
#include "Box.h"
#include "Player.h"
#include "PlayerColor.h"
#include <vector>


class Board
{
public:
	const static int rowBoxNmbersandCols = 8;   //the board is 8x8 boxes grid 
	static int BoxWidthandHigth;
	const  SDL_Color Color1 = { 255,255,255,SDL_ALPHA_OPAQUE };  //white (255,255,255)
	const  SDL_Color Color2 = { 118,150, 86, SDL_ALPHA_OPAQUE }; //green(118,150,86)
	static std::vector<std::vector<Box >> gameboxess;

private:
	Player* whitePlayer;
	Player* blackPlayer;

public:
	Board();
	void RenderBoard();//torendr all of the boxes 
	void RenderPieces();//torendr all of the pieces 

	void init();//init all of the boxes 
	void resize();
	~Board();
private:
	void RenderBox(Box* boxtorender);

};

