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
void Board::init()
{
	BoxWidthandHigth = Window::SCREEN_HEIGHT < Window::SCREEN_WIDTH ? Window::SCREEN_HEIGHT : Window::SCREEN_WIDTH; //get the min between them
	BoxWidthandHigth /= rowBoxNmbersandCols;
	for (int i = 0; i < rowBoxNmbersandCols; i++)
	{
		for (int j = 0; j < rowBoxNmbersandCols; j++)
		{
			SDL_Color boxColor = (i + j) % 2 == 0 ? Color1 : Color2;
			gameboxess[i][j] = { i * BoxWidthandHigth,j * BoxWidthandHigth,BoxWidthandHigth ,boxColor };
		}
	}

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

	for (int i = 0; i < rowBoxNmbersandCols; ++i) {
		delete[] gameboxess[i];
		std::cout << "gameboxess[" << i << "] is deleted" << std::endl;
	}
	delete[] gameboxess;
	std::cout << "gameboxess is deleted" << std::endl;

}


