#include <gtest/gtest.h>
#include "Board.h"
#include "King.h"

class CheckMateTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
        board.clear();
    }
};

TEST_F(CheckMateTest, CheckEscapes) {
    // White King at e1. Black Rook at e8.
    // King is in check.
    // Valid moves: d1, f1 (side), d2, e2, f2? 
    // e2 covered by Rook? No, rook at e8 attacks e file.
    // So e2 is attacked.
    // e1 attacked.
    // Escape: d1, f1? 
    // If d1 (3,7) is safe.
    // If f1 (5,7) is safe.
    // e2, d2, f2?
    // d2 (3,6) safe.
    // f2 (5,6) safe.
    // e2 (4,6) attacked.
    
    board.loadFEN("4r3/8/8/8/8/8/8/4K3 w - - 0 1");
    
    Piece* p = Board::gameboxess[4][7].getPiece(); // King
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // Should NOT contain e2 (4,6).
    bool hasE2 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 4 && m->y / Board::BoxWidthandHigth == 6) hasE2 = true;
    }
    EXPECT_FALSE(hasE2);
    
    // Should contain d1 (3,7)
    bool hasD1 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 3 && m->y / Board::BoxWidthandHigth == 7) hasD1 = true;
    }
    EXPECT_TRUE(hasD1);
}

TEST_F(CheckMateTest, FoolsMate) {
    // Quickest mate:
    // f3 e5 g4 Qh4#
    // Setup FEN for final position:
    // r1bqkbnr/pppp1ppp/2n5/4p3/6P1/5P2/PPPPP2P/RNBQKBNR b KQkq - 0 1
    // (Wait, that's regular pieces).
    // Let's force a simpler mate.
    // King e1. Black Rooks d2, f2 (impossible but ok).
    // Queen e2?
    // Let's use standard Back Rank mate.
    // White King h1. Pawns f2, g2, h2.
    // Black Rook at d1?
    // FEN: 6k1/8/8/8/8/8/5PPP/3r2K1 w - - 0 1
    
    board.loadFEN("6k1/8/8/8/8/8/5PPP/3r2K1 w - - 0 1");
    // White to move.
    // King at h1 (7,7).
    // Rook at d1 (3,7). Attacks rank 7? No, rank 1 (row 7).
    // King Cannot move to g1 (6,7) (occupied by Rook attack? No, d1 attacks row 7).
    // King Cannot move to h2 (7,6) (occupied by pawn).
    // King Cannot move to g2 (5,6)? No g2 (6,6) occupied.
    
    // Expect checkmate flag?
    // Board::checkresult() sets flags.
    // But checkresult is private.
    // We can infer from available moves.
    // King moves should be empty.
    
    // King is at g1 (6,7) per FEN "3r2K1"
    Piece* p = Board::gameboxess[6][7].getPiece();
    ASSERT_NE(p, nullptr);
    King* k = dynamic_cast<King*>(p);
    ASSERT_NE(k, nullptr);
    
    std::set<Box*> moves = k->moveAndTake();
    EXPECT_TRUE(moves.empty());
    
    // Can any other piece move?
    // Pawns g2, h2 are blocked?
    // g2 (6,6). One forward g3 (6,5).
    // Does it block check?
    // Check is on rank 1. Moving g2->g3 doesn't help.
    // So NO legal moves for White.
    // This is Checkmate.
}

TEST_F(CheckMateTest, KingCannotMoveIntoCheck) {
    // White King at d4. Black Rooks at a1 and a8.
    // King cannot move to any square on files a-c (ranks attack).
    board.loadFEN("r3k3/8/8/8/3K4/8/8/r7 w - - 0 1");
    
    Piece* p = Board::gameboxess[3][4].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // Should NOT contain any squares on column 0 (a-file)
    for(auto m : moves) {
        EXPECT_NE(m->x / Board::BoxWidthandHigth, 0);
    }
}

TEST_F(CheckMateTest, KingCanCaptureAttacker) {
    // White King at e1. Black Rook at e2 (adjacent).
    // King can capture the Rook.
    board.loadFEN("4k3/8/8/8/8/8/4r3/4K3 w - - 0 1");
    
    Piece* p = Board::gameboxess[4][7].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // Should contain e2 (4,6) - King can capture undefended Rook
    bool hasE2 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 4 && m->y / Board::BoxWidthandHigth == 6) hasE2 = true;
    }
    EXPECT_TRUE(hasE2);
}

