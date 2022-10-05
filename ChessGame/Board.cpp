#include "Board.h"
#include "Window.h"

int Board::BoxWidthandHigth;
std::vector<std::vector<Box >> Board::gameboxess;
Board::Board() : whitePlayer(NULL), blackPlayer(NULL), playerTurn(true)
{
}
void Board::init()
{
	BoxWidthandHigth = Window::SCREEN_HEIGHT < Window::SCREEN_WIDTH ? Window::SCREEN_HEIGHT : Window::SCREEN_WIDTH; //get the min between them
	BoxWidthandHigth /= rowBoxNmbersandCols;

	std::cout << BoxWidthandHigth << " " << BoxWidthandHigth << std::endl;

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

	if (gameboxess[box_x][box_y].getPiece() == NULL) { //this vox has no pice in it no action needed
		std::cout << " this vox has no pice in it no action needed " << std::endl;
		return;
	}
	if (playerTurn && (gameboxess[box_x][box_y].getPiece()->color == PlayerColor::WHITE)) {
		//to do get the player move
		boxtoLight = whitePlayer->play(gameboxess[box_x][box_y].getPiece());
		highlightboxs(true);
	}
	else if (!playerTurn && (gameboxess[box_x][box_y].getPiece()->color == PlayerColor::BLACK)) {
		//to do get the player move
		boxtoLight = blackPlayer->play(gameboxess[box_x][box_y].getPiece());
		highlightboxs(true);
	}


}
void  Board::play(int cor_x, int cor_y) {
	int box_x = cor_x / (Window::SQUARE_SIZE / 8);
	int box_y = cor_y / (Window::SQUARE_SIZE / 8);
	if (cor_x > Window::SQUARE_SIZE || cor_y > Window::SQUARE_SIZE) {
		std::cout << " out of range " << box_x << " " << box_y << std::endl;
		return;
	}
	if (boxtoLight.empty() || boxtoLight[0]->x / (Window::SQUARE_SIZE / 8) == box_x && boxtoLight[0]->y / (Window::SQUARE_SIZE / 8) == box_y) {
		highlightboxs(false);
		boxtoLight.clear();
		return;
	}
	for (int i = 1; i < boxtoLight.size(); i++) {

		if (boxtoLight[i]->x / (Window::SQUARE_SIZE / 8) == box_x && boxtoLight[i]->y / (Window::SQUARE_SIZE / 8) == box_y) {
			if (boxtoLight[i]->getPiece() != NULL) {
				boxtoLight[i]->getPiece()->~Piece();
			}
			boxtoLight[0]->getPiece()->setLocation(boxtoLight[i]);
			boxtoLight[i]->setPiece(boxtoLight[0]->getPiece());
			boxtoLight[0]->setPiece(NULL);
			playerTurn = !playerTurn; //change turns
			break;
		}
	}
	highlightboxs(false);
	boxtoLight.clear();

}


void Board::highlightboxs(bool onOrOff) {

	if (onOrOff) {
		for (int i = 0; i < boxtoLight.size(); i++) {

			boxtoLight[i]->boxColor = { 64,191, 255, SDL_ALPHA_OPAQUE };

		}
	}
	else {
		for (int i = 0; i < boxtoLight.size(); i++) {
			boxtoLight[i]->boxColor = boxtoLight[i]->originalColor;
		}
	}

}
void Board::RenderBoard()
{
	for (int i = 0; i < rowBoxNmbersandCols; i++)
	{
		for (int j = 0; j < rowBoxNmbersandCols; j++)
		{
			RenderBox(&gameboxess[i][j]);
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
		WhitePieces[i]->renderPiece();
	}


}

void Board::RenderBox(Box* boxtorender)
{
	SDL_Rect highlightRect;
	highlightRect.w = boxtorender->size;
	highlightRect.h = boxtorender->size;
	SDL_SetRenderDrawColor(Window::m_renderer, boxtorender->boxColor.r, boxtorender->boxColor.g, boxtorender->boxColor.b, boxtorender->boxColor.a);
	highlightRect.x = boxtorender->x;
	highlightRect.y = boxtorender->y;
	SDL_RenderFillRect(Window::m_renderer, &highlightRect);
}

Board::~Board() {

	delete whitePlayer;
	delete blackPlayer;
}




