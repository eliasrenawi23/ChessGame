#pragma once
#include "Piece.h"
class Queen :
    public Piece
{

public:
    Queen(Box *loc, PlayerColor color);
    std::set<Box*>  moveAndTake() override;
    std::set<Box*>PieceThreatMap()override;

    ~Queen();
};

