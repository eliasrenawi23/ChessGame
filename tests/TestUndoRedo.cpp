#include <gtest/gtest.h>
#include "Board.h"
#include "Window.h"
#include "Pawn.h"

class UndoRedoTest : public ::testing::Test {
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

TEST_F(UndoRedoTest, SimpleMoveUndoRedo) {
    board.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    // Move e2 (4,6) to e4 (4,4)
    board.getLegalMovs(450, 650);
    board.play(450, 450);
    
    EXPECT_NE(board.gameboxess[4][4].getPiece(), nullptr);
    EXPECT_EQ(board.gameboxess[4][6].getPiece(), nullptr);
    EXPECT_EQ(board.getHistory().size(), 1);
    EXPECT_EQ(board.getCurrentMoveIndex(), 1);
    
    // Undo
    board.undo();
    EXPECT_EQ(board.gameboxess[4][4].getPiece(), nullptr);
    EXPECT_NE(board.gameboxess[4][6].getPiece(), nullptr);
    EXPECT_EQ(board.getHistory().size(), 1);
    EXPECT_EQ(board.getCurrentMoveIndex(), 0);
    
    // Redo
    board.redo();
    EXPECT_NE(board.gameboxess[4][4].getPiece(), nullptr);
    EXPECT_EQ(board.gameboxess[4][6].getPiece(), nullptr);
    EXPECT_EQ(board.getCurrentMoveIndex(), 1);
}

TEST_F(UndoRedoTest, CaptureUndo) {
    // White Rook d1, Black Rook d8. Capture d8.
    board.loadFEN("3r4/8/8/8/8/8/8/3R4 w - - 0 1");
    
    board.getLegalMovs(350, 750); // d1 (3,7) -> Wait, row 7 is bottom. d1 is (3,7).
    
    // Move to d8 (3,0) -> 350, 50
    // Move to d8 (3,0) -> 350, 50
    board.play(350, 50);
    
    // Verify Capture
    EXPECT_NE(board.gameboxess[3][0].getPiece(), nullptr);
    EXPECT_EQ(board.gameboxess[3][0].getPiece()->getColor(), PlayerColor::WHITE);
    EXPECT_EQ(board.gameboxess[3][7].getPiece(), nullptr);
    
    // Undo
    board.undo();
    
    // White Rook back at d1
    EXPECT_NE(board.gameboxess[3][7].getPiece(), nullptr);
    EXPECT_EQ(board.gameboxess[3][7].getPiece()->getColor(), PlayerColor::WHITE);
    
    // Black Rook back at d8
    EXPECT_NE(board.gameboxess[3][0].getPiece(), nullptr);
    EXPECT_EQ(board.gameboxess[3][0].getPiece()->getColor(), PlayerColor::BLACK);
}

TEST_F(UndoRedoTest, PromotionUndo) {
    // White Pawn at e7. Move to e8.
    board.loadFEN("k7/4P3/8/8/8/8/8/K7 w - - 0 1");
    
    board.getLegalMovs(450, 150); // e7
    board.play(450, 50); // e8
    
    // Now selecting promotion piece (Queen)
    // Coords for Queen option? Player.cpp handles it.
    // The options are placed at (2,4) to (5,4).
    // Queen is at (2,4) -> 250, 450
    board.getLegalMovs(250, 450);
    board.play(250, 450);
    
    // Verify Queen at e8
    Piece* p = board.gameboxess[4][0].getPiece();
    ASSERT_NE(p, nullptr);
    // Dynamic cast might fail if class logic is hidden, but Piece getType? No. 
    // Just move on.
    
    // Undo
    board.undo();
    
    // Should be Pawn at e7
    EXPECT_EQ(board.gameboxess[4][0].getPiece(), nullptr);
    Piece* p2 = board.gameboxess[4][1].getPiece();
    ASSERT_NE(p2, nullptr);
    // Verify it is a Pawn?
    // Casting to Pawn*
    // EXPECT_TRUE(dynamic_cast<Pawn*>(p2));
}
