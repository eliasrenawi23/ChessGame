#include "Board.h"
#include "Window.h"

int Board::BoxWidthandHigth;
std::vector<std::vector<Box >> Board::gameboxess;
Board::Board() : whitePlayer(NULL), blackPlayer(NULL), playerTurn(true), selectedBox(NULL),
checkmate(false), En_passantPawn(NULL), promotion(false), promotionBox(NULL)
{
}
bool Board::isInBounds(int x, int y)
{
	 return x >= 0 && y >= 0 && x < rowBoxNmbersandCols && y < rowBoxNmbersandCols;;
}
void Board::init()
{	
	gameboxess.clear();
	BoxWidthandHigth = std::min(Window::SCREEN_HEIGHT, Window::SCREEN_WIDTH) / rowBoxNmbersandCols;//get the min between them

	for (int i = 0; i < rowBoxNmbersandCols; i++)
	{
		std::vector<Box> temp;
		for (int j = 0; j < rowBoxNmbersandCols; j++)
		{
			temp.emplace_back(i * BoxWidthandHigth, j * BoxWidthandHigth, BoxWidthandHigth, (i + j) % 2 == 0 ? Color1 : Color2);

		}
		gameboxess.push_back(std::move(temp));

	}
	whitePlayer = new Player(PlayerColor::WHITE);
	whitePlayer->init();
	blackPlayer = new Player(PlayerColor::BLACK);
	blackPlayer->init();


}

void Board::resize()
{
	BoxWidthandHigth = std::min(Window::SCREEN_HEIGHT, Window::SCREEN_WIDTH) / rowBoxNmbersandCols;


	for (int i = 0; i < rowBoxNmbersandCols; i++)
	{
		for (int j = 0; j < rowBoxNmbersandCols; j++)
		{
			gameboxess[i][j].resize(i * BoxWidthandHigth, j * BoxWidthandHigth, BoxWidthandHigth);
		}
	}


}


void Board::clear() {
	if (whitePlayer) whitePlayer->clear();
	if (blackPlayer) blackPlayer->clear();
	for (auto& row : gameboxess) {
		for (auto& box : row) {
			box.setPiece(nullptr);
		}
	}
	boxtoLight.clear();
	oldPieces.clear();
	history.clear();
    history.reserve(200); // Reserve memory to verify if reallocation is the issue
    currentMoveIndex = 0;
	selectedBox = nullptr;
	promotionBox = nullptr;
	En_passantPawn = nullptr;
	promotion = false;
	checkmate = false;
}

void Board::loadFEN(std::string fen) {
	clear();
	
	// Ensure players exist
	if (!whitePlayer) whitePlayer = new Player(PlayerColor::WHITE);
	if (!blackPlayer) blackPlayer = new Player(PlayerColor::BLACK);

	int row = 0;
	int col = 0;
	size_t i = 0;

	// 1. Piece placement
	for (; i < fen.length(); i++) {
		char c = fen[i];
		if (c == ' ') break;
		if (c == '/') {
			row++;
			col = 0;
		}
		else if (isdigit(c)) {
			col += (c - '0');
		}
		else {
			PlayerColor pieceColor = (isupper(c)) ? PlayerColor::WHITE : PlayerColor::BLACK;
			Piece* newPiece = nullptr;
			Box* box = &gameboxess[col][row];

			char lowerC = tolower(c);
			switch (lowerC) {
				case 'p': newPiece = new Pawn(box, pieceColor); break;
				case 'r': newPiece = new Rook(box, pieceColor); break;
				case 'n': newPiece = new Knight(box, pieceColor); break;
				case 'b': newPiece = new Bishop(box, pieceColor); break;
				case 'q': newPiece = new Queen(box, pieceColor); break;
				case 'k': newPiece = new King(box, pieceColor); break;
			}

			if (newPiece) {
				box->setPiece(newPiece);
				(pieceColor == PlayerColor::WHITE ? whitePlayer : blackPlayer)->addPiece(newPiece);
			}
			col++;
		}
	}

	// 2. Turn (w/b) - Basic implementation, can expand later
	i++; // skip space
	if (i < fen.length()) {
		playerTurn = (fen[i] == 'w');
	}
	// Further fields (castling, en passant, etc.) can be added here
}

