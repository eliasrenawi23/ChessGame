#pragma once
#include "Piece.h"
class King :
    public Piece
{

public:
    King(Box* loc);
    void moveAndTake() override;
    void render()override;

    ~King();
};

