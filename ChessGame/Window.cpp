#include "Window.h"

int Window::SCREEN_WIDTH = 300;
int Window::SCREEN_HEIGHT = 300;
int Window::SQUARE_SIZE = 300;
SDL_Renderer* Window::m_renderer = NULL;

Window::Window() :
	m_window(NULL), m_texture(NULL), m_gamBoard(NULL) {

}


bool Window::init() {

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		SDL_Quit();
		std::cout << "SDL_Init" << std::endl;

		return false;
	}
	//m_window = SDL_CreateWindow("Chess",SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOWPOS_UNDEFINED);
	m_window = SDL_CreateWindow("Chess",
		 SCREEN_WIDTH, SCREEN_HEIGHT, NULL);

	if (m_window == NULL) {
		SDL_Quit();
		std::cout << "m_window" << std::endl;
		return false;
	}
	calculateInitialWindowDimensions();
	SDL_SetWindowResizable(m_window, true);
	SDL_SetWindowMinimumSize(m_window, SCREEN_MIN_WIDTH, SCREEN_MIN_HEIGHT);

	m_renderer = SDL_CreateRenderer(m_window, NULL);

	if (m_renderer == NULL) {
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		std::cout << "m_renderer" << std::endl;

		return false;
	}

	m_gamBoard = new Board();
	if (m_gamBoard == NULL) {
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		std::cout << "m_renderer" << std::endl;

		return false;
	}
	m_gamBoard->init();
	return  true;
}
bool Window::processEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {  //SDL_PollEvent(&event)
		switch (event.type) {
		case SDL_EVENT_QUIT:
			return  false;
			break;
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:  // for resizeing of the window
				resizeWindow(event.window.data1, event.window.data2);

			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN://mouse pressed
			m_gamBoard->getLegalMovs(event.motion.x, event.motion.y);

			break;
		case SDL_EVENT_MOUSE_BUTTON_UP: //mouse relesde
			m_gamBoard->play(event.motion.x, event.motion.y);
			break;



		}
		//SDL_RenderClear(m_renderer);



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
	SDL_RenderClear(m_renderer);
	m_gamBoard->RenderBoard();
	SDL_RenderPresent(m_renderer);
}


//sets the initial height and width to be a square that is 80% of the smallest dimension.
void Window::calculateInitialWindowDimensions() {
	SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(displayID);
	if (mode == nullptr) {
		std::cout << "SDL_GetCurrentDisplayMode failed: " << SDL_GetError() << std::endl;
		return;
	}

	int Width = mode->w;
	int Height = mode->h;
	int squareWidth = (Width > Height) ? static_cast<int>(0.8f * Height) : static_cast<int>(0.8f * Width);

	Window::SCREEN_WIDTH = Window::SCREEN_HEIGHT = squareWidth;
	Window::SQUARE_SIZE = squareWidth;

	std::cout << SCREEN_WIDTH << " " << SCREEN_HEIGHT << std::endl;
}


void Window::resizeWindow(int newWidth, int newHeight) {
	Window::SCREEN_HEIGHT = newHeight;
	Window::SCREEN_WIDTH = newWidth;
	if (newWidth > newHeight) {
		Window::SQUARE_SIZE =  newHeight;
	}
	else {
		Window::SQUARE_SIZE = newWidth;
	}
	m_gamBoard->resize();
}

