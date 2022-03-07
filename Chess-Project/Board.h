#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Mystery.h"


class Board {

        // Throughout, we will be accessing board positions using an std::pair<char, char>.
	// The assumption is that the first value is the column with values in
        // {'A','B','C','D','E','F','G','H'} (all caps)
	// and the second is the row, with values in {'1','2','3','4','5','6','7','8'}

public:
	// Default constructor
	Board();

	// Returns a const pointer to the piece at a prescribed location if it exists,
        // or nullptr if there is nothing there.
	const Piece* operator() (std::pair<char, char> position) const;

	const Piece* get_at(std::pair<char, char> position) const
	{
		Board b = *this;
		return b(position);
	}

	// Attempts to add a new piece with the specified designator, at the given location.
	// Returns false if:
	// -- the designator is invalid,
	// -- the specified location is not on the board, or
	// -- if the specified location is occupied
	bool add_piece(std::pair<char, char> position, char piece_designator);

	// ADDED
	// removes the piece at a certain location
	bool remove_piece(std::pair<char, char> position);

	// Displays the board by printing it to stdout
	void display() const;

	// Returns true if the board has the right number of kings on it
	bool has_valid_kings() const;

	// ADDED
	// clears all the pieces from the board
	// used when loading a game
	void clear_board();

private:
	// The sparse map storing the pieces, keyed off locations
	std::map<std::pair<char, char>, Piece*> occ;
};

// Write the board state to an output stream
std::ostream& operator<< (std::ostream& os, const Board& board);

#endif // BOARD_H
