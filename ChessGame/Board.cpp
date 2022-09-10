#include "Board.h"
#include "Window.h"

int Board::BoxWidthandHigth;
Board::Board()
{
	gameboxess = new Box * [rowBoxNmbersandCols];
	for (int i = 0; i < rowBoxNmbersandCols; ++i) {
		gameboxess[i] = new Box[rowBoxNmbersandCols];
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
}

void Board::init()
{

	BoxWidthandHigth = Window::SCREEN_HEIGHT < Window::SCREEN_WIDTH ? Window::SCREEN_HEIGHT : Window::SCREEN_WIDTH; //get the min between them
	BoxWidthandHigth /= rowBoxNmbersandCols;
	for (int i = 0; i < rowBoxNmbersandCols; i++)
	{
		for (int j = 0; j < rowBoxNmbersandCols; j++)
		{
			SDL_Color boxColor = (i + j) % 2 == 0 ? Color1 : Color2;
			gameboxess[i][j] = { i * BoxWidthandHigth,j * BoxWidthandHigth,i * BoxWidthandHigth + BoxWidthandHigth,j * BoxWidthandHigth + BoxWidthandHigth ,boxColor };
		}
	}

}

void Board::resize()
{
	BoxWidthandHigth = Window::SCREEN_HEIGHT < Window::SCREEN_WIDTH ? Window::SCREEN_HEIGHT : Window::SCREEN_WIDTH; //get the min between them
	BoxWidthandHigth /= rowBoxNmbersandCols;

}

Board::~Board() {

	for (int i = 0; i < rowBoxNmbersandCols; ++i) {
		delete[] gameboxess[i];
	}
	delete[] gameboxess;
}