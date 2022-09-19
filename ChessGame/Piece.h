#pragma once
class Piece
{

protected:
	int x_coordinate;
	int y_coordinate;

	//box *location;
public:

	Piece();
	virtual void moveAndTake() = 0;
	~Piece();

};

