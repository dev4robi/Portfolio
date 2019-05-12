#include "Engine/ResourceManager.h"
#include "Test/Test3DWidget.h"

Test3DWidget::Test3DWidget(char modelName[64]) {
	pModel = g_pGameManager->getResourceManagerPtr()->getObjModelPtr(modelName);
}

Test3DWidget::Test3DWidget(Vector3 pos) : GameWidget(pos) {

}

Test3DWidget::~Test3DWidget() {

}

bool Test3DWidget::draw() {
	if (pModel != NULL) {
		glTranslatef(getPosX(), getPosY(), getPosZ());	// �������� 1.5, ȭ��������� 6.0 ������ŭ �̵�
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		//glmDraw(pModel, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
	}
	else {
		return false;
	}
	
	return true;
}