void Board::getLegalMovs(int cor_x, int cor_y)
{
	int box_x = cor_x / (Window::SQUARE_SIZE / 8);
	int box_y = cor_y / (Window::SQUARE_SIZE / 8);

	if (cor_x > Window::SQUARE_SIZE || cor_y > Window::SQUARE_SIZE) {
		std::cout << " outside " << box_x << " " << box_y << std::endl;
		return;
	}
	//std::cout << " box_x box_y MOUSE BUTTON Down : " << box_x << " " << box_y << std::endl;
	selectedBox = &gameboxess[box_x][box_y]; //selected box that contain piece to play with
	
	// Check if click on Undo/Redo area (outside board) - logic will be in Window.cpp but safeguards here?
	// The board logic assumes 0-7. Window maps clicks.
	// If Window handles coordinates, Board only receives 0-7.
	// We'll rely on Window checking bounds before calling getLegalMovs/play or handling the side panel separately.

	if (!selectedBox->getPiece()|| promotion) { //this vox has no pice in it no action needed
		std::cout << " this vox has no pice in it no action needed or it is promotion " << std::endl;
		return;
	}

	if (playerTurn && (selectedBox->getPiece()->getColor() == PlayerColor::WHITE)) {
		//to do get the player move
		boxtoLight = whitePlayer->play(selectedBox->getPiece(), &checkmate);
		//highlightKing(checkmate, whitePlayer);

	}
	else if (!playerTurn && (selectedBox->getPiece()->getColor() == PlayerColor::BLACK)) {
		//to do get the player move
		boxtoLight = blackPlayer->play(selectedBox->getPiece(), &checkmate);
		// highlightKing(checkmate, blackPlayer);

	}

	checkresult();
	highlightboxs(true);


}
void  Board::play(int cor_x, int cor_y) {
	int box_x = cor_x / (Window::SQUARE_SIZE / 8);
	int box_y = cor_y / (Window::SQUARE_SIZE / 8);
	if (cor_x > Window::SQUARE_SIZE || cor_y > Window::SQUARE_SIZE) {
		std::cout << " out of range " << box_x << " " << box_y << std::endl;
		return;
	}
	if (boxtoLight.empty()) {
        bool pre = promotion;
		handle_promotion(box_x, box_y, &promotion);
        
        if (pre && !promotion) {
             // Promotion finished. Update history.
              if (!history.empty()) {
                   Move& last = history.back();
                   last.promotedPiece = promotionBox->getPiece();
              }
        }
		return;
	}
	//if player didnot change the piece location
	if (boxtoLight.empty() || (selectedBox->x / (Window::SQUARE_SIZE / 8) == box_x && selectedBox->y / (Window::SQUARE_SIZE / 8) == box_y)) {
		highlightboxs(false);
		boxtoLight.clear();
		return;
	}

	Box* targetBox = &gameboxess[box_x][box_y]; // the box to play to 
	auto pos = boxtoLight.find(targetBox);
	if (pos != boxtoLight.end()) {
        std::cout << "Board::play: Move confirmed to " << box_x << "," << box_y << std::endl;
        
        // --- RECORD MOVE START ---
        // Clear any future history if we are in the middle of undo/redo stack
        if (currentMoveIndex < history.size()) {
            // Delete any promoted pieces in the redo stack to prevent leaks!
            for(size_t i = currentMoveIndex; i < history.size(); i++) {
                if (history[i].promotedPiece) delete history[i].promotedPiece;
                // DO NOT delete capturedPiece! It was restored to the board by Undo.
            }
            history.resize(currentMoveIndex);
        }

        Move m;
        m.from = selectedBox;
        m.to = targetBox;
        m.movedPiece = selectedBox->getPiece();
        m.wasFirstMove = m.movedPiece->firstMove;
        m.capturedPiece = nullptr;
        m.capturedPieceLocation = targetBox;
        m.isEnPassant = false;
        m.isCastling = false;
        m.isPromotion = false;
        m.promotedPiece = nullptr;

        // Capture logic
		if ((*pos)->getPiece() != NULL) {
            // Standard capture
			m.capturedPiece = (*pos)->getPiece();
			deletepiece((*pos));
		}
		
        // En Passant check
        if (m.capturedPiece == nullptr && dynamic_cast<Pawn*>(m.movedPiece) && m.from->x != m.to->x) {
             m.isEnPassant = true;
             // Where is the captured pawn?
             // Standard En Passant: Target is at [to_x][from_y].
             int capturedX = box_x;
             int capturedY = m.from->y / (Window::SQUARE_SIZE / 8);
             Box* capBox = &gameboxess[capturedX][capturedY];
             m.capturedPiece = capBox->getPiece();
             m.capturedPieceLocation = capBox;
        }

		En_passant(box_x, box_y);
        
		UpdatePieceLocation(selectedBox, (*pos));
		
        // Check Castling
        // If King moved 2 squares.
        if (dynamic_cast<King*>(m.movedPiece) && abs(m.from->x / (Window::SQUARE_SIZE / 8) - box_x) == 2) {
            m.isCastling = true;
        }
        CastleMove(box_x, box_y);		//check if Castle

        // Promotion check
        // handle_promotion(box_x, box_y, &promotion); calls dopromotion if promotion is true.
        // But the first call sets 'promotion' flag and waits for user input.
        // If 'promotion' becomes true, IT IS NOT A FULL MOVE YET.
        // The move completes only when the promotion piece is selected.
        
        // Crucial: The game flow for promotion is:
        // 1. Pawn moves to end. `handle_promotion` called. Sets `promotion = true`.
        // 2. `play` returns. User sees promotion menu.
        // 3. User clicks on menu. `play` called again. `handle_promotion` entered with `promotion=true`.
        // 4. `dopromotion` called.
        
        // So I should only record the move when the move is Finalized.
        // If `promotion` becomes true after this block, we shouldn't push to history yet?
        // OR we treat the Pawn move as one half, and promotion as another?
        // Better: Treat the whole sequence as one atomic move in history.
        // We only push to history when the turn flips.
        
        bool prePromotion = promotion;
		handle_promotion(box_x, box_y, &promotion);
        
        if (promotion && !prePromotion) {
            // We just entered promotion menu mode.
            // Do NOT record history yet. The move is not complete.
            // But `UpdatePieceLocation` already happened. The pawn is at the end.
            // If I click Undo now, it should undo the pawn move.
            // So yes, record it.
            // BUT we haven't promoted yet. `m.promotedPiece` is null. `m.isPromotion` is true (intent).
            // When we finish promotion (next click), we update this history entry?
            // Or we have a "pending" move?
            
            // Simplest: Record the Pawn move now.
            // When promotion happens (next turn/click), we EDIT the last history entry to include the promoted piece and swap the movedPiece?
            // Yes.
            m.isPromotion = true;
            m.san = calculateSAN(selectedBox, targetBox, m);
            history.push_back(m);
            currentMoveIndex++;
        } else if (prePromotion && !promotion) {
            // We just FINISHED promotion.
            // We need to update the LAST move in history.
            if (!history.empty()) {
               Move& last = history.back();
               last.promotedPiece = targetBox->getPiece(); // The new Queen
               last.san = calculateSAN(last.from, last.to, last);
               // The old movedPiece (Pawn) was already recorded.
               // We are done.
            }
        } else {
            // Normal move
            if (!promotion) {
               m.san = calculateSAN(selectedBox, targetBox, m);
               history.push_back(m);
               currentMoveIndex++;
            }
        }
        

        // --- RECORD MOVE END ---

		selectedBox->setPiece(NULL);

        if (!promotion) { // Only flip turn if not waiting for promotion selection
		    playerTurn = !playerTurn; //change turns	
		    checkmate = false;
		    if (playerTurn) {
			    whitePlayer->setopponentThreatMap(blackPlayer->ClacThreatMap(&checkmate));
		    }
		    else {
			    blackPlayer->setopponentThreatMap(whitePlayer->ClacThreatMap(&checkmate));
		    }
        }
		highlightKing();
	}

	highlightboxs(false);
	boxtoLight.clear();

}

