#include <iostream>
#include <SDL.h>
#include "Window.h"

using namespace std;
int Window::SCREEN_WIDTH = 600;
int Window::SCREEN_HEIGHT = 600;
SDL_Renderer* Window::m_renderer=NULL;


int main(int argc, char* args[]) {

	static Window screen;
	if (screen.init() == false) {
		cout << "Could not initialising SDL:  " << SDL_GetError() << "\n" << endl;
	}
	cout << " initialising SDL is okey:  " << "\n" << endl;


	while (true) {

		screen.updateRender();
		//check for messages/events
		if (screen.processEvents() == false) {
			break;
		}
	}
	screen.close();


	return 1;
}