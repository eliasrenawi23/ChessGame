#include <gtest/gtest.h>
#include "Board.h"
#include "Piece.h"

TEST(BoardTest, InBounds) {
    EXPECT_TRUE(Board::isInBounds(0, 0));
    EXPECT_TRUE(Board::isInBounds(7, 7));
    EXPECT_FALSE(Board::isInBounds(-1, 0));
    EXPECT_FALSE(Board::isInBounds(0, 8));
}

TEST(BoardTest, Initialization) {
    Board board;
    board.init();

    // Check if the board is populated
    // Dimensions 8x8
    EXPECT_EQ(Board::gameboxess.size(), 8);
    EXPECT_EQ(Board::gameboxess[0].size(), 8);
    
    // Check for pieces on initial board
    // A black rook should be at 0,0
    Piece* p = Board::gameboxess[0][0].getPiece();
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->getColor(), PlayerColor::BLACK);
    
    // White pawn should be at col 0, row 6
    Piece* wp = Board::gameboxess[0][6].getPiece();
    ASSERT_NE(wp, nullptr);
    EXPECT_EQ(wp->getColor(), PlayerColor::WHITE);
}
