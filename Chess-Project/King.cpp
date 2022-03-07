#include "King.h"
#include "Utils.h"
#include <math.h>
bool King::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const
{
	if (std::abs(start.first - end.first) > 1)// cannot move more than one field
		return false;
	if (std::abs(start.second - end.second) > 1)// cannot move more than one field
		return false;
	return true;
}


bool King::legal_capture_shape(std::pair<char, char> start, std::pair<char, char> end) const
{
	if (std::abs(start.first - end.first) > 1)// cannot move more than one field
		return false;
	if (std::abs(start.second - end.second) > 1)// cannot move more than one field
		return false;
	return true;
}