#pragma once
#include "Piece.h"


class Pawn :
    public Piece
{

public:
    Pawn(Box *loc, PlayerColor color);
     void moveAndTake() override;
     ~Pawn();
};

