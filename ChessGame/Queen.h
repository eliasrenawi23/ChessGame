#pragma once
#include "Piece.h"
class Queen :
    public Piece
{

public:
    Queen(int x, int y);
    void moveAndTake() override;
    ~Queen();
};

