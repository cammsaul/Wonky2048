//
//  BoardLayer.cpp
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#include "BoardLayer.h"

inline bool IsDown (EventKeyboard::KeyCode keyCode) { return keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW; }
inline bool IsUp   (EventKeyboard::KeyCode keyCode) { return keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW; }
inline bool IsLeft (EventKeyboard::KeyCode keyCode) { return keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW; }
inline bool IsRight(EventKeyboard::KeyCode keyCode) { return keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW; }

namespace wonky2048 {
	BoardLayer::BoardLayer():
		eventListener_(EventListenerKeyboard::create())
	{
	}
	
	bool BoardLayer::init() {
		auto backgroundLayer = LayerColor::create(Color4B::GRAY);
		
		backgroundLayer->setContentSize({BoardSize(), BoardSize()});
		backgroundLayer->setPosition(BoardOffset());
		addChild(backgroundLayer, 0);
		
		using namespace std::placeholders;
//		eventListener_->onKeyPressed = std::bind(&BoardLayer::KeyPressed, this, _1, _2);
		eventListener_->onKeyReleased = std::bind(&BoardLayer::KeyReleased, this, _1, _2);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener_.get(), this);
		
		DrawTiles();
		
		return true;
	}
	
	void BoardLayer::AddTileNode(TilePtr tile) {
		TilePosition pos = board_.PositionOfTile(tile);
		TileNodePtr tileNode = make_shared<TileNode>(tile);
		
		tileNodes_.emplace(tile, tileNode);
		addChild(tileNode.get(), 1);
		tileNode->setPosition(PointForPosition(pos));
		tileNode->init();
	}
	
	Point BoardLayer::BoardOffset() const {
		const float screenWidth = Director::getInstance()->getWinSizeInPixels().width;
		const float screenHeight = Director::getInstance()->getWinSizeInPixels().height;
		return {(screenWidth - BoardSize()) / 2.0f, (screenHeight - BoardSize()) / 2.0f};
	}
	
	float BoardLayer::BoardSize() const {
		const float screenWidth = Director::getInstance()->getWinSizeInPixels().width;
		const float screenHeight = Director::getInstance()->getWinSizeInPixels().height;
		return min<float>(screenWidth, screenHeight) * 0.8f;
	}
	
	float BoardLayer::TileSize() const {
		return BoardSize() / 4.0f;
	}
	
	Point BoardLayer::PointForPosition(TilePosition pos) const {
		float x = (pos.Col() * TileSize()) + (TileSize() / 2.0f) + BoardOffset().x;
		float y = (pos.Row() * TileSize()) + (TileSize() / 2.0f) + BoardOffset().y;
		return {x, y};
	}
	
	void BoardLayer::DrawTiles() {
		cout << "Board is now: " << board_ << endl;
		for (auto t : board_) {
			TilePosition pos = t.first;
			TilePtr tile = t.second;
			if (tileNodes_.find(tile) == tileNodes_.end()) {
				AddTileNode(tile);
			}
		}
		
		for (auto tileNode : tileNodes_) {
			TilePtr tile = tileNode.first;
			TileNodePtr node = tileNode.second;
			
			node->runAction(MoveTo::create(0.3f, PointForPosition(board_.PositionOfTile(tile))));
		}
	}
//	
//	void BoardLayer::MoveTileToPosition(TileNodePtr tile, TilePosition pos) {
//		tile->setPosition(PointForPosition(pos));
//	}
	
	void BoardLayer::KeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
		printf("Read key: left: %d, right: %d, up: %d, down: %d\n", IsLeft(keyCode), IsRight(keyCode), IsUp(keyCode), IsDown(keyCode));
		if (IsLeft(keyCode))	board_.ApplyLeft();
		if (IsRight(keyCode))	board_.ApplyRight();
		if (IsUp(keyCode))		board_.ApplyUp();
		if (IsDown(keyCode))	board_.ApplyDown();
		
		DrawTiles();
	}

}