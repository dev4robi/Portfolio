#ifndef __TEST_2D_WIDGET_H__
#define __TEST_2D_WIDGET_H__

#include "Engine/GameWidget.h"

class Test2DWidget : public GameWidget {
public:
	Test2DWidget();
	Test2DWidget(Vector3 pos);
	~Test2DWidget();

	virtual bool draw();
};

#endif