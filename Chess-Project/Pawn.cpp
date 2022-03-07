#include "Pawn.h"
#include "Utils.h"
bool Pawn::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const
{
	if (this->is_white())
	{
		// movement on empty board
		if (start.first != end.first)// can only move straight when not capturing
			return false;
		if (start.second > end.second)// can only move forward
			return false;
		if (end.second - start.second > 2)// can never move more than 2 fields
			return false;
		if (end.second - start.second == 2 && start.second!='2')// can move 2 fields only from initial position
			return false;
		// other pieces involved
		if (Utils::get_board()->get_at(std::pair<char, char>(start.first, start.second + 1)) != NULL)//there is a piece in front of the Pawn
			return false;
		return true;
	}
	else
	{
		// movement on empty board
		if (start.first != end.first)// can only move straight when not capturing
			return false;
		if (start.second < end.second)// can only move forward
			return false;
		if (start.second - end.second > 2)// can never move more than 2 fields
			return false;
		if (start.second - end.second == 2 && start.second != '7')// can move 2 fields only from initial position
			return false;
		// other pieces involved
		if (Utils::get_board()->get_at(std::pair<char, char>(start.first, start.second - 1)) != NULL) //there is a piece in front of the Pawn
			return false;
		return true;
	}
	return false;
}


bool Pawn::legal_capture_shape(std::pair<char, char> start, std::pair<char, char> end) const
{
	if ((start.first != end.first + 1 && start.first != end.first - 1))// can only capture pieces that are one column away
		return false;
	if (this->is_white() && start.second + 1 == end.second)//(white pawn) need to move diagonaly forward in orde to capture
			return true; 
	else if (!this->is_white() && start.second - 1 == end.second)//(black pawn) need to move diagonaly forward in orde to capture
			return true;
	return false;
}