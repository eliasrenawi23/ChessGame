#pragma once
#include "Piece.h"
class Queen :
    public Piece
{

public:
    Queen(Box *loc, PlayerColor color);
    void moveAndTake() override;

    ~Queen();
};

