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
				
		TilePtr operator[] (TilePosition pos) const;
		
		TilePosition PositionOfTile(TilePtr tile) const;
		
		unsigned Score() const { return score_; }
		
		bool HasTile(int row, int col) const;
		bool HasTile(TilePosition pos) const;
		
		unsigned ValueOfTileAt(int row, int col) const; ///< 0 if no tile
		unsigned ValueOfTileAt(TilePosition pos) const; ///< 0 if no tile
		
		void ApplyLeft();
		void ApplyRight();
		void ApplyUp();
		void ApplyDown();
	private:
		unsigned score_ = 0;
		
		void ApplyDirection(function<void()> fn);
		void ApplyLeft_();
		void ApplyRight_();
		void ApplyUp_();
		void ApplyDown_();
		
		void MergeTiles(TilePosition tileToKeepPos, TilePosition tileToRemovePos);

		vector<TilePosition> UnfilledPositions(std::function<bool(int row, int col)> pred = [](int,int){ return true; }) const; ///< optional : specify predicate to filter

		TilePtr AddTileAtPosition(TilePosition pos);
		
		TilePtr AddTileAtRandomPosition();
		TilePtr AddTileAtRandomPosition(TilePosition* pos); ///< Add tile at random unfilled position
		
		void MoveTile(TilePosition startPos, TilePosition destPos);
		
		void RemoveTileAtPosition(TilePosition pos);
	};
}

ostream& operator<< (ostream& os, const wonky2048::Board& board);

#endif