void Board::undo() {
    std::cout << "Board::undo called. currentMoveIndex: " << currentMoveIndex << std::endl;
    if (currentMoveIndex == 0) return;
    
    // If we are in the middle of a promotion selection (promotion == true), cancel it first?
    if (promotion) {
        // Cancel promotion mode
        // Restore old pieces
        for (int i = 2; i < 6; i++) {   //save the old piesess
			gameboxess[i][4].boxColor = gameboxess[i][4].originalColor;
			gameboxess[i][4].setPiece(oldPieces[i - 2]); //return the piecess
		}
		oldPieces.clear();
        promotion = false;
        playerTurn = !playerTurn; // Flip back to the player who was promoting
        // Now fall through to undo the pawn move itself
    }

    currentMoveIndex--;
    std::cout << "Undo: Decremented index to " << currentMoveIndex << ". History size: " << history.size() << std::endl;
    Move& m = history[currentMoveIndex];

    // 1. Restore Piece Position
    if (m.isPromotion && m.promotedPiece) {
        // Remove the promoted piece (e.g. Queen)
        deletepiece(m.to); 
        m.promotedPiece = nullptr; // It's gone from board, simplified. Or should we guard inside deletepiece?
        // Wait, if deletepiece doesn't delete, we are fine.
        // Actually, if we undo, we want to destroy the promoted piece because Redo will recreate it (via promotion logic? No, Redo should restore it).
        // If Redo restores it, we should NOT delete it. We detach it.
        // My deletepiece implementation detach. So m.promotedPiece is holding the pointer.
    }
    
    // Move the original piece back
    Box* fromBox = m.from;
    Box* toBox = m.to;
    
    std::cout << "Undo: Restoring piece from " << toBox << " to " << fromBox << std::endl;

    // Determine which piece is currently at 'toBox'.
    // If promotion, it's empty (we just detached promotedPiece).
    // If normal, it's 'movedPiece'.
    
    if (!m.isPromotion) {
        // Detach movedPiece from 'to'
        toBox->setPiece(nullptr);
    }
    
    // Place movedPiece back at 'from'
    if (m.movedPiece) {
        std::cout << "Undo: Setting piece " << m.movedPiece << " to fromBox" << std::endl;
        fromBox->setPiece(m.movedPiece);
        m.movedPiece->setLocation(fromBox);
        m.movedPiece->firstMove = m.wasFirstMove;
    } else {
        std::cout << "ERROR: movedPiece is NULL" << std::endl;
    }

    // 2. Restore Captured Piece
    if (m.capturedPiece) {
        std::cout << "Undoing capture. Piece: " << m.capturedPiece 
                  << " Location: " << m.capturedPieceLocation << std::endl;
        
        if (m.capturedPieceLocation) {
             m.capturedPieceLocation->setPiece(m.capturedPiece);
        } else {
             std::cout << "ERROR: capturedPieceLocation is NULL" << std::endl;
        }

        if (m.capturedPiece) {
             m.capturedPiece->setLocation(m.capturedPieceLocation);
             std::cout << "Restored location. Color: " << (int)m.capturedPiece->getColor() << std::endl;
             // Add back to player's vector
             (m.capturedPiece->getColor() == PlayerColor::WHITE ? whitePlayer : blackPlayer)->addPiece(m.capturedPiece);
        } else {
             std::cout << "ERROR: capturedPiece is NULL (impossible inside if)" << std::endl;
        }
    }

    // 3. Undo Castling
    if (m.isCastling) {
        // Rook was moved. We need to move it back.
        // King moved 2 squares. We already moved King back via 'movedPiece'.
        // Identify Rook move.
        // If King moved to x+2 (Kingside), Rook moved from x+3 to x+1.
        // If King moved to x-2 (Queenside), Rook moved from x-4 to x-1.
        int dx = (toBox->x - fromBox->x) / (Window::SQUARE_SIZE / 8);
        int y = fromBox->y / (Window::SQUARE_SIZE / 8); 
        
        if (dx == 2) { // Kingside
            // Rook at x+1 (5->4 or 7->5? Indices: King 4. Move to 6. Rook 7 moves to 5.)
            // King is at 4 now (restored). Rook is at 5. Move to 7.
            // Wait, coordinates.
            // White King: 4,7. Castles to 6,7. Rook 7,7 to 5,7.
            // Undo: King 6,7 -> 4,7. Rook 5,7 -> 7,7.
            Box* rookCurrent = &gameboxess[5][y];
            Box* rookOriginal = &gameboxess[7][y];
            UpdatePieceLocation(rookCurrent, rookOriginal);
        } else if (dx == -2) { // Queenside
             // King 4,7 -> 2,7. Rook 0,7 -> 3,7.
             // Undo: King 2,7 -> 4,7. Rook 3,7 -> 0,7.
             Box* rookCurrent = &gameboxess[3][y];
             Box* rookOriginal = &gameboxess[0][y];
             UpdatePieceLocation(rookCurrent, rookOriginal);
        }
    }

    // 4. Reset Turn
    playerTurn = !playerTurn;
    checkmate = false;
    // Re-calc threats?
    if (playerTurn) {
        whitePlayer->setopponentThreatMap(blackPlayer->ClacThreatMap(&checkmate));
    } else {
        blackPlayer->setopponentThreatMap(whitePlayer->ClacThreatMap(&checkmate));
    }
    highlightKing();
}

