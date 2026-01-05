#include <gtest/gtest.h>
#include "Board.h"
#include "King.h"

class KingTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
        board.clear();
    }
};

TEST_F(KingTest, BasicMovement) {
    // King at d4 (3,3) -> 8 moves
    board.loadFEN("8/8/8/3K4/8/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][3].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    EXPECT_EQ(k->moveAndTake().size(), 8);
}

TEST_F(KingTest, CornerMovement) {
    // King at a1 (0,7) -> 3 moves
    board.loadFEN("8/8/8/8/8/8/8/K7 w - - 0 1");
    Piece* p = Board::gameboxess[0][7].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    EXPECT_EQ(k->moveAndTake().size(), 3);
}

TEST_F(KingTest, CornerH8) {
    // King at h8 (7,0) -> 3 moves
    board.loadFEN("7K/8/8/8/8/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[7][0].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    EXPECT_EQ(k->moveAndTake().size(), 3);
}

TEST_F(KingTest, EdgeMiddle) {
    // King at a4 (0,4) -> 5 moves
    board.loadFEN("8/8/8/8/K7/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[0][4].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    EXPECT_EQ(k->moveAndTake().size(), 5);
}

TEST_F(KingTest, SurroundedByFriends) {
    // King at d4 surrounded by 8 friendly pawns
    board.loadFEN("8/8/8/2PPP3/2PKP3/2PPP3/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    EXPECT_EQ(k->moveAndTake().size(), 0);
}

TEST_F(KingTest, CaptureEnemy) {
    // King at d4, enemy pawn at e5
    board.loadFEN("8/8/8/4p3/3K4/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    // Should include capture at e5 (4,3)
    bool hasE5 = false;
    for(auto m : moves) {
        int c = m->x / Board::BoxWidthandHigth;
        int r = m->y / Board::BoxWidthandHigth;
        if (c == 4 && r == 3) hasE5 = true;
    }
    EXPECT_TRUE(hasE5);
}

TEST_F(KingTest, MixedFriendAndEnemy) {
    // King at d4, friend at c3, enemy at e3
    board.loadFEN("8/8/8/8/3K4/2P1p3/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    std::set<Box*> moves = k->moveAndTake();
    
    bool hitFriend = false, hitEnemy = false;
    for(auto m : moves) {
        int c = m->x / Board::BoxWidthandHigth;
        int r = m->y / Board::BoxWidthandHigth;
        if (c == 2 && r == 5) hitFriend = true;
        if (c == 4 && r == 5) hitEnemy = true;
    }
    EXPECT_FALSE(hitFriend);
    EXPECT_TRUE(hitEnemy);
}

TEST_F(KingTest, EdgeH1) {
    // King at h1 (7,7) -> 3 moves
    board.loadFEN("8/8/8/8/8/8/8/7K w - - 0 1");
    Piece* p = Board::gameboxess[7][7].getPiece();
    King* k = dynamic_cast<King*>(p);
    
    EXPECT_EQ(k->moveAndTake().size(), 3);
}

