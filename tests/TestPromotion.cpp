#include <gtest/gtest.h>
#include "Board.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Window.h"

class PromotionTest : public ::testing::Test {
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

TEST_F(PromotionTest, PawnToQueen) {
    // White Pawn at e7. Move to e8. Kings at e1/e8 to prevent crash?
    // Wait, e8 is where we move to.
    // Let's put Black King at a8 (0,0). White King at a1 (0,7).
    // FEN: k7/4P3/8/8/8/8/8/K7 w - - 0 1
    board.loadFEN("k7/4P3/8/8/8/8/8/K7 w - - 0 1");
    
    // Select e7 (4,1) -> (450, 150)
    board.getLegalMovs(450, 150);
    
    // Move to e8 (4,0) -> (450, 50)
    // Board::handle_promotion sets 'promotion' flag and breaks flow?
    // It says "we flip the turn twice to back to current player".
    // And it populates promotion options.
    board.play(450, 50);
    
    // Verify pawn is NOT at e8 yet (waiting for selection).
    // Or did it clear e7?
    // Board.cpp: handle_promotion -> sets promotion=true. Creates specific Options pieces.
    // It does NOT move the piece yet?
    // Wait. "if (boxtoLight.empty())" -> handle_promotion.
    
    // To trigger promotion menu:
    // Board::play calls handle_promotion then returns?
    
    // Let's check Board.cpp line 96:
    // if (boxtoLight.empty()) { handle_promotion... return; }
    // But we selected the piece, so boxtoLight is NOT empty.
    // It goes to line 116: handle_promotion...
    // then selectedBox->setPiece(NULL).
    
    // So Pawn is removed from e7.
    EXPECT_EQ(Board::gameboxess[4][1].getPiece(), nullptr);
    
    // It is NOT yet at e8?
    // Expectation: Game enters "promotion mode". e8 contains "promotionBox"?
    
    // Now we must select a promotion piece.
    // The player clicks on one of the options.
    // Where are the options?
    // Player::handle_promotion puts them at (2,4) to (5,4).
    // Queen is at (2,4) -> c5?
    
    // Select Queen at (2,4) -> (250, 450)
    board.getLegalMovs(250, 450);
    
    // Click again to confirm/play? 
    // Board::play -> "else if (*promotion)"
    // Calls dopromotion.
    board.play(250, 450);
    
    // Now e8 should have a Queen.
    Piece* p = Board::gameboxess[4][0].getPiece();
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(dynamic_cast<Queen*>(p));
    EXPECT_EQ(p->getColor(), PlayerColor::WHITE);
}

TEST_F(PromotionTest, PawnToRook) {
    board.loadFEN("k7/4P3/8/8/8/8/8/K7 w - - 0 1");
    
    board.getLegalMovs(450, 150); // Select e7
    board.play(450, 50);          // Move to e8
    
    // Select Rook from promotion options
    // Player::handle_promotion creates Queen@(2,4), Knight@(3,4), Rook@(4,4), Bishop@(5,4)
    board.getLegalMovs(450, 450); // (4,4) -> Rook option
    board.play(450, 450);
    
    Piece* p = Board::gameboxess[4][0].getPiece();
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(dynamic_cast<Rook*>(p));
}

TEST_F(PromotionTest, PawnToBishop) {
    board.loadFEN("k7/4P3/8/8/8/8/8/K7 w - - 0 1");
    
    board.getLegalMovs(450, 150);
    board.play(450, 50);
    
    // Bishop at col 5
    board.getLegalMovs(550, 450);
    board.play(550, 450);
    
    Piece* p = Board::gameboxess[4][0].getPiece();
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(dynamic_cast<Bishop*>(p));
}

TEST_F(PromotionTest, PawnToKnight) {
    board.loadFEN("k7/4P3/8/8/8/8/8/K7 w - - 0 1");
    
    board.getLegalMovs(450, 150);
    board.play(450, 50);
    
    // Knight at col 3
    board.getLegalMovs(350, 450);
    board.play(350, 450);
    
    Piece* p = Board::gameboxess[4][0].getPiece();
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(dynamic_cast<Knight*>(p));
}

TEST_F(PromotionTest, BlackPawnToQueen) {
    // Black Pawn at e2, move to e1
    board.loadFEN("k7/8/8/8/8/8/4p3/K7 b - - 0 1");
    
    board.getLegalMovs(450, 650); // e2 (4,6)
    board.play(450, 750);          // e1 (4,7)
    
    // Queen at col 2
    board.getLegalMovs(250, 450);
    board.play(250, 450);
    
    Piece* p = Board::gameboxess[4][7].getPiece();
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(dynamic_cast<Queen*>(p));
    EXPECT_EQ(p->getColor(), PlayerColor::BLACK);
}

TEST_F(PromotionTest, PromotionWithCapture) {
    // White Pawn at d7, Black Rook at e8. Capture and promote.
    board.loadFEN("k3r3/3P4/8/8/8/8/8/K7 w - - 0 1");
    
    board.getLegalMovs(350, 150); // d7 (3,1)
    board.play(450, 50);           // e8 (4,0) - capture Rook
    
    // Queen at col 2
    board.getLegalMovs(250, 450);
    board.play(250, 450);
    
    Piece* p = Board::gameboxess[4][0].getPiece();
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(dynamic_cast<Queen*>(p));
    EXPECT_EQ(p->getColor(), PlayerColor::WHITE);
}

TEST_F(PromotionTest, PromotionPawnAdvanceOnly) {
    // Check that pawn at e7 can only move forward (e8), not backward
    board.loadFEN("k7/4P3/8/8/8/8/8/K7 w - - 0 1");
    
    Piece* pawn = Board::gameboxess[4][1].getPiece();
    Pawn* p = dynamic_cast<Pawn*>(pawn);
    ASSERT_NE(p, nullptr);
    
    std::set<Box*> moves = p->moveAndTake();
    
    // Should only have e8 (4,0)
    EXPECT_EQ(moves.size(), 1);
    bool hasE8 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 4 && m->y / Board::BoxWidthandHigth == 0) hasE8 = true;
    }
    EXPECT_TRUE(hasE8);
}

