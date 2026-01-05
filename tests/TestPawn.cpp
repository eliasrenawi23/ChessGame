#include <gtest/gtest.h>
#include "Board.h"
#include "Pawn.h"

class PawnTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
        board.clear();
    }
};

// ============ Basic Movement Tests ============

TEST_F(PawnTest, WhiteInitialDoubleStep) {
    // White pawn at e2 can move to e3 or e4
    board.loadFEN("8/8/8/8/8/8/4P3/8 w - - 0 1");
    Piece* p = Board::gameboxess[4][6].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    ASSERT_NE(pawn, nullptr);
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // Should have 2 moves: e3 (4,5) and e4 (4,4)
    EXPECT_EQ(moves.size(), 2);
    
    bool hasE3 = false, hasE4 = false;
    for(auto m : moves) {
        int c = m->x / Board::BoxWidthandHigth;
        int r = m->y / Board::BoxWidthandHigth;
        if (c == 4 && r == 5) hasE3 = true;
        if (c == 4 && r == 4) hasE4 = true;
    }
    EXPECT_TRUE(hasE3);
    EXPECT_TRUE(hasE4);
}

TEST_F(PawnTest, WhiteSingleStepAfterMove) {
    // White pawn at e4 (already moved) can only move to e5
    board.loadFEN("8/8/8/8/4P3/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[4][4].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    pawn->firstMove = false;
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // Should have 1 move: e5 (4,3)
    EXPECT_EQ(moves.size(), 1);
}

TEST_F(PawnTest, BlackInitialDoubleStep) {
    // Black pawn at e7 can move to e6 or e5
    board.loadFEN("8/4p3/8/8/8/8/8/8 b - - 0 1");
    Piece* p = Board::gameboxess[4][1].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    EXPECT_EQ(moves.size(), 2);
}

TEST_F(PawnTest, BlackSingleStepAfterMove) {
    // Black pawn at e5 (already moved) can only move to e4
    board.loadFEN("8/8/8/4p3/8/8/8/8 b - - 0 1");
    Piece* p = Board::gameboxess[4][3].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    pawn->firstMove = false;
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    EXPECT_EQ(moves.size(), 1);
}

// ============ Blocked Movement Tests ============

TEST_F(PawnTest, BlockedByPieceInFront) {
    // White pawn at e2, enemy pawn at e3 - cannot move
    board.loadFEN("8/8/8/8/8/4p3/4P3/8 w - - 0 1");
    Piece* p = Board::gameboxess[4][6].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // Cannot move forward - blocked
    EXPECT_EQ(moves.size(), 0);
}

TEST_F(PawnTest, BlockedDoubleStepByPieceOnSecondSquare) {
    // White pawn at e2, piece at e4 - can only move to e3
    board.loadFEN("8/8/8/8/4p3/8/4P3/8 w - - 0 1");
    Piece* p = Board::gameboxess[4][6].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // Can move to e3, but not e4
    EXPECT_EQ(moves.size(), 1);
    
    bool hasE3 = false, hasE4 = false;
    for(auto m : moves) {
        int c = m->x / Board::BoxWidthandHigth;
        int r = m->y / Board::BoxWidthandHigth;
        if (c == 4 && r == 5) hasE3 = true;
        if (c == 4 && r == 4) hasE4 = true;
    }
    EXPECT_TRUE(hasE3);
    EXPECT_FALSE(hasE4);
}

TEST_F(PawnTest, BlockedByFriendlyPiece) {
    // White pawn at e2, friendly pawn at e3
    board.loadFEN("8/8/8/8/8/4P3/4P3/8 w - - 0 1");
    Piece* p = Board::gameboxess[4][6].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    EXPECT_EQ(moves.size(), 0);
}

// ============ Capture Tests ============

TEST_F(PawnTest, DiagonalCaptureRight) {
    // White pawn at d4, enemy at e5
    board.loadFEN("8/8/8/4p3/3P4/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    pawn->firstMove = false;
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // Can move d5 and capture e5
    bool hasD5 = false, hasE5 = false;
    for(auto m : moves) {
        int c = m->x / Board::BoxWidthandHigth;
        int r = m->y / Board::BoxWidthandHigth;
        if (c == 3 && r == 3) hasD5 = true;
        if (c == 4 && r == 3) hasE5 = true;
    }
    EXPECT_TRUE(hasD5);  // Forward move
    EXPECT_TRUE(hasE5);  // Capture
}

TEST_F(PawnTest, DiagonalCaptureLeft) {
    // White pawn at e4, enemy at d5
    board.loadFEN("8/8/8/3p4/4P3/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[4][4].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    pawn->firstMove = false;
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    bool hasD5 = false;
    for(auto m : moves) {
        int c = m->x / Board::BoxWidthandHigth;
        int r = m->y / Board::BoxWidthandHigth;
        if (c == 3 && r == 3) hasD5 = true;
    }
    EXPECT_TRUE(hasD5);
}

TEST_F(PawnTest, BothCapturesAvailable) {
    // White pawn at d4, enemies at c5 and e5
    board.loadFEN("8/8/8/2p1p3/3P4/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    pawn->firstMove = false;
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // d5, c5, e5 = 3 moves
    EXPECT_EQ(moves.size(), 3);
}

TEST_F(PawnTest, CannotCaptureFriendly) {
    // White pawn at d4, friendly pawns at c5 and e5
    board.loadFEN("8/8/8/2P1P3/3P4/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    pawn->firstMove = false;
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // Only d5 forward move
    EXPECT_EQ(moves.size(), 1);
}

// ============ Edge of Board Tests ============

TEST_F(PawnTest, PawnOnAFile) {
    // White pawn at a4 - can only capture to the right
    board.loadFEN("8/8/8/1p6/P7/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[0][4].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    pawn->firstMove = false;
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // a5 forward, b5 capture
    EXPECT_EQ(moves.size(), 2);
}

TEST_F(PawnTest, PawnOnHFile) {
    // White pawn at h4 - can only capture to the left
    board.loadFEN("8/8/8/6p1/7P/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[7][4].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    pawn->firstMove = false;
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // h5 forward, g5 capture
    EXPECT_EQ(moves.size(), 2);
}

// ============ Black Pawn Direction Tests ============

TEST_F(PawnTest, BlackPawnCapturesCorrectDirection) {
    // Black pawn at e5, enemies at d4 and f4
    board.loadFEN("8/8/8/4p3/3P1P2/8/8/8 b - - 0 1");
    Piece* p = Board::gameboxess[4][3].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    pawn->firstMove = false;
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // e4 forward, d4 capture, f4 capture
    EXPECT_EQ(moves.size(), 3);
}

TEST_F(PawnTest, BlackPawnBlockedFromBehind) {
    // Black pawn at e5, piece at e6 (behind) should not affect movement
    board.loadFEN("8/8/4P3/4p3/8/8/8/8 b - - 0 1");
    Piece* p = Board::gameboxess[4][3].getPiece();
    Pawn* pawn = dynamic_cast<Pawn*>(p);
    pawn->firstMove = false;
    
    std::set<Box*> moves = pawn->moveAndTake();
    
    // e4 forward
    EXPECT_EQ(moves.size(), 1);
}
