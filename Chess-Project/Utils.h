#pragma once
#include "Board.h"
class Utils
{
public: 
	static void set_board(Board* board);
	static Board* get_board();
private:
	static Board* board;
	Utils() {}
};

