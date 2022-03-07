#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : public Piece {

public:

	bool legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const;

	bool legal_capture_shape(std::pair<char, char> start, std::pair<char, char> end) const
	{
		return this->legal_move_shape(start, end); // the implementation of the capture is the same as the one for a normal move
	}


	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	char to_ascii() const {
		return is_white() ? 'B' : 'b';
	}

private:
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Bishop(bool is_white) : Piece(is_white) {}

	friend Piece* create_piece(char piece_designator);
};

#endif // BISHOP_H
