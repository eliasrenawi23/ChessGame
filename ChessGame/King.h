#pragma once
#include "Piece.h"
class King :
    public Piece
{

public:
    King(Box* loc, PlayerColor color);
    void moveAndTake() override;

    ~King();
};

