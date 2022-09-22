#pragma once
#include "Piece.h"
class Rook :
    public Piece
{

public:
    Rook(Box* loc, PlayerColor color);
    void moveAndTake() override;
    ~Rook();
};

