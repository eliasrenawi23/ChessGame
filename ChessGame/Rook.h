#pragma once
#include "Piece.h"
class Rook :
    public Piece
{

public:
    Rook(Box* loc, PlayerColor color);
    std::set<Box*>  moveAndTake() override;
    ~Rook();

};

