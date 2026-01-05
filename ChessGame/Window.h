#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h> 
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "Board.h"

class Window
{
public:
	SDL_Window* m_window;
	static SDL_Renderer* m_renderer;
	Board* m_gameBoard;

	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static int SQUARE_SIZE;
	static const int SCREEN_MIN_WIDTH = 400;
	static const int SCREEN_MIN_HEIGHT = 400;
    
    // Side Panel
    static const int SIDE_PANEL_WIDTH = 250;
    TTF_Font* m_font;
    SDL_FRect undoButtonRect;
    SDL_FRect redoButtonRect;

public:
	Window();
	bool init();
	bool processEvents();
	void close();
	void updateRender();

	void calculateInitialWindowDimensions();
	void resizeWindow(int width, int height);
    
    // New UI Rendering
    void renderSidePanel();
    void renderButton(const SDL_FRect& rect, const char* text);
    void renderHistory();
};