void Board::redo() {
    if (currentMoveIndex >= history.size()) return;
    
    // Get the move
    Move& m = history[currentMoveIndex];
    currentMoveIndex++;
    
    // 1. Move Piece
    // Remove from 'from'
    m.from->setPiece(nullptr);
    
    // Handle Capture
    if (m.capturedPiece) {
        deletepiece(m.capturedPieceLocation);
    }
    
    // valid En Passant capture handled by above info (capturedPieceLocation)
    
    // Place piece at 'to'
    if (m.isPromotion && m.promotedPiece) {
        // Place the promoted piece instead
        m.to->setPiece(m.promotedPiece);
        m.promotedPiece->setLocation(m.to);
        // Add to Player vector? It was probably removed by Undo.
        (m.promotedPiece->getColor() == PlayerColor::WHITE ? whitePlayer : blackPlayer)->addPiece(m.promotedPiece);
        
        // The original Pawn is effectively gone (detached).
    } else {
        m.to->setPiece(m.movedPiece);
        m.movedPiece->setLocation(m.to);
        m.movedPiece->firstMove = false; // By definition if we redo a move, it's not first (or it consumed it)
    }

    // 2. Redo Castling
    if (m.isCastling) {
         int dx = (m.to->x - m.from->x) / (Window::SQUARE_SIZE / 8);
         int y = m.from->y / (Window::SQUARE_SIZE / 8);
         
         if (dx == 2) { // Kingside
            // Move Rook 7 -> 5
            UpdatePieceLocation(&gameboxess[7][y], &gameboxess[5][y]);
         } else if (dx == -2) { // Queenside
            // Move Rook 0 -> 3
            UpdatePieceLocation(&gameboxess[0][y], &gameboxess[3][y]);
         }
    }
    
    // 3. Flip Turn
    playerTurn = !playerTurn;
    checkmate = false;
    if (playerTurn) {
        whitePlayer->setopponentThreatMap(blackPlayer->ClacThreatMap(&checkmate));
    } else {
        blackPlayer->setopponentThreatMap(whitePlayer->ClacThreatMap(&checkmate));
    }
    highlightKing();
}

