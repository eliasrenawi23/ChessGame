#include <gtest/gtest.h>
#include "Board.h"
#include "Queen.h"

class QueenTest : public ::testing::Test {
protected:
    Board board;
    void SetUp() override {
        board.init();
        board.clear();
    }
};

TEST_F(QueenTest, AllDirections) {
    // Queen at d4 (3,3)
    // Rook moves: 14
    // Bishop moves: 13
    // Total 27
    
    board.loadFEN("8/8/8/3Q4/8/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][3].getPiece();
    Queen* q = dynamic_cast<Queen*>(p);
    
    EXPECT_EQ(q->moveAndTake().size(), 27);
}

TEST_F(QueenTest, CornerA1) {
    // Queen at a1 (0,7)
    board.loadFEN("8/8/8/8/8/8/8/Q7 w - - 0 1");
    Piece* p = Board::gameboxess[0][7].getPiece();
    Queen* q = dynamic_cast<Queen*>(p);
    
    // Rook moves from corner: 14, Bishop moves: 7 = 21
    EXPECT_EQ(q->moveAndTake().size(), 21);
}

TEST_F(QueenTest, CornerH8) {
    // Queen at h8 (7,0)
    board.loadFEN("7Q/8/8/8/8/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[7][0].getPiece();
    Queen* q = dynamic_cast<Queen*>(p);
    
    EXPECT_EQ(q->moveAndTake().size(), 21);
}

TEST_F(QueenTest, BlockedByFriends) {
    // Queen at d4 with friends on adjacent squares
    board.loadFEN("8/8/8/2PPP3/2PQP3/2PPP3/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    Queen* q = dynamic_cast<Queen*>(p);
    
    // All 8 adjacent squares blocked by friends
    EXPECT_EQ(q->moveAndTake().size(), 0);
}

TEST_F(QueenTest, CapturesAllDirections) {
    // Queen at d4 with enemies on d1, d7, a4, h4, a7, g7, a1, g1
    board.loadFEN("8/p2p2p1/8/8/p2Q3p/8/8/p2p2p1 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    Queen* q = dynamic_cast<Queen*>(p);
    
    std::set<Box*> moves = q->moveAndTake();
    // Should include all captures plus intermediate squares
    EXPECT_GT(moves.size(), 8);
}

TEST_F(QueenTest, EdgeMiddle) {
    // Queen at a4
    board.loadFEN("8/8/8/8/Q7/8/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[0][4].getPiece();
    Queen* q = dynamic_cast<Queen*>(p);
    
    // Rook: 14, Bishop: 7 = 21
    EXPECT_EQ(q->moveAndTake().size(), 21);
}

TEST_F(QueenTest, MixedBlockAndCapture) {
    // Queen at d4, friend at c3, enemy at e5
    board.loadFEN("8/8/8/4p3/3Q4/2P5/8/8 w - - 0 1");
    Piece* p = Board::gameboxess[3][4].getPiece();
    Queen* q = dynamic_cast<Queen*>(p);
    
    std::set<Box*> moves = q->moveAndTake();
    
    // Cannot move to c3 (2,5) - friend
    // Can capture e5 (4,3) - enemy
    bool hitFriend = false, hitEnemy = false;
    for(auto m : moves) {
        int c = m->x / Board::BoxWidthandHigth;
        int r = m->y / Board::BoxWidthandHigth;
        if (c == 2 && r == 5) hitFriend = true;
        if (c == 4 && r == 3) hitEnemy = true;
    }
    EXPECT_FALSE(hitFriend);
    EXPECT_TRUE(hitEnemy);
}

