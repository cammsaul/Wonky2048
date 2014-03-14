//
//  TileNode.cpp
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#include "TileNode.h"

namespace wonky2048 {
	TileNode::TileNode(TilePtr tile):
		tile_(tile)
	{}

	bool TileNode::init() {
		return LabelBMFont::initWithString(to_string(tile_->Value()), "Font_Score_Large.fnt");
	}

	void TileNode::SetTile(TilePtr tile) {
		tile_ = tile;
		LabelBMFont::setString(to_string(tile_->Value()));
	}
}