void Board::En_passant(int new_x, int new_y) {

	Box* b = selectedBox; // the box to play to /// befor the move 
	if (Pawn* pawn = dynamic_cast<Pawn*>(b->getPiece())) {
		int diraction = (b->getPiece()->getColor() == PlayerColor::WHITE) ? 1 : -1;//if white move -1 id black move 1


		if (pawn->firstMove) {
			if (En_passantPawn != NULL)
				En_passantPawn->PossibleEnPassant = false;
			pawn->PossibleEnPassant = true;
			En_passantPawn = pawn;
			return;
		}
		int old_x = selectedBox->x / (Window::SQUARE_SIZE / 8);
		int old_y = selectedBox->y / (Window::SQUARE_SIZE / 8);

		en_passasntDelete(old_x - 1, old_y, new_x, new_y, diraction);
		en_passasntDelete(old_x + 1, old_y, new_x, new_y, diraction);
		En_passantPawn->PossibleEnPassant = false;

	}

}
void Board::en_passasntDelete(int old_x, int old_y, int new_x, int new_y, int diraction)
{
	if (old_x >= 0 && old_x < rowBoxNmbersandCols) {
		Box* b = &gameboxess[old_x][old_y]; //left box
		if (Pawn* pawn = dynamic_cast<Pawn*>(b->getPiece())) {
			if (pawn->PossibleEnPassant && (old_x == new_x && new_y == old_y - diraction)) {
				// pawn->~Pawn(); // Removed explicit destructor call
				deletepiece(b);
				return;
			}
		}
	}
}

