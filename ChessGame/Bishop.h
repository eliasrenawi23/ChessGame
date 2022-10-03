#pragma once
#include "Piece.h"
class Bishop :
    public Piece
{


public:
    Bishop(Box* loc, PlayerColor color);
    std::vector<Box*> moveAndTake() override;

    ~Bishop();
};

