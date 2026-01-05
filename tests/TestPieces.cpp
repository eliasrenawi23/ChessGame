#include <gtest/gtest.h>
#include "Board.h"
#include "Pawn.h"
#include "Rook.h"

class PieceTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
    }
};

TEST_F(PieceTest, PawnInitialMoves) {
    // White pawn at col 0, row 6
    // Should be able to move to 0,5 (one step) and 0,4 (two steps) (Since white moves UP, y decreases?)
    // Wait, if Row 0 is top, Row 7 is bottom. White is at 6 (pawns) and 7 (pieces).
    // White moves from 6 -> 5 -> 4.
    
    Piece* p = Board::gameboxess[0][6].getPiece();
    ASSERT_NE(p, nullptr);
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    ASSERT_NE(pawn, nullptr);
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    bool foundOneStep = false;
    bool foundTwoStep = false;
    
    for (Box* b : moves) {
        // x is col, y is row.
        // One step: [0][5]
        // Two step: [0][4]
        if (b == &Board::gameboxess[0][5]) foundOneStep = true;
        if (b == &Board::gameboxess[0][4]) foundTwoStep = true;
    }
    
    EXPECT_TRUE(foundOneStep);
    EXPECT_TRUE(foundTwoStep);
}
