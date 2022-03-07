#include "Chess.h"
#include "Utils.h"
/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
Chess::Chess() : is_white_turn(true) {
	// Add the pawns
	for (int i = 0; i < 8; i++) {
		board.add_piece(std::pair<char, char>('A' + i, '1' + 1), 'P');
		board.add_piece(std::pair<char, char>('A' + i, '1' + 6), 'p');
	}

	// Add the rooks
	board.add_piece(std::pair<char, char>('A' + 0, '1' + 0), 'R');
	board.add_piece(std::pair<char, char>('A' + 7, '1' + 0), 'R');
	board.add_piece(std::pair<char, char>('A' + 0, '1' + 7), 'r');
	board.add_piece(std::pair<char, char>('A' + 7, '1' + 7), 'r');

	// Add the knights
	board.add_piece(std::pair<char, char>('A' + 1, '1' + 0), 'N');
	board.add_piece(std::pair<char, char>('A' + 6, '1' + 0), 'N');
	board.add_piece(std::pair<char, char>('A' + 1, '1' + 7), 'n');
	board.add_piece(std::pair<char, char>('A' + 6, '1' + 7), 'n');

	// Add the bishops
	board.add_piece(std::pair<char, char>('A' + 2, '1' + 0), 'B');
	board.add_piece(std::pair<char, char>('A' + 5, '1' + 0), 'B');
	board.add_piece(std::pair<char, char>('A' + 2, '1' + 7), 'b');
	board.add_piece(std::pair<char, char>('A' + 5, '1' + 7), 'b');

	// Add the kings and queens
	board.add_piece(std::pair<char, char>('A' + 3, '1' + 0), 'Q');
	board.add_piece(std::pair<char, char>('A' + 4, '1' + 0), 'K');
	board.add_piece(std::pair<char, char>('A' + 3, '1' + 7), 'q');
	board.add_piece(std::pair<char, char>('A' + 4, '1' + 7), 'k');
}

void Chess::handle_pawn_change(std::pair<char, char> position, char current)
{
	if (current == 'p' && position.second == '1')//if a black pawn arrives on first line
	{
		this->board.remove_piece(position);// remove the pawn 
		this->board.add_piece(position, 'q');// add a queen
	}
	if (current == 'P' && position.second == '8')// if a white pawn arrives on line 8
	{
		this->board.remove_piece(position);// remove the pawn
		this->board.add_piece(position, 'Q');// add a queen
	}
}

bool Chess::make_move(std::pair<char, char> start, std::pair<char, char> end) {
	if (start.first < 'A' || start.first>'H' || start.second < '1' || start.second>'8'
		|| end.first < 'A' || end.first>'H' || end.second < '1' || end.second>'8')
		return false; // check if the given coordinates are valid
	const Piece* current = this->board(start);
	if (current != NULL)
	{
		if (current->is_white() != this->is_white_turn)
			return false;
		if (this->board(end) == NULL)//if the move we are making does not imply capturing a piece
		{
			if (current->legal_move_shape(start, end))// and the move is legal in terms of piece movement
			{
				this->board.add_piece(end, current->to_ascii());// add the piece at the desired location
				this->board.remove_piece(start);// remove the piece from previous location
				if (this->in_check(is_white_turn))// if the move resulted in a chess for the current player
				{
					this->board.add_piece(start, this->board(end)->to_ascii());// revert the move
					this->board.remove_piece(end);
					return false;// signal unsuccesful move
				}
				this->is_white_turn = !this->is_white_turn;// a good move results in changing of turns
				this->handle_pawn_change(end, this->board(end)->to_ascii()); // handle the pawn change

				return true; // signal good move
			}

		}
		else
		{
			if (this->board(start)->is_white() == this->board(end)->is_white()) // cannot capture a piece of the same color;
				return false;
			if (this->board(end)->to_ascii() == 'k' || this->board(end)->to_ascii() == 'K')// cannot capture a king
				return false;
			if (current->legal_capture_shape(start, end))
			{
				char removed = this->board(end)->to_ascii();
				this->board.remove_piece(end);// remove the captured piece
				this->board.add_piece(end, current->to_ascii());// move the capturing piece to the new location
				this->board.remove_piece(start); // remove the piece from the old location
				if (this->in_check(is_white_turn))// if the move resulted in a chess for the current player
				{
					this->board.add_piece(start, this->board(end)->to_ascii());// revert the move
					this->board.remove_piece(end);
					this->board.add_piece(end, removed);
					return false;// signal unsuccesful move
				}
				this->is_white_turn = !this->is_white_turn;// a good move results in changing of turns
				this->handle_pawn_change(end, this->board(end)->to_ascii()); // handle the pawn change

				return true;
			}
		}
	}
	return false;
}


