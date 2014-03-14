//
//  Board.cpp
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#include "Board.h"

namespace wonky2048 {
	typedef unordered_map<TilePosition, TilePtr> um;
	
	Board::Board()
	{
		// start with 2 rando tiles
		AddTileAtRandomPosition();
		AddTileAtRandomPosition();
	}

	vector<TilePosition> Board::UnfilledPositions(std::function<bool(int row, int col)> pred) const {
		vector<TilePosition> positions;
		for (unsigned row = 0; row < 4; row++) {
			for (unsigned col = 0; col < 4; col++) {
				if (!pred(row, col)) continue;
				
				TilePosition pos {row, col};
				if (find(pos) != end()) continue;
				
//				cout << "Board: position is free : " << pos << endl;
				positions.push_back(pos);
			}
		}
		return positions;
	}
	
	TilePtr Board::AddTileAtRandomPosition() {
		TilePosition p;
		return AddTileAtRandomPosition(&p);
	}

	TilePtr Board::AddTileAtPosition(TilePosition pos) {
		cout << "Adding tile at position " << pos << endl;
		if (find(pos) != end()) {
			throw runtime_error { "There is already a tile at: " + to_string(pos.Row()) + ", " + to_string(pos.Col()) };
		}
		
		// 10% chance of being a 4
		auto val = (rand() % 10 == 0) ? 4 : 2;
		
		auto tile = make_shared<Tile>(val);
		um::emplace(pos, tile);
		
		return tile;
	}
	
	TilePtr Board::AddTileAtRandomPosition(TilePosition *pos) {
		auto unfilledPositions = UnfilledPositions();
		if (unfilledPositions.empty()) {
			throw runtime_error { "There are no remaining unfilled positions." };
		}
		srand(time(NULL));
		
		auto idx = rand() % (int)unfilledPositions.size();
		cout << "Selecting random index: " << idx << endl;
		*pos = unfilledPositions[idx];
		return AddTileAtPosition(*pos);
	}
	
	TilePtr Board::operator[] (TilePosition pos) const {
		auto itr = find(pos);
		if (itr == end()) return nullptr;
		return itr->second;
	}
	
	TilePosition Board::PositionOfTile(TilePtr tile) const {
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				TilePosition pos { row, col };
				auto itr = find(pos);
				if (itr != end() && itr->second == tile) {
					return pos;
				}
			}
		}
		throw runtime_error { "Tile is not on the board." };
	}
	
	void Board::MoveTile(TilePosition startPos, TilePosition destPos) {
		cout << "Move tile from " << startPos << " to " << destPos << endl;
		assert(find(startPos) != end()); // no tile at start pos
		assert(find(destPos) == end()); // position not empty
		
		TilePtr tile = (*this)[startPos];
		erase(find(startPos));
		
		emplace(destPos, tile);
	}
	
	void Board::MergeTiles(TilePosition tileToKeepPos, TilePosition tileToRemovePos) {
		auto tile = (*this)[tileToKeepPos];
		*tile *= 2;
		score_ += tile->Value();
		RemoveTileAtPosition(tileToRemovePos);
	}
	
	void Board::ApplyLeft_() {
		// move all tiles to the leftmost open position
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				if (HasTile(row, col)) {
					// try to merge to the left
					if (col > 0) {
						auto leftVal = ValueOfTileAt(row, col-1);
						auto thisVal = ValueOfTileAt(row, col);
						if (leftVal == thisVal) {
							MergeTiles({row, col-1}, {row, col});
							continue;
						}
					}
					auto open = UnfilledPositions([=](int r, int c){ return r == row && c < col; });
					if (!open.empty()) {
						// move to first (leftmost) position
						MoveTile({row, col}, open[0]);
					}
				}
			}
		}
	}
	
	void Board::ApplyRight_() {
		// move all tiles to the leftmost open position
		for (int row = 0; row < 4; row++) {
			for (int col = 3; col >= 0; col--) {
				if (HasTile(row, col)) {
					if (col < 3) {
						auto rightVal = ValueOfTileAt(row, col+1);
						auto thisVal = ValueOfTileAt(row, col);
						if (rightVal == thisVal) {
							MergeTiles({row, col+1}, {row, col});
							continue;
						}
					}
					
					auto open = UnfilledPositions([=](int r, int c){ return r == row && c > col; });
					if (!open.empty()) {
						// move to first (leftmost) position
						MoveTile({row, col}, open[open.size() - 1]);
					}
				}
			}
		}
	}
	
	void Board::ApplyDown_() {
		for (int row = 3; row >= 0; row--) {
			for (int col = 0; col < 4; col++) {
				if (HasTile(row, col)) {
					if (row < 3) {
						auto downVal = ValueOfTileAt(row+1, col);
						auto thisVal = ValueOfTileAt(row, col);
						if (downVal == thisVal) {
							MergeTiles({row+1, col}, {row, col});
							continue;
						}
					}
					
					auto open = UnfilledPositions([=](int r, int c){ return c == col && r > row; });
					if (!open.empty()) {
						// move to first (leftmost) position
						MoveTile({row, col}, open[open.size() - 1]);
					}
				}
			}
		}
	}
	
	void Board::ApplyUp_() {
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				if (HasTile(row, col)) {
					if (row > 0) {
						auto upVal = ValueOfTileAt(row-1, col);
						auto thisVal = ValueOfTileAt(row, col);
						if (upVal == thisVal) {
							MergeTiles({row-1, col}, {row, col});
							continue;
						}
					}
					
					auto open = UnfilledPositions([=](int r, int c){ return c == col && r < row; });
					if (!open.empty()) {
						// move to first (leftmost) position
						MoveTile({row, col}, open[0]);
					}
				}
			}
		}
	}
	
	void Board::ApplyDirection(function<void()> fn) {
		fn();
//		fn();
		try {
			AddTileAtRandomPosition();
		} catch (...) {}
	}
	
	void Board::ApplyLeft() {
		ApplyDirection(bind(&Board::ApplyLeft_, this));
	}
	
	void Board::ApplyRight() {
		ApplyDirection(bind(&Board::ApplyRight_, this));
	}
	
	void Board::ApplyUp() {
		ApplyDirection(bind(&Board::ApplyUp_, this));
	}
	
	void Board::ApplyDown() {
		ApplyDirection(bind(&Board::ApplyDown_, this));
	}
	
	bool Board::HasTile(int row, int col) const {
		return HasTile({row, col});
	}
	
	bool Board::HasTile(TilePosition pos) const {
		return find(pos) != end();
	}
	
	unsigned Board::ValueOfTileAt(int row, int col) const {
		return ValueOfTileAt({row, col});
	}
	
	unsigned Board::ValueOfTileAt(TilePosition pos) const {
		auto itr = find(pos);
		if (itr == end()) return 0;
		return itr->second->Value();
	}
	
	void Board::RemoveTileAtPosition(TilePosition pos) {
		auto itr = find(pos);
		assert(itr != end());
		erase(itr);
	}

}

ostream& operator<< (ostream& os, const wonky2048::Board& board) {
	os << endl;
	os << "Score: " << board.Score() << endl;
	os << "   0  1  2  3" << endl << endl;
	for (int row = 0; row < 4; row++) {
		os << row << "   ";
		for (int col = 0; col < 4; col ++) {
			wonky2048::TilePosition pos {row, col};
			if (board.HasTile(pos)) os << board[pos]->Value() << "  ";
			else			os << "_  ";
		}
		os << endl;
	}
	return os;
}