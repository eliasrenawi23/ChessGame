#include <gtest/gtest.h>
#include "Board.h"
#include "King.h"

class KingTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
        board.clear();
    }
};

TEST_F(KingTest, BasicMovement) {
    // King at d4 (3,3) -> 8 moves
    board.loadFEN("8/8/8/3K4/8/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][3].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    EXPECT_EQ(k->moveAndTake().size(), 8);
}

TEST_F(KingTest, CornerMovement) {
    // King at a1 (0,7) -> 3 moves
    board.loadFEN("8/8/8/8/8/8/8/K7 w - - 0 1");
    Piece* p = Board::gameboxess[0][7].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    EXPECT_EQ(k->moveAndTake().size(), 3);
}
