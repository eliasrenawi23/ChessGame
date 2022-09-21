#pragma once
#include "Piece.h"

class Pawn :
    public Piece
{
public:
    static SDL_Texture* whitePawnTexture;

public:
    Pawn(Box *loc);
     void moveAndTake() override;
     void render()override;
     ~Pawn();
};

