#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece {

public:

	bool legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const;

	bool legal_capture_shape(std::pair<char, char> start, std::pair<char, char> end) const
	{
		return legal_move_shape(start, end);
	}

	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	char to_ascii() const {
		return is_white() ? 'R' : 'r';
	}

private:
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Rook(bool is_white) : Piece(is_white) {}

	friend Piece* create_piece(char piece_designator);
};

#endif // ROOK_H