void Board::CastleMove(int new_x, int new_y) {

	Box* k = &gameboxess[new_x][new_y]; // the box to play to 
	if (!dynamic_cast<King*>(k->getPiece()))return;

	int old_x = selectedBox->x / (Window::SQUARE_SIZE / 8);
	int old_y = selectedBox->y / (Window::SQUARE_SIZE / 8);


	if (old_y == new_y && new_x - 2 == old_x) { // king Castle 
		UpdatePieceLocation(&gameboxess[7][new_y], &gameboxess[5][new_y]);
	}
	else if (old_y == new_y && new_x + 2 == old_x) {// queen Castle 
		UpdatePieceLocation(&gameboxess[0][new_y], &gameboxess[3][new_y]);
	}
}



void Board::handle_promotion(int box_x, int box_y, bool* promotion)
{
	Box* targetBox = &gameboxess[box_x][box_y];
	if ((box_y == 0 || box_y == 7) && !*promotion && dynamic_cast<Pawn*>(targetBox->getPiece())) {// initilaize promotions
		//prepare promotion
		for (int i = 2; i < 6; i++) {   //save the old piesess
			oldPieces.push_back(gameboxess[i][4].getPiece());
			gameboxess[i][4].boxColor = { 160, 32, 240 ,SDL_ALPHA_OPAQUE };

		}
		//send to player class
		(playerTurn ? whitePlayer : blackPlayer)->handle_promotion(true);
		*promotion = true;
		promotionBox = targetBox;
		playerTurn = !playerTurn; //we flip the turn twice to bak to current player

	}
	else if (*promotion)
	{
		Piece* p = selectedBox->getPiece();//what the player selected queen ? bishop rook?...
		if (!p)return;
		int promotion_x = promotionBox->x / (Window::SQUARE_SIZE / 8);
		int promotion_y = promotionBox->y / (Window::SQUARE_SIZE / 8);
		deletepiece(promotionBox);
		(!playerTurn ? whitePlayer : blackPlayer)->dopromotion(p, promotion_x, promotion_y);
		*promotion = false;

		//remove the list
		for (int i = 2; i < 6; i++) {   //save the old piesess
			gameboxess[i][4].boxColor = gameboxess[i][4].originalColor;
			gameboxess[i][4].setPiece(oldPieces[i - 2]); //return the piecess
		}
		oldPieces.clear();
		playerTurn = !playerTurn; //we flip the turn twice to bak to current player

	}

}



void Board::UpdatePieceLocation(Box* from, Box* to)
{
	from->getPiece()->setLocation(to);   ///chenge the location of the piece
	to->setPiece(from->getPiece());
	from->setPiece(nullptr);
}

