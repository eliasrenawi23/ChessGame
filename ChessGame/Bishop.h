#pragma once
#include "Piece.h"
class Bishop :
    public Piece
{


public:
    Bishop(Box* loc);
    void moveAndTake() override;
    void render()override;

    ~Bishop();
};

