//
//  TilePosition.h
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#ifndef WONKY2048_TILEPOSITION_H
#define WONKY2048_TILEPOSITION_H

namespace wonky2048 {
	typedef std::pair<unsigned, unsigned> PosPair;
	
	class TilePosition : public PosPair {
	public:
		TilePosition() = default;
		TilePosition(unsigned row, unsigned col);
		
		unsigned Row() const { return PosPair::first; }
		unsigned Col() const { return PosPair::second; }
	};
}

ostream& operator<< (ostream& os, wonky2048::TilePosition pos);

template <>
class std::hash<wonky2048::TilePosition> : public std::unary_function<wonky2048::TilePosition&, int> {
public:
	size_t operator()(const wonky2048::TilePosition& pos) const {
		return (pos.Col() * 4) + pos.Row();
	}
};


#endif