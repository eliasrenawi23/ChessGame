#pragma once
#include "Piece.h"
class Bishop :
    public Piece
{


public:
    Bishop(Box* loc, PlayerColor color);
    std::vector<Box*> moveAndTake() override;
    ~Bishop();
private:
    std::vector<Box*> checkDiagonal(int x, int y, int direction, int Idirection);
};

