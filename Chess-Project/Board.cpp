#include <iostream>
#include <utility>
#include <map>
#include <string>
#include <cctype>
#include "Board.h"
#include "Utils.h"
#include "CreatePiece.h"


/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
Board::Board() {
	Utils::set_board(this);
}

const Piece* Board::operator()(std::pair<char, char> position) const {
	//MODIFIED
	if (occ.find(position) != occ.end())//check if there exists a piece at the given position
		return occ.find(position)->second;// return the piece
	return NULL;//return null otherwise
}


bool Board::add_piece(std::pair<char, char> position, char piece_designator) {
	// Returns false if:
	std::string validDesignators = "kqbnrpmKQBNRPM";
	if (validDesignators.find(piece_designator) == std::string::npos)	// -- the designator is invalid,
		return false;
	if (position.first < 'A' || position.first > 'H' || position.second < '1' || position.second > '8')// -- the specified location is not on the board, or
		return false;
	if (occ.find(position) != occ.end())	// -- if the specified location is occupied
		return false;
	occ[position] = create_piece(piece_designator);
	return true;
}


bool Board::remove_piece(std::pair<char, char> position)
{
	if (occ.find(position) != occ.end())//check if there exists a piece at the given position
	{
		delete occ.find(position)->second; // free memory
		occ.erase(occ.find(position)); // remove pair from map
		return true;
	}
	return false;
}

bool Board::has_valid_kings() const {
	int kingsCount = 0;
	std::map<std::pair<char, char>, Piece*>::const_iterator it;
	for (it = occ.begin(); it != occ.end(); it++)
	{
		if (it->second->to_ascii() == 'k' || it->second->to_ascii() == 'K')
			kingsCount++;
	}
	if (kingsCount == 2)
		return true;
	return false;
}

void Board::display() const {
	/////////////////////////
	// [REPLACE THIS STUB] //
	/////////////////////////
	//MODIFIED, TO BE CUSTOMIZED
	std::cout << *this;
}

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Board& board) {
	for (char r = '8'; r >= '1'; r--) {
		for (char c = 'A'; c <= 'H'; c++) {
			const Piece* piece = board(std::pair<char, char>(c, r));
			if (piece) {
				os << piece->to_ascii();
			}
			else {
				os << '-';
			}
		}
		os << std::endl;
	}
	return os;
}

void Board::clear_board()
{
	occ.clear();
}
