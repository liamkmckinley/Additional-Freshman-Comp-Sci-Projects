#include "Queen.h"
#include "Utils.h"
bool Queen::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const
{
	if((start.first != end.first && start.second != end.second) // the move is not only a line or a column (like a Rook)
		&& (abs(start.first - end.first) != abs(start.second - end.second)))// and the moove is not only diagonally (like a Bishop)
	return false;// the move is invalid

	// utility variables used to follow the path of the queen on the board
	int column_incremental = 0, line_incremental = 0; // zero means we don't change the line or column
	if (start.first > end.first)
		column_incremental = -1;
	else if (start.first < end.first)
		column_incremental = 1;
	if (start.second > end.second)
		line_incremental = -1;
	else if (start.second < end.second)
		line_incremental = 1;
	if (line_incremental == 0 || column_incremental == 0)// Queen behaves like a Rook
	{
		if (start.first != end.first) // if the rook moves on a column
		{
			for (char i = start.first + column_incremental; i != end.first; i += column_incremental)
				if (Utils::get_board()->get_at(std::pair<char, char>(i, start.second)))// if there is a piece on the way
					return false;// move is invalid
		}
		else// if the rook moves on a line
		{
			for (char i = start.second + line_incremental; i != end.second; i += line_incremental)
				if (Utils::get_board()->get_at(std::pair<char, char>(start.first, i)))// if there is a piece on the way
					return false;// move is invalid
		}
	}
	else // Queen behaves like a Bishop
	{
		for (char i = start.first + column_incremental, j = start.second + line_incremental; //start one position away in the direction of movement
			i != end.first && j != end.second; // to the final position
			i += column_incremental, j += line_incremental) // move in the right dirrection
			if (Utils::get_board()->get_at(std::pair<char, char>(i, j)) != NULL)// if there is a piece in the way, the move can't be done
				return false;
	}
	return true;
}

