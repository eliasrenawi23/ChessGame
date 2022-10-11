#pragma once
#include "Piece.h"
class Knight :
    public Piece
{

public:
    Knight(Box* loc, PlayerColor color);
    std::set<Box*> moveAndTake() override;
    std::set<Box*>PieceThreatMap(bool* checkmate)override;

    ~Knight();
};

