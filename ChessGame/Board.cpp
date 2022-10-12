#include "Board.h"
#include "Window.h"

int Board::BoxWidthandHigth;
std::vector<std::vector<Box >> Board::gameboxess;
Board::Board() : whitePlayer(NULL), blackPlayer(NULL), playerTurn(true), selectedBox(NULL), checkmate(false)
{
}
void Board::init()
{
	BoxWidthandHigth = Window::SCREEN_HEIGHT < Window::SCREEN_WIDTH ? Window::SCREEN_HEIGHT : Window::SCREEN_WIDTH; //get the min between them
	BoxWidthandHigth /= rowBoxNmbersandCols;

	for (int i = 0; i < rowBoxNmbersandCols; i++)
	{
		std::vector<Box> temp;
		for (int j = 0; j < rowBoxNmbersandCols; j++)
		{
			temp.push_back({ i * BoxWidthandHigth,j * BoxWidthandHigth,BoxWidthandHigth ,(i + j) % 2 == 0 ? Color1 : Color2 });

		}
		gameboxess.push_back(temp);

	}



	whitePlayer = new Player(PlayerColor::WHITE);
	whitePlayer->init();
	blackPlayer = new Player(PlayerColor::BLACK);
	blackPlayer->init();


}

void Board::resize()
{
	BoxWidthandHigth = Window::SCREEN_HEIGHT < Window::SCREEN_WIDTH ? Window::SCREEN_HEIGHT : Window::SCREEN_WIDTH; //get the min between them
	BoxWidthandHigth /= rowBoxNmbersandCols;

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
	std::cout << " box_x box_y MOUSE BUTTON Down : " << box_x << " " << box_y << std::endl;
	selectedBox = &gameboxess[box_x][box_y]; //selected box that contain piece to play with
	if (selectedBox->getPiece() == NULL) { //this vox has no pice in it no action needed
		std::cout << " this vox has no pice in it no action needed " << std::endl;
		return;
	}
	if (playerTurn && (selectedBox->getPiece()->getColor() == PlayerColor::WHITE)) {
		//to do get the player move
		whitePlayer->setopponentThreatMap(blackPlayer->ClacThreatMap(&checkmate));
		//check if king is in check mate
		boxtoLight = whitePlayer->play(selectedBox->getPiece(), &checkmate);
		highlightKing(checkmate, whitePlayer);
	
	}
	else if (!playerTurn && (selectedBox->getPiece()->getColor() == PlayerColor::BLACK)) {
		//to do get the player move
		blackPlayer->setopponentThreatMap(whitePlayer->ClacThreatMap(&checkmate));
		boxtoLight = blackPlayer->play(selectedBox->getPiece(), &checkmate);
		highlightKing(checkmate, blackPlayer);

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
	//if player didnot change the piece location
	if (boxtoLight.empty() || (selectedBox->x / (Window::SQUARE_SIZE / 8) == box_x && selectedBox->y / (Window::SQUARE_SIZE / 8) == box_y)) {
		highlightboxs(false);
		boxtoLight.clear();
		return;
	}
	std::set<Box*>::iterator itr;
	Box* b = &gameboxess[box_x][box_y]; // the box to play to 
	auto pos = boxtoLight.find(b);
	if (pos != boxtoLight.end()) {
		if ((*pos)->getPiece() != NULL) {
			(*pos)->getPiece()->~Piece();
			delete (*pos)->getPiece();
			whitePlayer->updateVectorPieces((*pos)->getPiece());
			blackPlayer->updateVectorPieces((*pos)->getPiece());
		}
		selectedBox->getPiece()->setLocation((*pos));   ///chenge the location of the piece
		(*pos)->setPiece(selectedBox->getPiece());
		selectedBox->setPiece(NULL);
		playerTurn = !playerTurn; //change turns		
	}
	checkmate = false;
	highlightboxs(false);
	boxtoLight.clear();

}


void Board::highlightboxs(bool onOrOff) {

	std::set<Box*>::iterator itr;
	if (onOrOff) {
		for (itr = boxtoLight.begin(); itr != boxtoLight.end(); itr++) {
			(*itr)->boxColor = { 64,191, 255, SDL_ALPHA_OPAQUE };
		}
	}
	else {
		for (itr = boxtoLight.begin(); itr != boxtoLight.end(); itr++) {
			(*itr)->boxColor = (*itr)->originalColor;
		}
	}

}
void Board::RenderBoard()
{
	for (int i = 0; i < rowBoxNmbersandCols; i++)
	{
		for (int j = 0; j < rowBoxNmbersandCols; j++)
		{
			gameboxess[i][j].RenderBox();
		}
	}
	RenderPieces();
}
void Board::RenderPieces()
{
	std::vector<Piece*> BlackPieces = blackPlayer->getPieces();
	for (int i = 0; i < BlackPieces.size(); i++)
	{
		BlackPieces[i]->renderPiece();

	}
	std::vector<Piece*> WhitePieces = whitePlayer->getPieces();
	for (int i = 0; i < WhitePieces.size(); i++)
	{
		if (WhitePieces[i] != nullptr) {

			WhitePieces[i]->renderPiece();
		}
	}
}


void Board::checkresult()
{
	bool stlemnt = false;
	bool gameover = false;
	PlayerColor winner = PlayerColor::BLACK;
	if (boxtoLight.size() == 1) {
		if ((*boxtoLight.begin()) == selectedBox) {
			//the game is stelment 
			stlemnt = true;
			std::cout << " this game is statement  no one wins  " << std::endl;
		}
	}
	else if (boxtoLight.empty() && checkmate) {
		if (playerTurn) {
			//black win 
			winner = PlayerColor::BLACK;
			std::cout << " BLACK is winner " << std::endl;


		}
		else {
			//white win
			winner = PlayerColor::WHITE;
			std::cout << " WHITE is winner " << std::endl;
		}
		gameover = true;
	}


}
void Board::highlightKing(bool checkmate, Player* player)
{
	King* k = player->getKing();
	Box* b = k->getLocation();;
	checkmate ? b->boxColor = { 255,0,0,0 } : b->boxColor = b->originalColor;

}
Board::~Board() {

	delete whitePlayer;
	delete blackPlayer;
}



