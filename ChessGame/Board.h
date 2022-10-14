#pragma once
#include <SDL.h>
#include "Box.h"
#include "Player.h"
#include "PlayerColor.h"
#include <vector>
#include <set>


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
	bool playerTurn; //false for black and true for white
	std::set<Box*> boxtoLight;
	Box* selectedBox;
	bool checkmate;
	Pawn* En_passantPawn;
public:
	Board();
	void getLegalMovs(int cor_x, int cor_y);
	void play(int cor_x, int cor_y);
	void RenderBoard();//torendr all of the boxes 
	void RenderPieces();//torendr all of the pieces 
	void highlightboxs(bool onOrOff);

	void init();//init all of the boxes 
	void resize();
	~Board();
private:
	void checkresult();
	void CastleMove(int new_x,int new_y);
	void En_passant(int new_x,int new_y);
	void highlightKing();
	void deletepiece(Piece * p);

	void UpdatePieceLocation(Box *from,Box *to);

};

