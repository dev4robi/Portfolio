#ifndef __TEST_3D_WIDGET_H__
#define __TEST_3D_WIDGET_H__

#include "Engine/GameWidget.h"
#include "Library/glm/glm.h"

class Test3DWidget : public GameWidget {
public:
	char modelName[64];
	GLMmodel* pModel;

	Test3DWidget(char modelName[64]);
	Test3DWidget(Vector3 pos);
	~Test3DWidget();

	virtual bool draw();
};

#endif