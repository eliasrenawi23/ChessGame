#pragma once
#include "Piece.h"


class Pawn :
    public Piece
{
public:
    Pawn(Box *loc, PlayerColor color);
    std::set<Box*> moveAndTake() override;
     //std::vector<Box*> movess();
    std::set<Box*>PieceThreatMap()override;
     ~Pawn();
};

