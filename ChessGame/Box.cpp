#include "Box.h"
#include "Window.h"
#include "Piece.h"

Box::Box()
    : x(0), y(0), size(0), piece(nullptr), boxColor({ 0, 0, 0, 255 }), originalColor({ 0, 0, 0, 255 }) {
}

Box::Box(int x, int y, int size, SDL_Color originalColor)
    : x(x), y(y), size(size), originalColor(originalColor), piece(nullptr), boxColor(originalColor) {
}

void Box::setPiece(Piece* p) {
    piece = p; // Assign the piece to the box
}

Piece* Box::getPiece() {
    return piece; // Return the piece in the box
}

void Box::resize(int x, int y, int size) {
    this->x = x; // Update x position
    this->y = y; // Update y position
    this->size = size; // Update size
}

void Box::RenderBox() {
    SDL_FRect highlightRect;
    highlightRect.w = size; // Set width
    highlightRect.h = size; // Set height
    highlightRect.x = x; // Set x position
    highlightRect.y = y; // Set y position

    // Set render color for the box
    SDL_SetRenderDrawColor(Window::m_renderer, boxColor.r, boxColor.g, boxColor.b, boxColor.a);

    // Render the box on the screen
    SDL_RenderFillRect(Window::m_renderer, &highlightRect);
}

Box::~Box() {
    // No dynamic memory to clean up in this class, so the destructor is empty
}
