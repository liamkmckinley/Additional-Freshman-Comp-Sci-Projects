#include "Utils.h"

Board* Utils::board=NULL;
void Utils::set_board(Board* board)
{
	Utils::board = board;
}

Board* Utils::get_board()
{
	return Utils::board;
}