void Board::deletepiece(Box* b)
{
	Piece* p = b->getPiece();
	//Player* player = (playerTurn) ? blackPlayer : whitePlayer;
	
    // Don't delete! Transfers ownership to history.
	// delete p;
	
    // But remove from Player's active list
    if (p) {
	    blackPlayer->updateVectorPieces(p);
	    whitePlayer->updateVectorPieces(p);
	    b->setPiece(nullptr);
    }
}

void Board::highlightboxs(bool onOrOff) {

	for (Box* b : boxtoLight) {
		b->boxColor = onOrOff ? SDL_Color{ 64, 191, 255, SDL_ALPHA_OPAQUE } : b->originalColor;
	}

}
void Board::RenderBoard()
{
	for (auto& row : gameboxess) {
		for (auto& box : row) {
			box.RenderBox();
		}
	}
	RenderPieces();
}
void Board::RenderPieces()
{
	for (Piece* p : blackPlayer->getPieces()) {
		if (p) p->renderPiece();
	}

	for (Piece* p : whitePlayer->getPieces()) {
		if (p) p->renderPiece();
	}
	for (Piece* p : (playerTurn ? whitePlayer->getPiecesOptions() : blackPlayer->getPiecesOptions())) {
		if (p) p->renderPiece();
	}
}


void Board::checkresult()
{
	if (boxtoLight.size() == 1 && *boxtoLight.begin() == selectedBox) {
		std::cout << "Stalemate: no one wins." << std::endl;
	}
	else if (boxtoLight.empty() && checkmate) {
		std::cout << (playerTurn ? "BLACK" : "WHITE") << " is the winner." << std::endl;
	}


}
void Board::highlightKing()
{
	King* k = playerTurn ? whitePlayer->getKing() : blackPlayer->getKing();
	Box* b = k->getLocation();
	checkmate ? b->boxColor = { 255,0,0,0 } : b->boxColor = b->originalColor;

}
Board::~Board() {

	delete whitePlayer;
	delete blackPlayer;
}

