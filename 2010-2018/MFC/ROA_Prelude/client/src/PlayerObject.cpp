#include "stdafx.h"
#include "ROAdat.h"
#include "PlayerObject.h"

PlayerObject::PlayerObject()
{
	CamX = 0;
	CamY = 0;
	CharWndOn = FALSE;
}

PlayerObject::~PlayerObject()
{

}

void PlayerObject::MoveCamera(int x, int y)
{
	CamX = x;
	CamY = y;
}