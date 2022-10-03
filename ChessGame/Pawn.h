#pragma once
#include "Piece.h"


class Pawn :
    public Piece
{
private:
    bool firstMove;
   // std::vector<Box*> legalmoves;

public:
    Pawn(Box *loc, PlayerColor color);
    std::vector<Box*> moveAndTake() override;
     //std::vector<Box*> movess();
     ~Pawn();
};

