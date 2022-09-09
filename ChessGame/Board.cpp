#include "Board.h"
#include "Window.h"


Board::Board()
{
	gameboard = new Box * [rowBoxNmbersandCols];
	for (int i = 0; i < rowBoxNmbersandCols; ++i) {
		gameboard[i] = new Box[rowBoxNmbersandCols];
	}


}

void Board::RenderBoard()
{
	for (int i = 0; i < rowBoxNmbersandCols; i++)
	{
		for (int j = 0; j < rowBoxNmbersandCols; j++)
		{
			gameboard[i][j].RenderBox();
		}
	}
}

void Board::init()
{

	int BoxWidthandHigth = Window::SCREEN_HEIGHT < Window::SCREEN_WIDTH ? Window::SCREEN_HEIGHT : Window::SCREEN_WIDTH; //get the min between them
	BoxWidthandHigth /= rowBoxNmbersandCols;
	for (int i = 0; i < rowBoxNmbersandCols; i++)
	{
		std::vector<Box> oneRow;
		for (int j = 0; j < rowBoxNmbersandCols; j++)
		{
			SDL_Color boxColor = (i + j) % 1 == 0 ? Color1 : Color2;
			gameboard[i][j] = { i * BoxWidthandHigth,j * BoxWidthandHigth,i * BoxWidthandHigth + BoxWidthandHigth,j * BoxWidthandHigth + BoxWidthandHigth ,boxColor };
			//oneRow.push_back({ i * BoxWidthandHigth,j * BoxWidthandHigth,i * BoxWidthandHigth + BoxWidthandHigth,j * BoxWidthandHigth + BoxWidthandHigth ,boxColor });
		}
		//gameboard.push_back(oneRow); ///cloud be worng somthin with the memory leaks
	}

}

Board::~Board() {

	for (int i = 0; i < rowBoxNmbersandCols; ++i) {
		delete[] gameboard[i];
	}
	delete[] gameboard;
}