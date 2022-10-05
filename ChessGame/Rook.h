#pragma once
#include "Piece.h"
class Rook :
    public Piece
{

public:
    Rook(Box* loc, PlayerColor color);
    std::vector<Box*> moveAndTake() override;
    ~Rook();
private:
    std::vector<Box*> Row(int x, int y, int direction);
    std::vector<Box*> col(int x, int y, int direction);
};

