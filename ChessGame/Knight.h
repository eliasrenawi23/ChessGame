#pragma once
#include "Piece.h"
class Knight :
    public Piece
{

public:
    Knight(Box* loc, PlayerColor color);
    std::vector<Box*> moveAndTake() override;

    ~Knight();
};

