#pragma once
#include "Piece.h"
class Knight :
    public Piece
{

public:
    Knight(int x, int y);
    void moveAndTake() override;
    ~Knight();
};

