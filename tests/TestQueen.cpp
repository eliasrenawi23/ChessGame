#include <gtest/gtest.h>
#include "Board.h"
#include "Queen.h"

class QueenTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
        board.clear();
    }
};

TEST_F(QueenTest, AllDirections) {
    // Queen at d4 (3,3)
    // Rook moves: 14
    // Bishop moves: 13
    // Total 27
    
    board.loadFEN("8/8/8/3Q4/8/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][3].getPiece();
    Queen* q = dynamic_cast<Queen*>(p);
    
    EXPECT_EQ(q->moveAndTake().size(), 27);
}
