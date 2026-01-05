#pragma once
#include <SDL3/SDL.h>

class Piece; // Forward declaration

class Box
{
public:
    int x;              // X position of the box
    int y;              // Y position of the box
    int size;           // Size of the box
    SDL_Color boxColor; // Color of the box (used for highlighting)
    SDL_Color originalColor; // Original color of the box

private:
    Piece* piece; // Pointer to the piece located in this box

public:
    Box(); // Default constructor
    Box(int x, int y, int size, SDL_Color originalColor); // Parameterized constructor

    void setPiece(Piece* p); // Set the piece in this box
    Piece* getPiece(); // Get the piece located in this box

    void resize(int x, int y, int size); // Resize and reposition the box
    void RenderBox(); // Render the box on the screen

    ~Box(); // Destructor
};
