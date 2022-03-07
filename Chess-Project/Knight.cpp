#include "Knight.h"
#include "Utils.h"
#include <math.h>
bool Knight::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const
{
	if ((std::abs(start.first - end.first) == 2 && std::abs(start.second - end.second) == 1) ||
		(std::abs(start.first - end.first) == 1 && std::abs(start.second - end.second) == 2))
		return true;
	return false;
}
