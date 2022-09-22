#pragma once
#include "Piece.h"
class Knight :
    public Piece
{

public:
    Knight(Box* loc, PlayerColor color);
    void moveAndTake() override;

    ~Knight();
};

