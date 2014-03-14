//
//  TilePosition.cpp
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//
#include "TilePosition.h"

namespace wonky2048 {
	TilePosition::TilePosition(unsigned row, unsigned col):
	pair<unsigned, unsigned>(row, col)
	{
		if (row > 3) throw runtime_error { "Invalid row! " + to_string(row) };
		if (col > 3) throw runtime_error { "Invalid col! " + to_string(col) };
	}
}

ostream& operator<< (ostream& os, wonky2048::TilePosition pos) {
	os << pos.Col() << ", " << pos.Row();
	return os;
}