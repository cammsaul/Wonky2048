//
//  BoardLayer.h
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#ifndef WONKYBIRD_BOARDLAYER_H
#define WONKYBIRD_BOARDLAYER_H

#include "Board.h"
#include "TileNode.h"

namespace wonky2048 {
	class BoardLayer : public cocos2d::Layer {
	public:
		typedef shared_ptr<TileNode> TileNodePtr;
	
		BoardLayer();
		
		virtual bool init() override;
		
		cocos2d::Point BoardOffset() const;
		cocos2d::Point PointForPosition(TilePosition pos) const;
		float BoardSize() const;
		float TileSize() const;
		
	private:
		unordered_map<TilePtr, TileNodePtr> tileNodes_;
		shared_ptr<EventListenerKeyboard> eventListener_;
		Board board_;
		
		shared_ptr<LabelBMFont> scoreLabel_ = nullptr;
		
		void AddTileNode(TilePtr tile);
		void DrawTiles();
//		void MoveTileToPosition(TileNodePtr tile, TilePosition pos);
		
//		void KeyPressed(EventKeyboard::KeyCode, Event*);
		void KeyReleased(EventKeyboard::KeyCode, Event*);
	};
}

#endif