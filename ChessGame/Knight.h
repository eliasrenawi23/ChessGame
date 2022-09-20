#pragma once
#include "Piece.h"
class Knight :
    public Piece
{

public:
    Knight(Box* loc);
    void moveAndTake() override;
    ~Knight();
};

