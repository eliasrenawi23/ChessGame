#pragma once
#include "Piece.h"


class Pawn :
    public Piece
{

public:
    bool PoosblieEnPassant;
public:
    Pawn(Box *loc, PlayerColor color);
    std::set<Box*> moveAndTake() override;
    std::set<Box*>PieceThreatMap(bool* checkmate)override;
    ~Pawn();
private:
    Box* CheckEnPassant();

};

