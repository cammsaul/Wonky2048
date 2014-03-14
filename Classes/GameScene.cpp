//
//  GameScene.cpp
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#include "GameScene.h"


namespace wonky2048 {
	GameScene::GameScene()
	{}

	bool GameScene::init() {
		auto backgroundLayer = LayerColor::create(Color4B::WHITE);
		addChild(backgroundLayer);
		
		boardLayer_.init();
		auto boardSize = getContentSize().width * 0.8f;
		boardLayer_.setContentSize({boardSize, boardSize});
		addChild(&boardLayer_);
		
		return true;
	}
}