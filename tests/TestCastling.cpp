#include <gtest/gtest.h>
#include "Board.h"
#include "King.h"
#include "Rook.h"

class CastlingTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
        board.clear();
    }
};

TEST_F(CastlingTest, KingsideWhite) {
    // White King at e1, White Rook at h1. Empty f1, g1.
    // FEN: r3k2r/8/8/8/8/8/8/4K2R w Kkq - 0 1
    // (Added other pieces just to clear flags? No, standard FEN flags handle it).
    // Wait, my FEN parser sets up pieces. It implicitly sets firstMove=true for all new Pieces.
    // DOES IT? Piece constructor sets firstMove(true). So yes.
    
    board.loadFEN("r3k2r/8/8/8/8/8/8/4K2R w Kkq - 0 1");
    
    Piece* p = Board::gameboxess[4][7].getPiece(); // e1 (4,7) King
    ASSERT_NE(p, nullptr);
    King* k = dynamic_cast<King*>(p);
    ASSERT_NE(k, nullptr);
    
    // Check moves. Should include g1 (6,7).
    std::set<Box*> moves = k->moveAndTake();
    
    bool canCastle = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 6 && m->y / Board::BoxWidthandHigth == 7) canCastle = true;
    }
    EXPECT_TRUE(canCastle);
}

TEST_F(CastlingTest, QueensideWhite) {
    // White King e1, Rook a1. Empty b1, c1, d1.
    board.loadFEN("r3k2r/8/8/8/8/8/8/R3K3 w Qkq - 0 1");
    
    Piece* p = Board::gameboxess[4][7].getPiece(); // e1
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // Target is c1 (2,7)
    bool canCastle = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 2 && m->y / Board::BoxWidthandHigth == 7) canCastle = true;
    }
    EXPECT_TRUE(canCastle);
}

TEST_F(CastlingTest, Blocked) {
    // White King e1, Rook h1. Bishop at f1.
    board.loadFEN("r3k2r/8/8/8/8/8/8/4KB1R w Kkq - 0 1");
    
    Piece* p = Board::gameboxess[4][7].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    bool canCastle = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 6) canCastle = true;
    }
    EXPECT_FALSE(canCastle);
}

TEST_F(CastlingTest, KingMoved) {
    // White King e1, Rook h1.
    board.loadFEN("r3k2r/8/8/8/8/8/8/4K2R w Kkq - 0 1");
    
    Piece* p = Board::gameboxess[4][7].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    // Manually set firstMove = false
    k->firstMove = false;
    
    std::set<Box*> moves = k->moveAndTake();
    
    bool canCastle = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 6) canCastle = true;
    }
    EXPECT_FALSE(canCastle);
}

TEST_F(CastlingTest, RookMoved) {
    // White King e1, Rook h1.
    board.loadFEN("r3k2r/8/8/8/8/8/8/4K2R w Kkq - 0 1");
    
    Piece* p = Board::gameboxess[7][7].getPiece(); // h1 Rook
    Rook* r = dynamic_cast<Rook*>(p);
    r->firstMove = false;
    
    Piece* kp = Board::gameboxess[4][7].getPiece(); 
    King* k = dynamic_cast<King*>(kp);
    
    std::set<Box*> moves = k->moveAndTake();
    
    bool canCastle = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 6) canCastle = true;
    }
    EXPECT_FALSE(canCastle);
}

// ============ Black Castling Tests ============

TEST_F(CastlingTest, KingsideBlack) {
    // Black King at e8, Black Rook at h8. Empty f8, g8.
    board.loadFEN("r3k2r/8/8/8/8/8/8/4K2R b Kkq - 0 1");
    
    Piece* p = Board::gameboxess[4][0].getPiece(); // e8 (4,0)
    ASSERT_NE(p, nullptr);
    King* k = dynamic_cast<King*>(p);
    ASSERT_NE(k, nullptr);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // Target is g8 (6,0)
    bool canCastle = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 6 && m->y / Board::BoxWidthandHigth == 0) canCastle = true;
    }
    EXPECT_TRUE(canCastle);
}

TEST_F(CastlingTest, QueensideBlack) {
    // Black King e8, Rook a8.
    board.loadFEN("r3k2r/8/8/8/8/8/8/R3K3 b Qkq - 0 1");
    
    Piece* p = Board::gameboxess[4][0].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // Target is c8 (2,0)
    bool canCastle = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 2 && m->y / Board::BoxWidthandHigth == 0) canCastle = true;
    }
    EXPECT_TRUE(canCastle);
}

TEST_F(CastlingTest, BlackKingMoved) {
    board.loadFEN("r3k2r/8/8/8/8/8/8/4K2R b kq - 0 1");
    
    Piece* p = Board::gameboxess[4][0].getPiece();
    King* k = dynamic_cast<King*>(p);
    k->firstMove = false;
    
    std::set<Box*> moves = k->moveAndTake();
    
    bool canCastleKingside = false;
    bool canCastleQueenside = false;
    for(auto m : moves) {
        int col = m->x / Board::BoxWidthandHigth;
        if (col == 6) canCastleKingside = true;
        if (col == 2) canCastleQueenside = true;
    }
    EXPECT_FALSE(canCastleKingside);
    EXPECT_FALSE(canCastleQueenside);
}

TEST_F(CastlingTest, BlackRookMoved) {
    board.loadFEN("r3k2r/8/8/8/8/8/8/4K2R b kq - 0 1");
    
    Piece* rook = Board::gameboxess[7][0].getPiece(); // h8
    Rook* r = dynamic_cast<Rook*>(rook);
    r->firstMove = false;
    
    Piece* kp = Board::gameboxess[4][0].getPiece();
    King* k = dynamic_cast<King*>(kp);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // Kingside should be blocked
    bool canCastleKingside = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 6 && m->y / Board::BoxWidthandHigth == 0) canCastleKingside = true;
    }
    EXPECT_FALSE(canCastleKingside);
}

TEST_F(CastlingTest, BlackBlockedByPiece) {
    // Bishop at f8 blocking kingside
    board.loadFEN("r3kB1r/8/8/8/8/8/8/4K2R b kq - 0 1");
    
    Piece* kp = Board::gameboxess[4][0].getPiece();
    King* k = dynamic_cast<King*>(kp);
    
    std::set<Box*> moves = k->moveAndTake();
    
    bool canCastleKingside = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 6 && m->y / Board::BoxWidthandHigth == 0) canCastleKingside = true;
    }
    EXPECT_FALSE(canCastleKingside);
}

