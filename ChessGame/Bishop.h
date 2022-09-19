#pragma once
#include "Piece.h"
class Bishop :
    public Piece
{


public:
    Bishop(int x, int y);
    void moveAndTake() override;
    ~Bishop();
};

