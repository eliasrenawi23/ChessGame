#include <gtest/gtest.h>
#include "Board.h"
#include "Window.h"

class SANTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        Window::SCREEN_WIDTH = 800;
        Window::SCREEN_HEIGHT = 800;
        Window::SQUARE_SIZE = 800;
        board.init();
        board.clear();
    }
};

TEST_F(SANTest, SimplePawnMove) {
    board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // e2 -> e4
    // e2 is (4,6) ? Board logic: row 6 is white pawns. col 4 is e.
    // e4 is (4,4).
    
    // Coordinates in Board are [col][row].
    // e2: box at [4][6].
    // e4: box at [4][4].
    
    // Coordinates passing to play are screen pixel coords?
    // play(int cor_x, int cor_y)
    // Coords: x = col * 100, y = row * 100.
    // e2 (4,6) -> 450, 650
    // e4 (4,4) -> 450, 450
    
    board.getLegalMovs(450, 650);
    board.play(450, 450);
    
    ASSERT_EQ(board.getHistory().size(), 1);
    
    // This line should fail compilation first (no 'san' member)
    EXPECT_EQ(board.getHistory().back().san, "e4"); 
}

TEST_F(SANTest, SimpleKnightMove) {
    board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    // Ng1 -> f3
    // g1 is (6, 7). f3 is (5, 5). 
    // Wait, rank 1 is row 7. rank 3 is row 5. Correct.
    // g is col 6. f is col 5. Correct.
    
    board.getLegalMovs(650, 750);
    board.play(550, 550);
    
    EXPECT_EQ(board.getHistory().back().san, "Nf3");
}

TEST_F(SANTest, Capture) {
    // White Pawn e4, Black Pawn d5.
    board.loadFEN("rnbqkbnr/pppp1ppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1");
    
    // e4 captures d5. e4->(4,4). d5->(3,3).
    board.getLegalMovs(450, 450);
    board.play(350, 350);
    
    EXPECT_EQ(board.getHistory().back().san, "exd5");
}

TEST_F(SANTest, CastlingShort) {
    // White King e1, Rook h1. Empty f1, g1.
    board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");
    
    // e1 -> g1
    // e1 (4,7). g1 (6,7).
    board.getLegalMovs(450, 750);
    board.play(650, 750);
    
    EXPECT_EQ(board.getHistory().back().san, "O-O");
}


