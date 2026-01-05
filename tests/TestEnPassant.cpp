#include <gtest/gtest.h>
#include "Board.h"
#include <gtest/gtest.h>
#include "Board.h"
#include "Pawn.h"
#include "Window.h"

class EnPassantTest : public ::testing::Test {
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

TEST_F(EnPassantTest, Execution) {
    // Setup: White Pawn at e2, Black Pawn at d4.
    // FEN: 8/8/8/8/3p4/8/4P3/8 w - - 0 1
    // e2 -> col 4, row 6
    // d4 -> col 3, row 4
    
    // FEN: 4k3/8/8/8/3p4/8/4P3/4K3 w - - 0 1 (Black Pawn at d4/Row 4)
    board.loadFEN("4k3/8/8/8/3p4/8/4P3/4K3 w - - 0 1");
    // Manually set firstMove = false for Black Pawn at d4 (3,4)
    if (Piece* bp = Board::gameboxess[3][4].getPiece()) {
        bp->firstMove = false;
    }
    
    // 1. Move White e2 -> e4 (Double step)
    // Coords: e2 (4,6) -> e4 (4,4)
    // Note: SDL rendering/clicking uses pixel coordinates in Play().
    // But we want to call logic directly if possible.
    // Board::play(x, y) takes pixels.
    // Board::getLegalMovs(x, y) selects piece.
    
    // Let's use Board::play() sequence to simulate user input.
    // We need to set 'playerTurn' correctly. FEN said 'w' so White to move.
    
    int squareSize = Window::SQUARE_SIZE / 8; // Need this val. 
    // Wait, Window::SQUARE_SIZE is static.
    // But Board uses it.
    // But Window class needs to be fine (it's static members).
    // Assuming 800x800 for headless? No, Window::SQUARE_SIZE might be 0 if not initialized?
    // Let's check Board.cpp usage. 
    // BoxWidthandHigth is initialized in Board::init().
    
    // In TestMain/SetUp, we called board.init().
    // Board::init() does:
    // BoxWidthandHigth = std::min(Window::SCREEN_HEIGHT, Window::SCREEN_WIDTH) / 8;
    // Window::SCREEN_HEIGHT are static consts (800). So it should be 100.
    
    int sq = 100;
    
    // Select White Pawn at e2 (4,6) -> Pixel (450, 650)
    // x = 4*100 + 50 = 450
    // y = 6*100 + 50 = 650
    board.getLegalMovs(450, 650); 
    
    // Move to e4 (4,4) -> Pixel (450, 450)
    board.play(450, 450);
    
    // Verify White Pawn is at e4
    Piece* wp = Board::gameboxess[4][4].getPiece();
    EXPECT_NE(wp, nullptr);
    EXPECT_EQ(Board::gameboxess[4][6].getPiece(), nullptr);
    
    // Verify PossibleEnPassant flag
    Pawn* wPawn = dynamic_cast<Pawn*>(wp);
    ASSERT_NE(wPawn, nullptr);
    EXPECT_TRUE(wPawn->PossibleEnPassant) << "White Pawn PossibleEnPassant should be true";
    
    // Now it's Black's turn. 
    // Black Pawn at d4 (3,4).
    // Can it capture En Passant to e3 (4,5)?
    
    // Select Black Pawn at d4 -> Pixel (350, 450)
    board.getLegalMovs(350, 450);
    
    // Move to e3 (4,5) -> Pixel (450, 550)
    // Capture En Passant!
    board.play(450, 550);
    
    // Verify:
    // 1. Black pawn at e3 (4,5)
    Piece* p = Board::gameboxess[4][5].getPiece();
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->getColor(), PlayerColor::BLACK);
    
    // 2. White Pawn at e4 (4,4) is GONE (captured)
    EXPECT_EQ(Board::gameboxess[4][4].getPiece(), nullptr);
}

TEST_F(EnPassantTest, WhiteCapturesLeft) {
    // White Pawn at d4, Black Pawn at c4 (just moved two steps).
    board.loadFEN("4k3/8/8/8/2pP4/8/8/4K3 w - - 0 1");
    
    // Simulate that Black pawn at c4 (2,4) just moved two steps
    Pawn* bp = dynamic_cast<Pawn*>(Board::gameboxess[2][4].getPiece());
    ASSERT_NE(bp, nullptr);
    bp->PossibleEnPassant = true;
    bp->firstMove = false;
    
    // Get White Pawn at d4 (3,4)
    Piece* wp = Board::gameboxess[3][4].getPiece();
    Pawn* whitePawn = dynamic_cast<Pawn*>(wp);
    ASSERT_NE(whitePawn, nullptr);
    
    std::set<Box*> moves = whitePawn->moveAndTake();
    
    // c5 (2,3) should be in legal moves (en passant capture - White moves upward)
    bool hasC5 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 2 && m->y / Board::BoxWidthandHigth == 3) hasC5 = true;
    }
    EXPECT_TRUE(hasC5);
}

