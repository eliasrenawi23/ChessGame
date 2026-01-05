#include <gtest/gtest.h>
#include "Board.h"
#include "Window.h"
#include "Pawn.h"
#include "King.h"
#include "Rook.h"

class IntegrationTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        Window::SCREEN_WIDTH = 800;
        Window::SCREEN_HEIGHT = 800;
        Window::SQUARE_SIZE = 800;
        board.init();
    }
    
    // Helper to simulate a move (select + play)
    void makeMove(int fromCol, int fromRow, int toCol, int toRow) {
        int sq = 100; // 800/8 = 100
        board.getLegalMovs(fromCol * sq + 50, fromRow * sq + 50);
        board.play(toCol * sq + 50, toRow * sq + 50);
    }
};

// ============ Opening Move Sequences ============

TEST_F(IntegrationTest, StandardOpeningE4E5) {
    // 1. e4 e5
    // White pawn e2 (4,6) -> e4 (4,4)
    makeMove(4, 6, 4, 4);
    
    // Verify White pawn moved
    EXPECT_EQ(Board::gameboxess[4][6].getPiece(), nullptr);
    EXPECT_NE(Board::gameboxess[4][4].getPiece(), nullptr);
    
    // Black pawn e7 (4,1) -> e5 (4,3)
    makeMove(4, 1, 4, 3);
    
    EXPECT_EQ(Board::gameboxess[4][1].getPiece(), nullptr);
    EXPECT_NE(Board::gameboxess[4][3].getPiece(), nullptr);
}

TEST_F(IntegrationTest, ItalianGameOpening) {
    // 1. e4 e5 2. Nf3 Nc6 3. Bc4
    makeMove(4, 6, 4, 4);  // e4
    makeMove(4, 1, 4, 3);  // e5
    makeMove(6, 7, 5, 5);  // Nf3
    makeMove(1, 0, 2, 2);  // Nc6
    makeMove(5, 7, 2, 4);  // Bc4
    
    // Verify final positions
    EXPECT_NE(Board::gameboxess[4][4].getPiece(), nullptr);  // e4
    EXPECT_NE(Board::gameboxess[4][3].getPiece(), nullptr);  // e5
    EXPECT_NE(Board::gameboxess[5][5].getPiece(), nullptr);  // Nf3
    EXPECT_NE(Board::gameboxess[2][2].getPiece(), nullptr);  // Nc6
    EXPECT_NE(Board::gameboxess[2][4].getPiece(), nullptr);  // Bc4
}

// ============ Castling Execution ============

TEST_F(IntegrationTest, KingsideCastlingExecution) {
    // Set up for castling: clear f1, g1
    board.clear();
    board.loadFEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
    
    // White King e1 (4,7) -> g1 (6,7)
    makeMove(4, 7, 6, 7);
    
    // Verify King at g1
    Piece* king = Board::gameboxess[6][7].getPiece();
    ASSERT_NE(king, nullptr);
    EXPECT_TRUE(dynamic_cast<King*>(king) != nullptr);
    
    // Verify Rook at f1 (moved from h1)
    Piece* rook = Board::gameboxess[5][7].getPiece();
    ASSERT_NE(rook, nullptr);
    EXPECT_TRUE(dynamic_cast<Rook*>(rook) != nullptr);
    
    // Verify original positions empty
    EXPECT_EQ(Board::gameboxess[4][7].getPiece(), nullptr);  // e1
    EXPECT_EQ(Board::gameboxess[7][7].getPiece(), nullptr);  // h1
}

TEST_F(IntegrationTest, QueensideCastlingExecution) {
    board.clear();
    board.loadFEN("r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1");
    
    // White King e1 (4,7) -> c1 (2,7)
    makeMove(4, 7, 2, 7);
    
    // Verify King at c1
    Piece* king = Board::gameboxess[2][7].getPiece();
    ASSERT_NE(king, nullptr);
    EXPECT_TRUE(dynamic_cast<King*>(king) != nullptr);
    
    // Verify Rook at d1 (moved from a1)
    Piece* rook = Board::gameboxess[3][7].getPiece();
    ASSERT_NE(rook, nullptr);
    EXPECT_TRUE(dynamic_cast<Rook*>(rook) != nullptr);
}

// ============ Capture Sequences ============

