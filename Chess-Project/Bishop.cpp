#include "Bishop.h"
#include "Utils.h"
#include <math.h>
bool Bishop::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const
{
	if (abs(start.first - end.first) != abs(start.second - end.second)) // can only move diagonally 
		return false;// so the differences in column and lines between start and end position should be equal
	// utility variables used to follow the path of the bishop on the board
	int column_incremental = 1, line_incremental = 1;
	if (start.first > end.first)
		column_incremental = -1;
	if (start.second > end.second)
		line_incremental = -1;
	// follow the path of the bishop
	for (char i = start.first + column_incremental, j = start.second + line_incremental; //start one position away in the direction of movement
		i != end.first && j != end.second; // to the final position
		i += column_incremental, j += line_incremental) // move in the right dirrection
		if (Utils::get_board()->get_at(std::pair<char, char>(i, j)) != NULL)// if there is a piece in the way, the move can't be done
			return false;
	return true;
}

