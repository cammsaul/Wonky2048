//
//  Tile.cpp
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#include "Tile.h"

namespace wonky2048 {
	Tile::Tile(unsigned value):
		value_(value)
	{}
	
	void Tile::SetValue(unsigned value) {
		assert(value % 2 == 0);
		value_ = value;
	}
	
	void Tile::operator *=(int val) {
		assert(val == 2); // don't attempt to *= by non-multiple-of-2
		value_ *= val;
	}
}