bool Chess::in_check(bool white) const {
	// detect the position of the king
	char c = white == true ? 'K' : 'k';
	bool found = false;
	std::pair<char, char> king_position;
	for (char i = 'A'; i <= 'H' && !found; i++)// search over the entire board 
		for (char j = '1'; j <= '8' && !found; j++)
			if (this->board(std::pair<char, char>(i, j)) != NULL && this->board(std::pair<char, char>(i, j))->to_ascii() == c)// if we find the king 
			{
				king_position = std::pair<char, char>(i, j); // mark the position
				found = true;
			}
	// check whether there is any enemy piece attacking the king
	for (char i = 'A'; i <= 'H'; i++)// search over the entire board
		for (char j = '1'; j <= '8'; j++)
			if (this->board(std::pair<char, char>(i, j)) != NULL && this->board(std::pair<char, char>(i, j))->is_white() != white)// if there is a piece at current position an it is an enemy piece
				if (this->board(std::pair<char, char>(i, j))->legal_capture_shape(std::pair<char, char>(i, j), king_position) == true)// if the found enemy piece can capture the king
					return true;// the current player is in check
	return false;
}

bool Chess::valid_move_exists(bool white) const
{
	// check if there is any possible move that would get it out of check
	for (char i = 'A'; i <= 'H'; i++)// search over the entire board
		for (char j = '1'; j <= '8'; j++)
			if (Utils::get_board()->get_at(std::pair<char, char>(i, j)) != NULL && Utils::get_board()->get_at(std::pair<char, char>(i, j))->is_white() == white)// when we find a piece of the current color
			{
				std::pair<char, char> current(i, j);
				for (char c = 'A'; c <= 'H'; c++)// try and move this piece to any location on the map
					for (char l = '1'; l <= '8'; l++)
					{
						std::pair<char, char> target(c, l);

						if (Utils::get_board()->get_at(target) == NULL)
						{
							if (Utils::get_board()->get_at(current)->legal_move_shape(current, target)) // if we find a location we can move this piece to
							{
								Utils::get_board()->add_piece(target, Utils::get_board()->get_at(current)->to_ascii());// add the piece at the desired location
								Utils::get_board()->remove_piece(current);// remove the piece from previous location
								if (!this->in_check(is_white_turn))// check if it solves the check
								{
									//if it solves the check
									Utils::get_board()->add_piece(current, Utils::get_board()->get_at(target)->to_ascii());// revert the move
									Utils::get_board()->remove_piece(target);
									return false;// it means it is not a check mate
								}
								Utils::get_board()->add_piece(current, Utils::get_board()->get_at(target)->to_ascii());// revert the move
								Utils::get_board()->remove_piece(target);
							}
						}
						else
						{
							if (Utils::get_board()->get_at(target)->is_white() != white) // cannot capture a piece of the same color;
								if (Utils::get_board()->get_at(target)->to_ascii() != 'k' && Utils::get_board()->get_at(target)->to_ascii() != 'K')// cannot capture a king
									if (Utils::get_board()->get_at(current)->legal_move_shape(current, target))
										return false;
						}
					}
			}

	return true;
}

bool Chess::in_mate(bool white) const {
	//check if the player is in check
	if (this->in_check(white) == false)
		return false;
	bool valid_move=valid_move_exists(white);
	// if(valid_move)
	// 	free(Utils::get_board());
	return valid_move;
}


bool Chess::in_stalemate(bool white) const {
	//check if the player is in check
	if (this->in_check(white) == true)
		return false;
	// check if there is any possible move
	return valid_move_exists(white);
}

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
std::ostream& operator<< (std::ostream& os, const Chess& chess) {
	// Write the board out and then either the character 'w' or the character 'b',
	// depending on whose turn it is
	return os << chess.get_board() << (chess.turn_white() ? 'w' : 'b');
}

void Chess::force_next_color(char color)
{
	if (color == 'b')
		this->is_white_turn = 0;
	else
		this->is_white_turn = 1;
}

void Chess::clear_board()
{
	this->board.clear_board();
}

void Chess::add_piece_to_board(std::pair<char, char> position, char piece_designator)
{
	this->board.add_piece(position, piece_designator);
}

std::istream& operator>> (std::istream& is, Chess& chess) {
	char piece;
	chess.clear_board();
	for (char i = '8'; i >= '1'; i--)//line by line
	{
		for (char j = 'A'; j <= 'H'; j++)//column by column
		{
			is >> piece;//read the board position
			if (piece != '-')//and if there is a piece on the position
				chess.add_piece_to_board(std::pair<char, char>(j, i), piece);//add it to the board
		}
	}
	is >> piece;
	chess.force_next_color(piece);
	return is;
}
