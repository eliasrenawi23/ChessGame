#pragma once
#include "Piece.h"
class Rook :
    public Piece
{

public:
    Rook(int x, int y);
    void moveAndTake() override;
    ~Rook();
};
