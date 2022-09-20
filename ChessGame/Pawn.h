#pragma once
#include "Piece.h"
class Pawn :
    public Piece
{


public:
    Pawn(Box *loc);
     void moveAndTake() override;
     ~Pawn();
};

