#pragma once
#include "Piece.h"
class Queen :
    public Piece
{

public:
    Queen(Box *loc);
    void moveAndTake() override;
    ~Queen();
};

