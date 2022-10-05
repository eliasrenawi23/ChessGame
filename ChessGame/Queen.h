#pragma once
#include "Piece.h"
class Queen :
    public Piece
{

public:
    Queen(Box *loc, PlayerColor color);
    std::vector<Box*> moveAndTake() override;
    ~Queen();
private:
    std::vector<Box*> checkDiagonal(int x, int y, int direction, int Idirection);
    std::vector<Box*> Row(int x, int y, int direction);
    std::vector<Box*> col(int x, int y, int direction);
};

