#include "Window.h"

int Window::SCREEN_WIDTH = 600;
int Window::SCREEN_HEIGHT = 600;
SDL_Renderer* Window::m_renderer = NULL;

	Window::Window() :
		m_window(NULL), m_texture(NULL), m_gamBoard(NULL) {

	}


	bool Window::init() {

		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			SDL_Quit();
			return false;
		}
		m_window = SDL_CreateWindow("Chess",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_SHOWN);

		if (m_window == NULL) {
			SDL_Quit();
			return false;
		}
		calculateInitialWindowDimensions();
		SDL_SetWindowResizable(m_window, SDL_TRUE);
		SDL_SetWindowMinimumSize(m_window, SCREEN_MIN_WIDTH, SCREEN_MIN_HEIGHT);

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);

		if (m_renderer == NULL) {
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}
		//SDL_Color drawColor = { 255, 255, 255, SDL_ALPHA_OPAQUE };//white
		//SDL_SetRenderDrawColor(m_renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
		m_gamBoard = new Board();

		if (m_gamBoard == NULL) {
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}
		m_gamBoard->init();

		return  true;
	}
	bool Window::processEvents() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				return  false;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					resizeWindow(event.window.data1, event.window.data2);
				}
				break;
	

			}




		}
		return  true;

	}
	void Window::close() {

		delete (m_gamBoard);
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	void Window::updateRender() {

		//SDL_Color drawColor = { 255, 255, 255, SDL_ALPHA_OPAQUE };//blue
		//SDL_SetRenderDrawColor(m_renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);

		SDL_RenderClear(m_renderer);
		m_gamBoard->RenderBoard();
		SDL_RenderPresent(m_renderer);
	}


	//sets the initial height and width to be a square that is 80% of the smallest dimension.
	void Window::calculateInitialWindowDimensions() {
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		auto Width = DM.w;
		auto Height = DM.h;


		//If height is smaller.
		int squareWidth;
		if (Width > Height) {
			squareWidth = .8 * Height;
		}
		else {
			squareWidth = .8 * Width;
		}
		Window::SCREEN_WIDTH = Window::SCREEN_HEIGHT = squareWidth;
	}
	void Window::resizeWindow(int newWidth, int newHeight) {
		Window::SCREEN_HEIGHT = newHeight;
		Window::SCREEN_WIDTH = newWidth;
		m_gamBoard->resize();
	}

