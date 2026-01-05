#include <gtest/gtest.h>
#include "Board.h"
#include "Rook.h"

class RookTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
        board.clear();
    }
};

TEST_F(RookTest, SlidingMovement) {
    // Rook at d4 (3,3)
    // Row 3: 7 other squares
    // Col 3: 7 other squares
    // Total 14 moves on empty board
    
    board.loadFEN("8/8/8/3R4/8/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][3].getPiece();
    Rook* r = dynamic_cast<Rook*>(p);
    
    EXPECT_EQ(r->moveAndTake().size(), 14);
}

TEST_F(RookTest, BlockAndCapture) {
    // Rook at d4 (3,3)
    // Friend at d2 (3,1) (Up in FEN sense? 1 is higher up than 3)
    // Enemy at g4 (6,3) (Right)
    
    // FEN construction:
    // Row 1: ... P ... (d2) -> "3P4"
    // Row 3: ... R .. p (d4, g4) -> "3R2p1"
    
    board.loadFEN("8/3P4/8/3R2p1/8/8/8/8 w - - 0 1");
    // Verify pieces
    EXPECT_NE(Board::gameboxess[3][1].getPiece(), nullptr); // Friend P
    EXPECT_NE(Board::gameboxess[6][3].getPiece(), nullptr); // Enemy p
    
    Piece* p = Board::gameboxess[3][3].getPiece();
    Rook* r = dynamic_cast<Rook*>(p);
    
    std::set<Box*> moves = r->moveAndTake();
    
    // Should NOT contain d2 (3,1)
    // Should contain g4 (6,3)
    // Should NOT contain h4 (7,3) (past enemy)
    
    bool hitFriend = false;
    bool hitEnemy = false;
    bool pastEnemy = false;
    
    for(auto m : moves) {
        int c = m->x / Board::BoxWidthandHigth;
        int row = m->y / Board::BoxWidthandHigth;
        
        if (c == 3 && row == 1) hitFriend = true;
        if (c == 6 && row == 3) hitEnemy = true;
        if (c == 7 && row == 3) pastEnemy = true;
    }
    
    EXPECT_FALSE(hitFriend);
    EXPECT_TRUE(hitEnemy);
    EXPECT_FALSE(pastEnemy);
}
