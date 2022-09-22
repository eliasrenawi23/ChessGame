#pragma once
#include "Piece.h"
class Bishop :
    public Piece
{


public:
    Bishop(Box* loc, PlayerColor color);
    void moveAndTake() override;

    ~Bishop();
};

