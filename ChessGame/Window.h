#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h> 
#include "Board.h"

	class Window {
	public:
		  static int SCREEN_WIDTH; //width and hight are the same size;
		  static int SCREEN_HEIGHT;
		  static int SQUARE_SIZE;
		  static SDL_Renderer* m_renderer;

		const static int SCREEN_MIN_HEIGHT = 400;
		const static int SCREEN_MIN_WIDTH = 400;
	private:
		SDL_Window* m_window;
		SDL_Texture* m_texture;
		Board *m_gamBoard;

	public:
		Window();
		bool init();
		bool processEvents();
		void close();
		void updateRender();
		void calculateInitialWindowDimensions();
		void resizeWindow(int newWidth, int newHeight);

	};



