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
void Board::play(int cor_x, int cor_y)
{
	std::cout << " cor_x cor_y MOUSE BUTTON UP or down : " << cor_x << " " << cor_y << std::endl;

	if (gameboxess[cor_x][cor_y].getPiece() == NULL) { //this vox has no pice in it no action needed
		std::cout << " this vox has no pice in it no action needed " << std::endl;
		return;
	}
	if (playerTurn && (gameboxess[cor_x][cor_y].getPiece()->color == PlayerColor::WHITE)) {
		//to do get the player move
		boxtoLight = whitePlayer->play(gameboxess[cor_x][cor_y].getPiece());
		highlightboxs(true);
		playerTurn = !playerTurn; //change turns
	}
	else if (!playerTurn && (gameboxess[cor_x][cor_y].getPiece()->color == PlayerColor::BLACK)) {
		//to do get the player move
		boxtoLight = blackPlayer->play(gameboxess[cor_x][cor_y].getPiece());
		highlightboxs(true);
		playerTurn = !playerTurn; //change turns
	}


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




