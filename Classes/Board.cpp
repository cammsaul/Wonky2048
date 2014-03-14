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
	
	Board::Board() {
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
				
				cout << "Board: position is free : " << pos << endl;
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
		
		auto tile = make_shared<Tile>(2);
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
		assert(false); // Tile is not on the board.
	}
	
	void Board::MoveTile(TilePosition startPos, TilePosition destPos) {
		assert(find(startPos) != end()); // no tile at start pos
		assert(find(destPos) == end()); // position not empty
		
		TilePtr tile = (*this)[startPos];
		erase(find(startPos));
		
		emplace(destPos, tile);
	}
	
	void Board::ApplyLeft() {
		// move all tiles to the leftmost open position
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				bool hasTile = find({row, col}) != end();
				if (hasTile) {
					auto open = UnfilledPositions([=](int r, int){ return r == row; });
					if (!open.empty()) {
						// move to first (leftmost) position
						MoveTile({row, col}, open[0]);
					}
				}
			}
		}
		
		try {
			AddTileAtRandomPosition();
		} catch (...) {}
	}
	
	void Board::ApplyRight() {
		// move all tiles to the leftmost open position
		for (int row = 0; row < 4; row++) {
			for (int col = 3; col >= 0; col--) {
				bool hasTile = find({row, col}) != end();
				if (hasTile) {
					auto open = UnfilledPositions([=](int r, int){ return r == row; });
					if (!open.empty()) {
						// move to first (leftmost) position
						MoveTile({row, col}, open[open.size() - 1]);
					}
				}
			}
		}
		
		try {
			AddTileAtRandomPosition();
		} catch (...) {}
	}
	
	void Board::ApplyUp() {
		for (int row = 3; row >= 0; row--) {
			for (int col = 0; col < 4; col++) {
				bool hasTile = find({row, col}) != end();
				if (hasTile) {
					auto open = UnfilledPositions([=](int, int c){ return c == col; });
					if (!open.empty()) {
						// move to first (leftmost) position
						MoveTile({row, col}, open[open.size() - 1]);
					}
				}
			}
		}
		
		try {
			AddTileAtRandomPosition();
		} catch (...) {}
	}
	
	void Board::ApplyDown() {
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				bool hasTile = find({row, col}) != end();
				if (hasTile) {
					auto open = UnfilledPositions([=](int, int c){ return c == col; });
					if (!open.empty()) {
						// move to first (leftmost) position
						MoveTile({row, col}, open[0]);
					}
				}
			}
		}
		
		try {
			AddTileAtRandomPosition();
		} catch (...) {}
	}
}

ostream& operator<< (ostream& os, const wonky2048::Board& board) {
	os << endl;
	for (int row = 3; row >= 0; row--) {
		for (int col = 0; col < 4; col ++) {
			wonky2048::TilePosition pos {row, col};
			if (board[pos]) os << board[pos]->Value() << " ";
			else			os << "_ ";
		}
		os << endl;
	}
	return os;
}