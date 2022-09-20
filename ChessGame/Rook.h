#pragma once
#include "Piece.h"
class Rook :
    public Piece
{

public:
    Rook(Box* loc);
    void moveAndTake() override;
    ~Rook();
};

