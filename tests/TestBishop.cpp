#include <gtest/gtest.h>
#include "Board.h"
#include "Bishop.h"

class BishopTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
        board.clear();
    }
};

TEST_F(BishopTest, UnobstructedMovement) {
    // Bishop at d4 (3,3)
    // Diagonals: 
    // TL: c3, b2, a1 (3 squares)
    // TR: e3, f2, g1 (3 squares)
    // BL: c5, b6, a7 (3 squares)
    // BR: e5, f6, g7, h8 (4 squares)
    // Total 13 moves.
    
    board.loadFEN("8/8/8/3B4/8/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][3].getPiece();
    ASSERT_NE(p, nullptr);
    Bishop* b = dynamic_cast<Bishop*>(p);
    
    std::set<Box*> moves = b->moveAndTake();
    EXPECT_EQ(moves.size(), 13);
}

TEST_F(BishopTest, BlockedAndCapture) {
    // Bishop at d4 (3,3)
    // Blocked by friend at e5 (4,4)
    // Capture enemy at c3 (2,2)
    
    board.loadFEN("8/8/2p5/3B4/4P3/8/8/8 w - - 0 1");
    // e5 is 4,4 (row 4)? 
    // FEN: "4P3" is on row index 4. (Top is 0).
    // d4 (3,3)
    // c3 (2,2) is enemy 'p'.
    // e5 (4,4) is friend 'P'.
    
    Piece* p = Board::gameboxess[3][3].getPiece();
    Bishop* b = dynamic_cast<Bishop*>(p);
    
    std::set<Box*> moves = b->moveAndTake();
    
    bool hitFriend = false;
    bool hitEnemy = false;
    bool pastEnemy = false; 
    
    for(auto m : moves) {
        int c = m->x / Board::BoxWidthandHigth;
        int r = m->y / Board::BoxWidthandHigth;
        
        if (c == 4 && r == 4) hitFriend = true;
        if (c == 2 && r == 2) hitEnemy = true;
        if (c == 1 && r == 1) pastEnemy = true; // b2, should be blocked by capture
    }
    
    EXPECT_FALSE(hitFriend); // Cannot take friend
    EXPECT_TRUE(hitEnemy);   // Can take enemy
    EXPECT_FALSE(pastEnemy); // Cannot move past capture
}
