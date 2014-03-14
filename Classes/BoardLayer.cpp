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
		
		scoreLabel_ = make_shared<LabelBMFont>();
		scoreLabel_->initWithString("SCORE: 0", "Font_Score_Small.fnt");
		scoreLabel_->setScale(0.5f);
		scoreLabel_->setPosition(PointForPosition({0,3}).x, BoardOffset().y + BoardSize() + 10);
		addChild(scoreLabel_.get());
		
		using namespace std::placeholders;
//		eventListener_->onKeyPressed = std::bind(&BoardLayer::KeyPressed, this, _1, _2);
		eventListener_->onKeyReleased = std::bind(&BoardLayer::KeyReleased, this, _1, _2);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener_.get(), this);
		
//		// add debug labels
//		for (int row = 0; row < 4; row++) {
//			for (int col = 0; col < 4; col++) {
//				auto* label = LabelBMFont::create(to_string(row) + ", " + to_string(col), "Font_Score_Small.fnt");
//				label->setScale(0.5f);
//				label->setPosition(PointForPosition({row, col}));
//				addChild(label);
//			}
//		}
		
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
		
		tileNode->setScale(0.0f);
		tileNode->runAction(ScaleTo::create(0.2f, 0.8f));
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
		float y = ((3 - pos.Row()) * TileSize()) + (TileSize() / 2.0f) + BoardOffset().y;
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
			node->setString(to_string(tile->Value()));
			try {
				auto pos = board_.PositionOfTile(tile);
				node->runAction(MoveTo::create(0.2f, PointForPosition(pos)));
			} catch (...) {
				node->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.2f, 0.0f), CallFunc::create([=](){
					tileNodes_.erase(tile);
					node->removeFromParentAndCleanup(true); // tile no longer on board
				})));
			}
		}
		
		scoreLabel_->setString("SCORE: " + to_string(board_.Score()));
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