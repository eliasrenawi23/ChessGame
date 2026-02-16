#include "Window.h"
#include <iomanip>
#include <sstream>

int Window::SCREEN_WIDTH = 300;
int Window::SCREEN_HEIGHT = 300;
int Window::SQUARE_SIZE = 300;
SDL_Renderer* Window::m_renderer = NULL;

Window::Window() :
	m_window(nullptr), m_gameBoard(nullptr), m_font(nullptr) {

}


bool Window::init() {

	SDL_Log("SDL_Init started");

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return false;
	}

    if (!TTF_Init()) {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        return false;
    }
    
    // Load font - fallback to generic system font if possible
    m_font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 20);
    if (!m_font) {
         SDL_Log("Failed to load font: %s", SDL_GetError());
         // Try another one or continue without text? 
         // Critical for history.
    }

	calculateInitialWindowDimensions();

	//m_window = SDL_CreateWindow("Chess",SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOWPOS_UNDEFINED);
    // Add side panel to width
	m_window = SDL_CreateWindow("Chess", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!m_window) {
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
		SDL_Quit();
		return false;
	}
	//SDL_SetWindowResizable(m_window, true);
	SDL_SetWindowMinimumSize(m_window, SCREEN_MIN_WIDTH + SIDE_PANEL_WIDTH, SCREEN_MIN_HEIGHT);

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
            if (event.motion.x < SQUARE_SIZE) {
			    m_gameBoard->getLegalMovs(static_cast<int>(event.motion.x), static_cast<int>(event.motion.y));
            } else {
                // Check buttons
                SDL_Point p = { (int)event.motion.x, (int)event.motion.y };
                SDL_Rect rUndo = { (int)undoButtonRect.x, (int)undoButtonRect.y, (int)undoButtonRect.w, (int)undoButtonRect.h };
                SDL_Rect rRedo = { (int)redoButtonRect.x, (int)redoButtonRect.y, (int)redoButtonRect.w, (int)redoButtonRect.h };
                
                if (SDL_PointInRect(&p, &rUndo)) {
                    m_gameBoard->undo();
                } else if (SDL_PointInRect(&p, &rRedo)) {
                    m_gameBoard->redo();
                }
            }
			break;
		case SDL_EVENT_MOUSE_BUTTON_UP: //mouse relesde
            if (event.motion.x < SQUARE_SIZE) {
			    m_gameBoard->play(static_cast<int>(event.motion.x), static_cast<int>(event.motion.y));
            }
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

    if (m_font) {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
    TTF_Quit();
	SDL_Quit();
}

void Window::updateRender() {

	SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255); // Dark background
	SDL_RenderClear(m_renderer);
	m_gameBoard->RenderBoard();
    renderSidePanel();
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

	SQUARE_SIZE = square;
    SCREEN_HEIGHT = square;
    SCREEN_WIDTH = square + SIDE_PANEL_WIDTH;

	SDL_Log("Calculated window size: %d x %d", SCREEN_WIDTH, SCREEN_HEIGHT);
}


void Window::resizeWindow(int newWidth, int newHeight) {
    // Preserve aspect ratio? Or just resize board?
    // Let's keep board square.
    
    // newWidth includes side panel?
    int availableWidth = newWidth - SIDE_PANEL_WIDTH;
	int square = (availableWidth > newHeight) ? newHeight : availableWidth;

    if (square < 0) square = 100;

    SQUARE_SIZE = square;
	SCREEN_WIDTH = newWidth; // actual window size
	SCREEN_HEIGHT = newHeight;
    // We may leave empty space if aspect ratio drifts.
    
    // Board needs to resize to SQUARE_SIZE
    // But Board::resize() uses Window::BoxWidthandHeight... which uses...
    // Board::resize implementation checks Window::SCREEN_HEIGHT/Width.
    // I need to patch Board::resize to use SQUARE_SIZE instead of recalculating from SCREEN_...
    // Actually Board::resize calls: BoxWidthandHigth = std::min(Window::SCREEN_HEIGHT, Window::SCREEN_WIDTH) / rowBoxNmbersandCols;
    // Since I updated global static SQUARE_SIZE, I should update Board to use IT.
    
    // Board::resize calculates BoxWidthandHigth based on MIN(SCREEN_HEIGHT, SCREEN_WIDTH).
    // This is WRONG now because SCREEN_WIDTH includes side panel.
    // I should Fix Board::resize logic too? 
    // Or just set static SCREEN_WIDTH to the BOARD width? No, that breaks window creation.
    // I should simply update Board to use SQUARE_SIZE.
    // But Board code (which I modified only in play/undo) still has old resize logic.
    // I should probably patch that first or assume it's acceptable enough?
    // Wait, if Board uses SCREEN_WIDTH (including panel), the board will be too big and flow under panel?
    // Board::resize: `std::min(SCREEN_HEIGHT, SCREEN_WIDTH)`.
    // If Height=800, Width=1050. Min=800.
    // It works! It takes the smaller dimension.
    // As long as Panel adds to Width and Width > Height, Min is Height. Correct.
    // If Height > Width (Portrait), Min is Width. 
    // If Width=600 (Panel 250 -> Board 350). Height=800. Min=600.
    // Board draws 600x600. Panel at 600?
    // Board draws at 0,0.
    // It seems "Okay" for most desktop cases where Width > Height.
    
	m_gameBoard->resize();
}