TEST_F(EnPassantTest, BlackCapturesRight) {
    // Black Pawn at e5, White Pawn at f5 (just moved two steps).
    board.loadFEN("4k3/8/8/4pP2/8/8/8/4K3 b - - 0 1");
    
    Pawn* wp = dynamic_cast<Pawn*>(Board::gameboxess[5][3].getPiece());
    ASSERT_NE(wp, nullptr);
    wp->PossibleEnPassant = true;
    wp->firstMove = false;
    
    // Get Black Pawn at e5 (4,3)
    Piece* bp = Board::gameboxess[4][3].getPiece();
    Pawn* blackPawn = dynamic_cast<Pawn*>(bp);
    ASSERT_NE(blackPawn, nullptr);
    
    std::set<Box*> moves = blackPawn->moveAndTake();
    
    // f4 (5,4) should be in legal moves (en passant capture)
    bool hasF4 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 5 && m->y / Board::BoxWidthandHigth == 4) hasF4 = true;
    }
    EXPECT_TRUE(hasF4);
}

TEST_F(EnPassantTest, BlackCapturesLeft) {
    // Black Pawn at d5, White Pawn at c5 (just moved two steps).
    board.loadFEN("4k3/8/8/2Pp4/8/8/8/4K3 b - - 0 1");
    
    Pawn* wp = dynamic_cast<Pawn*>(Board::gameboxess[2][3].getPiece());
    ASSERT_NE(wp, nullptr);
    wp->PossibleEnPassant = true;
    wp->firstMove = false;
    
    // Get Black Pawn at d5 (3,3)
    Piece* bp = Board::gameboxess[3][3].getPiece();
    Pawn* blackPawn = dynamic_cast<Pawn*>(bp);
    ASSERT_NE(blackPawn, nullptr);
    
    std::set<Box*> moves = blackPawn->moveAndTake();
    
    // c4 (2,4) should be in legal moves (en passant capture)
    bool hasC4 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 2 && m->y / Board::BoxWidthandHigth == 4) hasC4 = true;
    }
    EXPECT_TRUE(hasC4);
}

TEST_F(EnPassantTest, NoEnPassantIfFlagNotSet) {
    // White Pawn at e4, Black Pawn at d4. But Black's PossibleEnPassant is FALSE.
    board.loadFEN("4k3/8/8/8/3Pp3/8/8/4K3 b - - 0 1");
    
    // White pawn at e4 (4,4) -> do NOT set PossibleEnPassant
    Pawn* wp = dynamic_cast<Pawn*>(Board::gameboxess[4][4].getPiece());
    ASSERT_NE(wp, nullptr);
    wp->PossibleEnPassant = false; // Not a fresh two-step move
    
    // Black tries to capture en passant
    Piece* bp = Board::gameboxess[3][4].getPiece();
    Pawn* blackPawn = dynamic_cast<Pawn*>(bp);
    ASSERT_NE(blackPawn, nullptr);
    
    std::set<Box*> moves = blackPawn->moveAndTake();
    
    // e3 (4,5) should NOT be in legal moves
    bool hasE3 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 4 && m->y / Board::BoxWidthandHigth == 5) hasE3 = true;
    }
    EXPECT_FALSE(hasE3);
}

TEST_F(EnPassantTest, EnPassantOnlyFromCorrectRank) {
    // White Pawn at e5 (rank 5 for white which is row 3).
    // Black Pawn adjacent at d5.
    // Even if PossibleEnPassant is set, en passant is only valid from ranks 4/5.
    board.loadFEN("4k3/8/8/3pP3/8/8/8/4K3 w - - 0 1");
    
    Pawn* bp = dynamic_cast<Pawn*>(Board::gameboxess[3][3].getPiece());
    ASSERT_NE(bp, nullptr);
    bp->PossibleEnPassant = true;
    
    Piece* wp = Board::gameboxess[4][3].getPiece();
    Pawn* whitePawn = dynamic_cast<Pawn*>(wp);
    
    std::set<Box*> moves = whitePawn->moveAndTake();
    
    // d6 (3,2) should be in legal moves (en passant capture)
    bool hasD6 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 3 && m->y / Board::BoxWidthandHigth == 2) hasD6 = true;
    }
    EXPECT_TRUE(hasD6);
}

TEST_F(EnPassantTest, NoEnPassantOnOwnPawn) {
    // White Pawns at e4 and d4. Should NOT be able to EP capture own pawn.
    board.loadFEN("4k3/8/8/8/3PP3/8/8/4K3 w - - 0 1");
    
    // Even if we set PossibleEnPassant (shouldn't happen in real game)
    Pawn* d4Pawn = dynamic_cast<Pawn*>(Board::gameboxess[3][4].getPiece());
    d4Pawn->PossibleEnPassant = true;
    
    Piece* wp = Board::gameboxess[4][4].getPiece();
    Pawn* e4Pawn = dynamic_cast<Pawn*>(wp);
    
    std::set<Box*> moves = e4Pawn->moveAndTake();
    
    // d3 (3,5) should NOT be in legal moves
    bool hasD3 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 3 && m->y / Board::BoxWidthandHigth == 5) hasD3 = true;
    }
    EXPECT_FALSE(hasD3);
}