TEST_F(CheckMateTest, KingCannotCaptureDefendedPiece) {
    // White King at e1. Black Rook at e2. Black Bishop at b5 defending e2.
    board.loadFEN("4k3/8/8/1b6/8/8/4r3/4K3 w - - 0 1");
    
    Piece* p = Board::gameboxess[4][7].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // Should NOT contain e2 (4,6) - Rook is defended
    bool hasE2 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 4 && m->y / Board::BoxWidthandHigth == 6) hasE2 = true;
    }
    EXPECT_FALSE(hasE2);
}

TEST_F(CheckMateTest, DoubleCheck) {
    // King in double check - can only move, cannot block or capture.
    // White King e1. Black Rook e8 and Black Knight c2 (both giving check).
    board.loadFEN("4r3/8/8/8/8/8/2n5/4K3 w - - 0 1");
    
    Piece* p = Board::gameboxess[4][7].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // King must escape. Available squares: d1, f1, d2, f2
    // e2 is attacked by Rook.
    // d2 is attacked by Knight? c2 Knight attacks b4, d4, a3, e3, a1, e1. NOT d2.
    // f2? Safe.
    // d1? Safe.
    // f1? Safe.
    
    EXPECT_FALSE(moves.empty());
    
    // e2 should not be in moves
    bool hasE2 = false;
    for(auto m : moves) {
        if (m->x / Board::BoxWidthandHigth == 4 && m->y / Board::BoxWidthandHigth == 6) hasE2 = true;
    }
    EXPECT_FALSE(hasE2);
}

TEST_F(CheckMateTest, Stalemate) {
    // Black King at a8, White Queen at b6, White King at c6.
    // Black to move but has no legal moves (not in check).
    board.loadFEN("k7/8/1QK5/8/8/8/8/8 b - - 0 1");
    
    Piece* p = Board::gameboxess[0][0].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // King at a8 (0,0). Adjacent squares: a7, b8, b7.
    // a7 (0,1) - attacked by Queen at b6.
    // b8 (1,0) - attacked by Queen at b6.
    // b7 (1,1) - attacked by both Queen and King.
    // All moves blocked!
    
    EXPECT_TRUE(moves.empty());
}

TEST_F(CheckMateTest, ScholarsMate) {
    // Classic 4-move checkmate position
    // FEN after 1.e4 e5 2.Bc4 Nc6 3.Qh5 Nf6?? 4.Qxf7#
    board.loadFEN("r1bqkb1r/pppp1Qpp/2n2n2/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 4");
    
    Piece* p = Board::gameboxess[4][0].getPiece(); // Black King at e8
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // King is checkmated - no escape squares
    EXPECT_TRUE(moves.empty());
}

TEST_F(CheckMateTest, BugRepro_CanBlockCheck) {
    // White King at e1. Black Queen at h4.
    // White Pawn at g2.
    // The King is in check. coverPath should include g3 (6,5) and f2? No, check checks diagonals?
    // Queen h4 (7,4) -> e1 (4,7).
    // Path: f2 (5,6), g3 (6,5).
    // So coverPath MUST contain g3 and f2.
    
    board.loadFEN("rnb1kbnr/pppp1ppp/8/8/7q/8/PPPPP1PP/RNBQKBNR w KQkq - 0 1");
    
    // Check if King is in check
    Piece* king = Board::gameboxess[4][7].getPiece();
    ASSERT_TRUE(dynamic_cast<King*>(king));
    King* k = dynamic_cast<King*>(king);
    
    // Verify coverpath
    std::set<Box*> coverPath = k->getCoverPath();
    
    // Expect g3 (6,5) to be in coverPath
    bool hasG3 = false;
    for(auto m : coverPath) {
        if (m->x / Board::BoxWidthandHigth == 6 && m->y / Board::BoxWidthandHigth == 5) {
            hasG3 = true;
        }
    }
    
    // This assertion should fail if the bug exists (and it returns empty or wrong path)
    EXPECT_TRUE(hasG3) << "King's cover path should include g3 to block Queen from h4.";
}

