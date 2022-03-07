#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include "Piece.h"
#include "Board.h"

class Chess {

public:
	// This default constructor initializes a board with the standard
	// piece positions, and sets the state to white's turn
	Chess();

	// Returns a constant reference to the board 
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	const Board& get_board() const { return board; }


	// Returns true if it is white's turn
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	bool turn_white() const { return is_white_turn; }


	// Attemps to make a move. If successful, the move is made and
	// the turn is switched white <-> black
	bool make_move(std::pair<char, char> start, std::pair<char, char> end);

	// Returns true if the designated player is in check
	bool in_check(bool white) const;

	// Returns true if the designated player is in mate
	bool in_mate(bool white) const;

	// Returns true if the designated player is in mate
	bool in_stalemate(bool white) const;

	// ADDED
	// Changes the colour of the player that moves next.
	// Needed when a game is loaded 
	void force_next_color(char color);

	//ADDED
	// function that checks whether there exists one valid move from the current player
	bool valid_move_exists(bool white) const;

	// ADDED 
	// Clears the board of the current chess game
	// Needed when loading a new game from a file
	// as the get_board function returns a constant 
	// and we are not allowed to modify the function
	void clear_board();

	// ADDED
	// Addsa new piece to the board of the current game
	// Needed when loading a new game from a file
	// as the get_board function returns a constant 
	// and we are not allowed to modify the function
	void add_piece_to_board(std::pair<char, char> position ,char piece);

private:
	// The board
	Board board;

	// Is it white's turn?
	bool is_white_turn;

	//ADDED
	// function used to check and change the pawns that arrive 
	// at the other side of the board and need to be changed to queens
	void handle_pawn_change(std::pair<char, char> position, char current);

};

// Writes the board out to a stream
std::ostream& operator<< (std::ostream& os, const Chess& chess);

// Reads the board in from a stream
std::istream& operator>> (std::istream& is, Chess& chess);


#endif // CHESS_H
