//
//  Tile.h
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#ifndef WONKY2048_TILE_H
#define WONKY2048_TILE_H

namespace wonky2048 {
	class Tile {
	public:
		Tile(unsigned value = 2);
		
		unsigned Value() const { return value_; }
		void SetValue(unsigned value);
		
		void operator *=(int val);
	private:
		unsigned value_;
	};
}

#endif
