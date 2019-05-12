#ifndef __COMBAT_FRAME_H__
#define __COMBAT_FRAME_H__

#include "Engine/GameFrame.h"

class JetFighter;

class CombatFrame : public GameFrame {
private:
	JetFighter *playerJet;

public:
	CombatFrame();
	~CombatFrame();

	virtual bool updateFrame();
};

#endif