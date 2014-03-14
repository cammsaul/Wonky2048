//
//  TileNode.h
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#ifndef WONKYBIRD_TileNode_H
#define WONKYBIRD_TileNode_H

#include "Tile.h"

namespace wonky2048 {
	typedef shared_ptr<wonky2048::Tile> TilePtr;
	class TileNode : public LabelBMFont {
	public:
		TileNode(TilePtr tile = make_shared<wonky2048::Tile>(2));
		
		const TilePtr Tile() const { return tile_; }
		void SetTile(TilePtr tile);
		
		virtual bool init() override;
	private:
		TilePtr tile_;
	};
}

#endif