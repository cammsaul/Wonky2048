//
//  GameScene.h
//  Wonky2048
//
//  Created by Cam Saul on 3/13/14.
//
//

#ifndef WONKYBIRD_GameScene_H
#define WONKYBIRD_GameScene_H

#include "BoardLayer.h"

namespace wonky2048 {
	class GameScene : public Scene {
	public:
		GameScene();
		
		virtual bool init() override;
	private:
		BoardLayer boardLayer_;
	};
}

#endif