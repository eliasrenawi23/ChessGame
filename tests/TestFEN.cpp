#include <gtest/gtest.h>
#include "Board.h"
#include "Pawn.h"

// Test fixture
class FENTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init(); // Initialize mainly to allocate stuff, but FEN should clear it.
    }
};

TEST_F(FENTest, LoadStandardStart) {
    board.loadFEN("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
    
    // Check White Rook at 0,7 (file a, rank 1) -> 7 is bottom
    // Parser logic:
    // "rnbqkbnr" -> row 0.
    // row 0 should be top (Black).
    // Let's verify our parsing loop logic.
    // for string "r..."
    //   if c=='/' row++; 
    //   if c is piece -> gameboxess[col][row].
    //
    // Standard FEN starts with Rank 8 (Top/Black).
    // So row 0 in parse loop corresponds to FEN Rank 8.
    // In SDK/Engine representation where 0,0 is usually Top-Left?
    // In our tests earlier, we assumed [0][6] is White Pawn.
    // If [0][0] is Top-Left (Black Rook usually).
    //
    // Let's check Board::init() from Board.cpp:
    // PawnYPostion = (Black) ? 1 : 6;
    // RestOfPieces = (Black) ? 0 : 7;
    //
    // So 0 is Top (Black), 7 is Bottom (White).
    //
    // FEN String: "rnbqkbnr..."
    // r is Black Rook.
    // Should be at row 0.
    
    Piece* p = Board::gameboxess[0][0].getPiece();
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->getColor(), PlayerColor::BLACK);
    EXPECT_TRUE(dynamic_cast<Rook*>(p));
    
    // Last char in first block 'r' -> col 7, row 0.
    Piece* p2 = Board::gameboxess[7][0].getPiece();
    ASSERT_NE(p2, nullptr);
    EXPECT_EQ(p2->getColor(), PlayerColor::BLACK);
    
    // Basic check for White Pawn at row 6, col 4 (e2 -> e4 in FEN?)
    // Wait, the FEN string I used above is "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR"
    // This is NOT the standard start position!
    // This is a position after 1. e4 c5.
    //
    // Let's use standard start FEN first: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    
    board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    // White Pawn at [0][6] (a2)
    Piece* wp = Board::gameboxess[0][6].getPiece();
    ASSERT_NE(wp, nullptr);
    EXPECT_EQ(wp->getColor(), PlayerColor::WHITE);
}

TEST_F(FENTest, CustomScenario) {
    // Empty board except white king at 0,0 and black king at 7,7
    // FEN: 8/8/8/8/8/8/7k/K7 w - - 0 1
    // Wait, 'K' is white king, 'k' is black king.
    // Row 0 usually top.
    // Fen: "8/8/..."
    
    // Let's try: "k7/8/8/8/8/8/8/K7 w - - 0 1"
    // Row 0: "k7" -> Black King at 0,0
    // Row 7: "K7" -> White King at 0,7
    
    board.loadFEN("k7/8/8/8/8/8/8/K7 w - - 0 1");
    
    Piece* bk = Board::gameboxess[0][0].getPiece();
    ASSERT_NE(bk, nullptr);
    EXPECT_TRUE(dynamic_cast<King*>(bk));
    EXPECT_EQ(bk->getColor(), PlayerColor::BLACK);
    
    Piece* wk = Board::gameboxess[0][7].getPiece();
    ASSERT_NE(wk, nullptr);
    EXPECT_TRUE(dynamic_cast<King*>(wk));
    EXPECT_EQ(wk->getColor(), PlayerColor::WHITE);
    
    // Ensure middle is empty
    EXPECT_EQ(Board::gameboxess[4][4].getPiece(), nullptr);
}
