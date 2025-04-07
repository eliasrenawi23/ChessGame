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
		handle_promotion(box_x, box_y, &promotion);
		return;
	}
	//if player didnot change the piece location
	if (boxtoLight.empty() || (selectedBox->x / (Window::SQUARE_SIZE / 8) == box_x && selectedBox->y / (Window::SQUARE_SIZE / 8) == box_y)) {
		highlightboxs(false);
		boxtoLight.clear();
		return;
	}

	Box* b = &gameboxess[box_x][box_y]; // the box to play to 
	auto pos = boxtoLight.find(b);
	if (pos != boxtoLight.end()) {
		if ((*pos)->getPiece() != NULL) {
			deletepiece((*pos));
		}
		En_passant(box_x, box_y);
		UpdatePieceLocation(selectedBox, (*pos));
		CastleMove(box_x, box_y);		//check if Castle

		handle_promotion(box_x, box_y, &promotion);

		selectedBox->setPiece(NULL);

		playerTurn = !playerTurn; //change turns	
		checkmate = false;
		if (playerTurn) {
			whitePlayer->setopponentThreatMap(blackPlayer->ClacThreatMap(&checkmate));
		}
		else {
			blackPlayer->setopponentThreatMap(whitePlayer->ClacThreatMap(&checkmate));
		}
		highlightKing();
	}

	highlightboxs(false);
	boxtoLight.clear();

}

void Board::En_passant(int new_x, int new_y) {

	Box* b = selectedBox; // the box to play to /// befor the move 
	if (Pawn* pawn = dynamic_cast<Pawn*>(b->getPiece())) {
		int diraction = (b->getPiece()->getColor() == PlayerColor::WHITE) ? 1 : -1;//if white move -1 id black move 1


		if (pawn->firstMove) {
			if (En_passantPawn != NULL)
				En_passantPawn->PoosblieEnPassant = false;
			pawn->PoosblieEnPassant = true;
			En_passantPawn = pawn;
			return;
		}
		int old_x = selectedBox->x / (Window::SQUARE_SIZE / 8);
		int old_y = selectedBox->y / (Window::SQUARE_SIZE / 8);

		en_passasntDelete(old_x - 1, old_y, new_x, new_y, diraction);
		en_passasntDelete(old_x + 1, old_y, new_x, new_y, diraction);
		En_passantPawn->PoosblieEnPassant = false;

	}

}
void Board::en_passasntDelete(int old_x, int old_y, int new_x, int new_y, int diraction)
{
	if (old_x >= 0 && old_x < rowBoxNmbersandCols) {
		Box* b = &gameboxess[old_x][old_y]; //left box
		if (Pawn* pawn = dynamic_cast<Pawn*>(b->getPiece())) {
			if (pawn->PoosblieEnPassant && (old_x == new_x && new_y == old_y - diraction)) {
				pawn->~Pawn();
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
		(playerTurn ? whitePlayer : blackPlayer)->dopromotion(p, promotion_x, promotion_y);
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
	//p->~Piece();
	delete p;
	b->setPiece(nullptr);
	blackPlayer->updateVectorPieces(p);
	whitePlayer->updateVectorPieces(p);
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



