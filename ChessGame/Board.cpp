#include "Board.h"
#include "Window.h"

int Board::BoxWidthandHigth;
std::vector<std::vector<Box >> Board::gameboxess;
Board::Board() : whitePlayer(NULL), blackPlayer(NULL)
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
	std::vector<Piece*> WhitePieces = whitePlayer->getPieces();
	for (int  i = 0; i < WhitePieces.size(); i++)
	{
		WhitePieces[i]->render();
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




