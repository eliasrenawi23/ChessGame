#pragma once
#include <SDL.h>
#include <iostream>

namespace WindowP {
	class Window {
	public:
		 int SCREEN_WIDTH;
		 int SCREEN_HEIGHT;
		
		const static int SCREEN_MIN_HEIGHT = 400;
		const static int SCREEN_MIN_WIDTH = 400;
	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_texture;

	public:
		Window();
		bool init();
		bool processEvents();
		void close();
		void update();
		void calculateInitialWindowDimensions();
		void resizeWindow(int newWidth, int newHeight);

	};



}

