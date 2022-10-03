#pragma once
#include "Piece.h"
class Rook :
    public Piece
{

public:
    Rook(Box* loc, PlayerColor color);
    std::vector<Box*> moveAndTake() override;
    ~Rook();
};

