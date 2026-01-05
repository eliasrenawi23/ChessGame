#include "Window.h"

int Window::SCREEN_WIDTH = 300;
int Window::SCREEN_HEIGHT = 300;
int Window::SQUARE_SIZE = 300;
SDL_Renderer* Window::m_renderer = NULL;

Window::Window() :
	m_window(nullptr), m_texture(nullptr), m_gameBoard(nullptr) {

}


bool Window::init() {

	SDL_Log("SDL_Init started");

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return false;
	}

	calculateInitialWindowDimensions();

	//m_window = SDL_CreateWindow("Chess",SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOWPOS_UNDEFINED);
	m_window = SDL_CreateWindow("Chess", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!m_window) {
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
		SDL_Quit();
		return false;
	}
	//SDL_SetWindowResizable(m_window, true);
	SDL_SetWindowMinimumSize(m_window, SCREEN_MIN_WIDTH, SCREEN_MIN_HEIGHT);

	m_renderer = SDL_CreateRenderer(m_window, nullptr);
	if (!m_renderer) {
		SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}


	m_gameBoard = new Board();
	if (!m_gameBoard) {
		SDL_Log("Board allocation failed.");
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	m_gameBoard->init();
	return  true;
}
bool Window::processEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {  //SDL_PollEvent(&event)
		switch (event.type) {
		case SDL_EVENT_QUIT:
			return  false;
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:  // for resizeing of the window
				resizeWindow(event.window.data1, event.window.data2);

			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN://mouse pressed
			m_gameBoard->getLegalMovs(event.motion.x, event.motion.y);

			break;
		case SDL_EVENT_MOUSE_BUTTON_UP: //mouse relesde
			m_gameBoard->play(event.motion.x, event.motion.y);
			break;
		default :
			break;


		}
		//SDL_RenderClear(m_renderer);



	}
	return  true;
}
void Window::close() {
	delete m_gameBoard;
	m_gameBoard = nullptr;

	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;

	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	SDL_Quit();
}

void Window::updateRender() {

	SDL_RenderClear(m_renderer);
	m_gameBoard->RenderBoard();
	SDL_RenderPresent(m_renderer);
}


//sets the initial height and width to be a square that is 80% of the smallest dimension.
void Window::calculateInitialWindowDimensions() {
	SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
	const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(displayID);
	if (!mode) {
		SDL_Log("SDL_GetCurrentDisplayMode failed: %s", SDL_GetError());
		return;
	}

	int Width = mode->w;
	int Height = mode->h;
	int square = (Width > Height) ? static_cast<int>(0.8f * Height) : static_cast<int>(0.8f * Width);

	SCREEN_WIDTH = SCREEN_HEIGHT = square;
	SQUARE_SIZE = square;

	SDL_Log("Calculated window size: %d x %d", SCREEN_WIDTH, SCREEN_HEIGHT);
}


void Window::resizeWindow(int newWidth, int newHeight) {
	SCREEN_WIDTH = newWidth;
	SCREEN_HEIGHT = newHeight;
	SQUARE_SIZE = (newWidth > newHeight) ? newHeight : newWidth;
	m_gameBoard->resize();
}

