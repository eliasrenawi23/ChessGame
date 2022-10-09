#pragma once
#include "Piece.h"
class King :
    public Piece
{

public:
    King(Box* loc, PlayerColor color);
    std::set<Box*> moveAndTake() override;
    std::set<Box*>PieceThreatMap()override;


    ~King();
};

