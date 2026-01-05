#include <gtest/gtest.h>
#include "Board.h"
#include "Knight.h"
#include <set>
#include <algorithm>

class KnightTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
        board.clear();
    }
};

TEST_F(KnightTest, CenterMovement) {
    // Knight at d4 (3, 3) - should have 8 moves
    // FEN: 8/8/8/8/3N4/8/8/8 w - - 0 1
    // row 0 is top (8), row 7 is bottom (1).
    // d4 is at col 3, row 4.
    // 0: 8
    // 1: 7
    // 2: 6
    // 3: 5
    // 4: 4
    
    board.loadFEN("8/8/8/3N4/8/8/8/8 w - - 0 1");
    // Verify placement: col 3, row 3. (d5 actually. 3,3 is d5 if row 0 is 8).
    // Let's rely on getPiece from known coord.
    // FEN "3N4" is on 4th line (index 3).
    
    Piece* p = Board::gameboxess[3][3].getPiece();
    ASSERT_NE(p, nullptr);
    Knight* knight = dynamic_cast<Knight*>(p);
    ASSERT_NE(knight, nullptr);
    
    std::set<Box*> moves = knight->moveAndTake();
    EXPECT_EQ(moves.size(), 8);
}

TEST_F(KnightTest, CornerMovement) {
    // Knight at a1 (0, 7) - should have 2 moves (b3, c2)
    // FEN: 8/8/8/8/8/8/8/N7 w - - 0 1
    
    board.loadFEN("8/8/8/8/8/8/8/N7 w - - 0 1");
    Piece* p = Board::gameboxess[0][7].getPiece();
    ASSERT_NE(p, nullptr);
    Knight* knight = dynamic_cast<Knight*>(p);
    
    std::set<Box*> moves = knight->moveAndTake();
    EXPECT_EQ(moves.size(), 2);
    
    bool hasB3 = false; // 1, 5
    bool hasC2 = false; // 2, 6
    for(auto m : moves) {
        // x=col, y=row. a1 is 0,7.
        // b3 is 1, 5.
        // c2 is 2, 6.
        int c = m->x / Board::BoxWidthandHigth;
        int r = m->y / Board::BoxWidthandHigth;
        if (c == 1 && r == 5) hasB3 = true;
        if (c == 2 && r == 6) hasC2 = true;
    }
    EXPECT_TRUE(hasB3);
    EXPECT_TRUE(hasC2);
}

TEST_F(KnightTest, CaptureAndBlock) {
    // Knight at d5 (3,3) [Row index 3 in FEN if we assume top-start, but let's stick to indices]
    // Valid moves from (3,3):
    // (1,2), (2,1), (4,1), (5,2), etc.
    
    // We want:
    // Capture enemy at (2,1) [c7]
    // Blocked by friend at (4,1) [e7]
    
    // FEN Rows:
    // 0: 8
    // 1: 7 -> "2p1P3" (col 2='p', col 4='P')
    // 2: 6 -> "8"
    // 3: 5 -> "3N4" (col 3='N')
    
    board.loadFEN("8/2p1P3/8/3N4/8/8/8/8 w - - 0 1");
    // Verify Knight
    Piece* p = Board::gameboxess[3][3].getPiece();
    ASSERT_NE(p, nullptr);
    Knight* knight = dynamic_cast<Knight*>(p);
    
    // Verify targets
    EXPECT_NE(Board::gameboxess[2][1].getPiece(), nullptr); // Enemy
    EXPECT_NE(Board::gameboxess[4][1].getPiece(), nullptr); // Friend
    
    std::set<Box*> moves = knight->moveAndTake();
    
    bool canMoveToFriend = false; // (4,1)
    bool canCaptureEnemy = false; // (2,1)
    
    for(auto m : moves) {
        int c = m->x / Board::BoxWidthandHigth;
        int r = m->y / Board::BoxWidthandHigth;
        if (c == 4 && r == 1) canMoveToFriend = true;
        if (c == 2 && r == 1) canCaptureEnemy = true;
    }
    
    EXPECT_FALSE(canMoveToFriend);
    EXPECT_TRUE(canCaptureEnemy);
}

TEST_F(KnightTest, EdgeOfBoard) {
    // Knight at b1 (1,7) - should have 3 moves: a3, c3, d2
    board.loadFEN("8/8/8/8/8/8/8/1N6 w - - 0 1");
    Piece* p = Board::gameboxess[1][7].getPiece();
    Knight* knight = dynamic_cast<Knight*>(p);
    
    std::set<Box*> moves = knight->moveAndTake();
    EXPECT_EQ(moves.size(), 3);
}

TEST_F(KnightTest, NearCornerG1) {
    // Knight at g1 (6,7) - should have 3 moves
    board.loadFEN("8/8/8/8/8/8/8/6N1 w - - 0 1");
    Piece* p = Board::gameboxess[6][7].getPiece();
    Knight* knight = dynamic_cast<Knight*>(p);
    
    EXPECT_EQ(knight->moveAndTake().size(), 3);
}

TEST_F(KnightTest, SurroundedByFriends) {
    // Knight at d4 with all 8 target squares occupied by friendly pawns
    board.loadFEN("8/8/2P1P3/1P3P2/3N4/1P3P2/2P1P3/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    Knight* knight = dynamic_cast<Knight*>(p);
    
    // Should have 0 legal moves - all blocked by friends
    EXPECT_EQ(knight->moveAndTake().size(), 0);
}

TEST_F(KnightTest, AllCapturesAvailable) {
    // Knight at d4 with all 8 target squares occupied by enemy pawns
    board.loadFEN("8/8/2p1p3/1p3p2/3N4/1p3p2/2p1p3/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    Knight* knight = dynamic_cast<Knight*>(p);
    
    // Should have 8 legal moves - all captures
    EXPECT_EQ(knight->moveAndTake().size(), 8);
}

TEST_F(KnightTest, MixedFriendAndEnemy) {
    // Knight at d4 with mixed friendly and enemy pieces on target squares
    // Friends at b3, c2, e2, f3 - Enemies at b5, c6, e6, f5
    board.loadFEN("8/8/2p1p3/1p3p2/3N4/1P3P2/2P1P3/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    Knight* knight = dynamic_cast<Knight*>(p);
    
    std::set<Box*> moves = knight->moveAndTake();
    // Can capture 4 enemies, blocked by 4 friends
    EXPECT_EQ(moves.size(), 4);
}

