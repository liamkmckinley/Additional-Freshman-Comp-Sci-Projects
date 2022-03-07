#include "Rook.h"
#include "Utils.h"
bool Rook::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const
{
	if(start.first!=start.first && end.second!=end.second)//Rooks move in straight lines, so only one of the coordinates can change
		return false;
	int increment=1;// utility variable used to follow the path of the rook
	if (start.first != end.first) // if the rook moves on a column
	{
		if (start.first > end.first)
			increment = -1;// decide the direction of movement on the column
		for (char i = start.first+increment; i != end.first; i += increment)
			if (Utils::get_board()->get_at(std::pair<char, char>(i, start.second)))// if there is a piece on the way
				return false;// move is invalid
	}
	else// if the rook moves on a line
	{
		if (start.second > end.second)
			increment = -1;// decide the direction of movement on the line
		for (char i = start.second + increment; i != end.second; i += increment)
			if (Utils::get_board()->get_at(std::pair<char, char>(start.first, i)))// if there is a piece on the way
				return false;// move is invalid
	}
	return true;
}