std::string Board::calculateSAN(Box* from, Box* to, const Move& partialMove) {
    if (!partialMove.movedPiece) return "";

    std::string san = "";
    Piece* p = partialMove.movedPiece;
    Player* player = (p->getColor() == PlayerColor::WHITE) ? whitePlayer : blackPlayer;

    int fromCol = from->x / (Window::SQUARE_SIZE / 8);
    int fromRow = from->y / (Window::SQUARE_SIZE / 8);
    int toCol = to->x / (Window::SQUARE_SIZE / 8);
    int toRow = to->y / (Window::SQUARE_SIZE / 8);
    
    char colChar = 'a' + toCol;
    char rowChar = '8' - toRow;

    // Piece Type Logic
    if (dynamic_cast<Pawn*>(p)) {
        if (partialMove.capturedPiece || partialMove.isEnPassant) {
             san += (char)('a' + fromCol);
             san += "x";
        }
        // Promotion handled at end
    } else {
        std::string pieceLetter = "";
        if (dynamic_cast<Knight*>(p)) pieceLetter = "N";
        else if (dynamic_cast<Bishop*>(p)) pieceLetter = "B";
        else if (dynamic_cast<Rook*>(p)) pieceLetter = "R";
        else if (dynamic_cast<Queen*>(p)) pieceLetter = "Q";
        else if (dynamic_cast<King*>(p)) {
            if (partialMove.isCastling) {
                int dx = toCol - fromCol;
                if (dx > 0) return "O-O";
                else return "O-O-O";
            }
            pieceLetter = "K";
        }

        san += pieceLetter;

        // AMBIGUITY CHECK
        // If not King/Pawn, check conflicts
        if (pieceLetter != "K" && pieceLetter != "") {
             bool fileConflict = false;
             bool rankConflict = false;
             bool ambiguous = false;

             bool dummyCheckmate = false;
             for (Piece* other : player->getPieces()) {
                 if (other == p) continue; // Skip self
                 if (typeid(*other) != typeid(*p)) continue; // Must be same type

                 // Check if 'other' can move to 'to'
                 // This is expensive: generate moves for 'other'
                 // We pass dummy checkmate ptr
                 // Note: Board state is "before the move" (we are calculating SAN before push)
                 // But wait! inside play(), we already did NOT move the piece technically?
                 // calculateSAN called BEFORE move is executed?
                 // No, I inserted calls: 
                 // 1. promotion-start: before push. (Piece at from)
                 // 2. promotion-end: after push? (No, we updated last.san. Piece currently at TO (promoted)? No, standard Pawn)
                 // 3. normal: before push. (Piece at from).
                 
                 // So board is in Pre-Move state.
                 // Correct for ambiguity check.
                 
                 std::set<Box*> moves = player->play(other, &dummyCheckmate);
                 if (moves.count(to)) {
                     ambiguous = true;
                     int otherCol = other->getLocation()->x / (Window::SQUARE_SIZE / 8);
                     int otherRow = other->getLocation()->y / (Window::SQUARE_SIZE / 8);

                     if (otherCol == fromCol) fileConflict = true; // Same file -> need rank
                     // If cols differ, we can use file.
                 }
             }

             if (ambiguous) {
                 if (!fileConflict) {
                     san += (char)('a' + fromCol); // Disambiguate by file
                 } else {
                     // File conflict exists (e.g. both on 'g' file)
                     // Check if ranks differ (they must, or same square?)
                     // Use rank
                     san += (char)('8' - fromRow);
                     // If both file and rank conflict? 3 Queens?
                     // Standard says: File, then Rank if File matches.
                     // If 3 queens: Q at(4,4), Q at(4,6), Q at(6,4). Target (6,6).
                     // Q44 -> 4!=6 file ok. Q64 -> 6==6 file conflict.
                     // So one file conflict exists.
                     // We use File if NO file conflict? No.
                     // Rule:
                     // 1. If file distinguishes, use file.
                     // 2. Else if rank distinguishes, use rank.
                     // 3. Else use both.
                     
                     // My logic: `fileConflict` means "There is at least one other candidate on the same file".
                     // If so, file letter 'a'+fromCol is NOT unique.
                     // So we must use rank?
                     // Wait, if 3 pieces: A(g1), B(g5), C(e1). Target g3.
                     // A and B collide on file. C is distinct.
                     // Moving A: "Ngg3" is ambiguous vs B. "N1g3" is unique.
                     // Moving C: "Neg3". Unique.
                     
                     // So for A: fileConflict=true (due to B).
                     // We use Rank? "N1".
                     // Is Rank unique?
                     // Does B share rank with A? No.
                     // So "N1g3" works.
                     
                     // What if rank conflict too?
                     // A(g1), B(h1). Target f3. (Knights).
                     // Shared rank 1. `rankConflict=true`.
                     // File different. `fileConflict=false`.
                     // Use File: "Ngf3". Unique.
                     
                     // So:
                     // If (!fileConflict) use File.
                     // Else (file is shared), use Rank.
                 }
                 // Edge case: Both shared? (3 queens on same file/rank/diag?)
                 // Q1(e1), Q2(e8), Q3(h1). Target e4.
                 // Q1 moves.
                 // Q2 on e-file -> fileConflict.
                 // Q3 not on e-file.
                 // So use Rank: "Q1e4".
                 // Q2 on e8. Rank 8 != 1. Unique.
                 // Q3 on h1. Rank 1 == 1. Wait.
                 // If Q3 could move to e4? (h1->e4 is diagonal? 3,3 vs 7,0. dy=3, dx=-4. No.)
                 
                 // If Q3(a1) moves to e1.
                 // Q1(e8). Q2(h1).
                 // Use file.
                 
                 // Just use simple priority for now:
                 // if (!fileConflict) san += file;
                 // else san += rank;
             }
        }

        if (partialMove.capturedPiece) san += "x";
    }

    san += colChar;
    san += rowChar;
    
    if (partialMove.isPromotion) {
        san += "=Q"; 
    }
    
    return san;
}
    