TEST_F(IntegrationTest, PawnCaptureSequence) {
    board.clear();
    board.loadFEN("4k3/8/8/3p4/4P3/8/8/4K3 w - - 0 1");
    
    // White pawn e4 (4,4) captures d5 (3,3)
    makeMove(4, 4, 3, 3);
    
    // Verify capture
    Piece* pawn = Board::gameboxess[3][3].getPiece();
    ASSERT_NE(pawn, nullptr);
    EXPECT_EQ(pawn->getColor(), PlayerColor::WHITE);
    EXPECT_EQ(Board::gameboxess[4][4].getPiece(), nullptr);
}

TEST_F(IntegrationTest, KnightCaptureSequence) {
    board.clear();
    board.loadFEN("4k3/8/5p2/8/4N3/8/8/4K3 w - - 0 1");
    
    // Knight e4 (4,4) captures f6 (5,2)
    makeMove(4, 4, 5, 2);
    
    Piece* knight = Board::gameboxess[5][2].getPiece();
    ASSERT_NE(knight, nullptr);
    EXPECT_EQ(knight->getColor(), PlayerColor::WHITE);
}

// ============ Turn Management ============

TEST_F(IntegrationTest, TurnAlternation) {
    // White moves first
    makeMove(4, 6, 4, 4);  // e4
    
    // Black should move next - verify Black can move
    Piece* blackPawn = Board::gameboxess[4][1].getPiece();
    ASSERT_NE(blackPawn, nullptr);
    EXPECT_EQ(blackPawn->getColor(), PlayerColor::BLACK);
    
    makeMove(4, 1, 4, 3);  // e5
    
    // White's turn again
    Piece* whitePawn = Board::gameboxess[3][6].getPiece();
    ASSERT_NE(whitePawn, nullptr);
    EXPECT_EQ(whitePawn->getColor(), PlayerColor::WHITE);
}

// ============ En Passant Full Flow ============

TEST_F(IntegrationTest, EnPassantFullExecution) {
    board.clear();
    board.loadFEN("4k3/8/8/8/4p3/8/3P4/4K3 w - - 0 1");
    
    // Set Black pawn as not first move
    if (Piece* bp = Board::gameboxess[4][4].getPiece()) {
        bp->firstMove = false;
    }
    
    // White pawn d2 (3,6) -> d4 (3,4) - double step
    makeMove(3, 6, 3, 4);
    
    // Verify PossibleEnPassant is set
    Pawn* wp = dynamic_cast<Pawn*>(Board::gameboxess[3][4].getPiece());
    ASSERT_NE(wp, nullptr);
    EXPECT_TRUE(wp->PossibleEnPassant);
    
    // Black pawn e4 (4,4) captures d3 (3,5) en passant
    makeMove(4, 4, 3, 5);
    
    // Verify Black pawn at d3
    Piece* capturer = Board::gameboxess[3][5].getPiece();
    ASSERT_NE(capturer, nullptr);
    EXPECT_EQ(capturer->getColor(), PlayerColor::BLACK);
    
    // Verify White pawn at d4 is captured
    EXPECT_EQ(Board::gameboxess[3][4].getPiece(), nullptr);
}

// ============ Multi-Move Game Sequence ============

TEST_F(IntegrationTest, TenMoveSequence) {
    // Play 5 moves for each side
    // 1. e4 e5
    makeMove(4, 6, 4, 4);
    makeMove(4, 1, 4, 3);
    
    // 2. Nf3 Nc6
    makeMove(6, 7, 5, 5);
    makeMove(1, 0, 2, 2);
    
    // 3. Bb5 a6
    makeMove(5, 7, 1, 3);
    makeMove(0, 1, 0, 2);
    
    // 4. Ba4 Nf6
    makeMove(1, 3, 0, 4);
    makeMove(6, 0, 5, 2);
    
    // 5. O-O (Kingside castling)
    makeMove(4, 7, 6, 7);
    
    // Verify final position - King should be at g1
    Piece* king = Board::gameboxess[6][7].getPiece();
    EXPECT_TRUE(dynamic_cast<King*>(king) != nullptr);
    
    // Rook at f1
    Piece* rook = Board::gameboxess[5][7].getPiece();
    EXPECT_TRUE(dynamic_cast<Rook*>(rook) != nullptr);
}
