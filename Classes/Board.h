//
//  Board.h
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#ifndef WONKY2048_BOARD_H
#define WONKY2048_BOARD_H

#include "TilePosition.h"
#include "Tile.h"

namespace wonky2048 {
	typedef shared_ptr<Tile> TilePtr;
	class Board : public unordered_map<TilePosition, TilePtr> {
	public:
		Board();
		
		vector<TilePosition> UnfilledPositions(std::function<bool(int row, int col)> pred = [](int,int){ return true; }) const; ///< optional : specify predicate to filter
		
		TilePtr AddTileAtPosition(TilePosition pos);
		
		TilePtr AddTileAtRandomPosition();
		TilePtr AddTileAtRandomPosition(TilePosition* pos); ///< Add tile at random unfilled position
		
		TilePtr operator[] (TilePosition pos) const;
		
		TilePosition PositionOfTile(TilePtr tile) const;
		
		void ApplyLeft();
		void ApplyRight();
		void ApplyUp();
		void ApplyDown();
	private:
		void MoveTile(TilePosition startPos, TilePosition destPos);
	};
}

ostream& operator<< (ostream& os, const wonky2048::Board& board);

#endif