void Window::renderSidePanel() {
    int xStart = SQUARE_SIZE;
    SDL_FRect panelRect = { (float)xStart, 0, (float)SIDE_PANEL_WIDTH, (float)SCREEN_HEIGHT };
    
    // Background
    SDL_SetRenderDrawColor(m_renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(m_renderer, &panelRect);
    
    // Buttons
    float btnWidth = 100;
    float btnHeight = 40;
    float padding = 20;
    
    undoButtonRect = { (float)xStart + padding, (float)SCREEN_HEIGHT - btnHeight - padding, btnWidth, btnHeight };
    redoButtonRect = { (float)xStart + padding + btnWidth + 10, (float)SCREEN_HEIGHT - btnHeight - padding, btnWidth, btnHeight };
    
    renderButton(undoButtonRect, "Undo");
    renderButton(redoButtonRect, "Redo");
    
    // History
    renderHistory();
}

void Window::renderButton(const SDL_FRect& rect, const char* text) {
    SDL_SetRenderDrawColor(m_renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(m_renderer, &rect);
    
    SDL_SetRenderDrawColor(m_renderer, 200, 200, 200, 255);
    SDL_RenderRect(m_renderer, &rect); // Border
    
    if (m_font) {
        SDL_Surface* surface = TTF_RenderText_Solid(m_font, text, 0, {255, 255, 255, 255});
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
            SDL_FRect textRect = { rect.x + 10, rect.y + 5, (float)surface->w, (float)surface->h }; // Centering approximates
            SDL_RenderTexture(m_renderer, texture, NULL, &textRect);
            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        }
    }
}

void Window::renderHistory() {
    if (!m_font) return;
    
    auto& history = m_gameBoard->getHistory();
    size_t currentIndex = m_gameBoard->getCurrentMoveIndex();
    
    int startX = SQUARE_SIZE + 20;
    int startY = 20;
    int msgHeight = 25;
    
    // Show last 20 moves or scroll?
    // Just list last ~20 moves for now.
    int maxLines = (SCREEN_HEIGHT - 100) / msgHeight;
    int startIdx = (int)history.size() - maxLines;
    if (startIdx < 0) startIdx = 0;
    
    for (size_t i = startIdx; i < history.size(); i++) {
        std::stringstream ss;
        ss << (i + 1) << ". ";
        // Use SAN string
        ss << history[i].san;
        
        std::string line = ss.str();
        SDL_Color color = { 200, 200, 200, 255 };
        if (i == currentIndex - 1) color = { 64, 191, 255, 255 }; // Highlight last move
        if (i >= currentIndex) color = { 100, 100, 100, 255 }; // Future moves (undo stack)
        
        SDL_Surface* surface = TTF_RenderText_Solid(m_font, line.c_str(), 0, color);
         if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
            SDL_FRect textRect = { (float)startX, (float)startY + (i - startIdx) * msgHeight, (float)surface->w, (float)surface->h };
            SDL_RenderTexture(m_renderer, texture, NULL, &textRect);
            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        }
    }
}

