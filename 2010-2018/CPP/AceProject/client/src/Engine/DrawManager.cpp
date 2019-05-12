#include "Engine/GameManager.h"
#include "Engine/GameWindow.h"
#include "Engine/GameFrame.h"
#include "Engine/GameWidget.h"
#include "Engine/GameCamera.h"
#include "Engine/DrawManager.h"
#include "Engine/CollisionSytem.h"
#include "Game/Object/Skybox.h"
#include "Game/Object/Terrain.h"
#include "Game/Object/Player.h"
#include "Game/Effect/GL_Effect.h"

GameCamera* DrawManager::getCameraFromFrame() {
	GameFrame *pFrame = g_pGameManager->getGameFramePtr();
	return pFrame->getOwnerPlayerPtr()->getPlayerCamera();
}

DrawManager::DrawManager(HDC* hdc, HGLRC* hglrc, Size2D* resolution) {
	// 1. ���� �������� HDC�� HGLRC�� �����ͷ� ����Ų��
	if (hdc != NULL && hglrc != NULL) {
		pHDC = hdc;
		pHGLRC = hglrc;
	}
	else {
		MessageBox(NULL, TEXT("DC null pointer error."), TEXT("DrawManager::DrawManager() error"), (MB_OK | MB_ICONINFORMATION));
		exit(1);
	}

	// 2. �ػ󵵸� �����Ѵ�
	pResolution = resolution;

	// 3. OpenGL�� �ʱ�ȭ�Ѵ�
	if (initGL() == true) {

	}
	else {
		MessageBox(NULL, TEXT("OpenGL initialization error."), TEXT("DrawManager::DrawManager() error"), (MB_OK | MB_ICONINFORMATION));
		exit(1);
	}
}

DrawManager::~DrawManager() {
	// ī�޶� ��ü�� �ı�
	if (pCamera != NULL) {
		delete pCamera;
		pCamera = NULL;
	}

	// �� �� �����ϴ� �����͸� �η� ����
	pHDC = NULL;
	pHGLRC = NULL;
	pDrawList = NULL;
}

Size2D* DrawManager::getResolutionPtr() {
	return pResolution;
}

void DrawManager::setBufferClearColor(Vector3 color) {
	glClearColor(color.x, color.y, color.z, 1.00f);
}

Vector3 DrawManager::get2dPostionFromVector3(Vector3 vec) {
	Vector3 rtScrPos;

	vec = mymath::Vecor3Transform(vec, pCamera->getViewMatrix());
	vec = mymath::Vecor3Transform(vec, pCamera->getProjectionMatrix());
	vec.x = vec.x / (vec.z == 0 ? 0.0001 : vec.z);
	vec.y = vec.y / (vec.z == 0 ? 0.0001 : vec.z);
	vec.x = (vec.x + 1) * pResolution->getWidth() / 2.0;
	vec.y = (vec.y + 1) * pResolution->getHeight() / 2.0;

	rtScrPos.x = vec.x;
	rtScrPos.y = vec.y;
	rtScrPos.z = 0.0;

	return rtScrPos;
}

bool DrawManager::initGL() {
	glShadeModel(GL_SMOOTH);							// �ε巯�� ���̵� ���
	glClearColor(0.0, 0.0, 0.0, 0.0);					// �⺻ ����� ������ ���
	glClearDepth(1.0);									// ���� ���� ����
	glEnable(GL_DEPTH_TEST);							// �����׽�Ʈ�� ���
	glDepthFunc(GL_LEQUAL);								// ����� ���� �׽�Ʈ�� ����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// �ְ� ������ ���� ��� ���

	return true;	// �ʱ�ȭ�� ���������� �Ϸ�
}

bool DrawManager::draw() {
	// 1. �׷��� �� �������� ���� ����Ʈ�� ī�޶� ã��
	GameFrame *pFrame = (g_pGameManager->getGameFramePtr());
	pDrawList = (pFrame->getDrawListPtr());
	pEffectList = (pFrame->getEffectListPtr());
	pCamera = getCameraFromFrame();

	// 2. �׸��� ����Ʈ �����Ͱ� �ִ� ��� (GameFrame�� widgetList�� �����ϰ� �ִ� ���)
	if (pCamera != NULL && pDrawList != NULL) {
		// 3-1. 2D����Ʈ�� ���� ����Ʈ ���� �� ���� �ʱ�ȭ
		static list<GameWidget*> temp2DWigetList;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// 3-2. GL�ɼ�
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_LINE_SMOOTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		// 3-3. ����Ʈ �ʱ�ȭ
		Size2D size(g_pGameManager->getGameWndPtr()->getWndSize());
		glViewport(0, 0, size.getWidth(), size.getHeight());
		
		// 4. 3D������ ����Ʈ �׸���
		glPushMatrix();
		{
			// 4-1. ��Ʈ���� �ʱ�ȭ �� ī�޶� ����
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glLoadMatrixf(&pCamera->getProjectionMatrix()[0][0]);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glLoadMatrixf(&pCamera->getViewMatrix()[0][0]);

			// �ϴ� ���� �׸���
			glPushMatrix();
			{
				Vector3 camPos = pCamera->getPosition();
				glTranslatef(camPos.x, camPos.y, camPos.z);
				if (pFrame->getSkyboxPtr() != NULL) pFrame->getSkyboxPtr()->draw();
			}
			glPopMatrix();

			// ���� �׸���
			glPushMatrix();
			{
				if (pFrame->getTerrainPtr() != NULL) pFrame->getTerrainPtr()->draw();
			}
			glPopMatrix();

			// 4-2. ����Ʈ �׸���
			static list<GL_Effect*> tempEffectList;
			list<GL_Effect*>::iterator iterEffect = pEffectList->begin();
			glPushMatrix();
			{
				// ��Ʈ���� �ʱ�ȭ �� ī�޶� ����
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glLoadMatrixf(&pCamera->getProjectionMatrix()[0][0]);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glLoadMatrixf(&pCamera->getViewMatrix()[0][0]);
				for (iterEffect; iterEffect != pEffectList->end(); ++iterEffect) {
					glPushMatrix();
					{
						if ((*iterEffect)->getDrawAfterWidget() == false) { // ���� �׸��� ������ �׷����� �ɼ� Ȱ��ȭ ��
							(*iterEffect)->draw();
						}
						else { // ���� �׸��� ������ �׷����� �ɼ� �� Ȱ��ȭ �� ������ ����Ʈ�� ��� ������ �׸� �� �׸��� ����
							tempEffectList.push_back(*iterEffect);
						}
					}
					glPopMatrix();
				}
			}
			glPopMatrix();

			// 4-3. ����Ʈ�� ó������ ������ �ݺ��ڸ� ���Ͽ� draw() ���� �Լ��� ȣ�� (3D object)
			list<GameWidget*>::iterator iter3D;
			for (iter3D = pDrawList->begin(); iter3D != pDrawList->end(); ++iter3D) {
				if ((*iter3D)->getDimension() == false) { // 3D ������ ��� �׸��� �۾� ����
						glPushMatrix();
						{
							if ((*iter3D)->getVisible() == true) (*iter3D)->draw();
						}
						glPopMatrix();
					}
					else { // 2D ������ ��� ������ ����Ʈ�� ��Ƽ� ���Ŀ� �׸���
						temp2DWigetList.push_back(*iter3D);
				}
 			}

#ifdef __DEBUG__ 
			// 4-4. �浹 ���� ������Ʈ���� �׷��� (�׽�Ʈ��)
			for (list<CollisionObject*>::iterator iterCol = g_colList.begin(); iterCol != g_colList.end(); ++iterCol) {
				glPushMatrix();
				{
					(*iterCol)->draw();
				}
				glPopMatrix();
			}
#endif
			// 4-5. ���� �׸� ���Ŀ� �׸� ����Ʈ�� �׸�
			if (tempEffectList.size() > 0) {
				glPushMatrix();
				{
					// ��Ʈ���� �ʱ�ȭ �� ī�޶� ����
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					glLoadMatrixf(&pCamera->getProjectionMatrix()[0][0]);

					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glLoadMatrixf(&pCamera->getViewMatrix()[0][0]);
					for (iterEffect = tempEffectList.begin(); iterEffect != tempEffectList.end(); ++iterEffect) {
						glPushMatrix();
						{
								(*iterEffect)->draw();
						}
						glPopMatrix();
					}
				}
				glPopMatrix();
			}
			tempEffectList.clear();
		}
		glPopMatrix();

		// 5. 2D���� �׸��� (���� ����)
		glPushMatrix();
		{
			// 5-1. ���� �Ȱ� ��� ���� �� ��� �ʱ�ȭ
			glDisable(GL_LIGHTING);
			glDisable(GL_FOG);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, pResolution->getWidth(), 0.0, pResolution->getHeight(), 0.0, 1.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// 5-2. ����Ʈ�� ó������ ������ �ݺ��ڸ� ���Ͽ� drawWidget() ���� �Լ��� ȣ�� (UI)
			list<GameWidget*>::iterator iter2D;
			for (iter2D = temp2DWigetList.begin(); iter2D != temp2DWigetList.end(); ++iter2D) {
				glPushMatrix();
				{
					glDisable(GL_DEPTH_TEST);
					if ((*iter2D)->getVisible() == true) (*iter2D)->draw();
				}
				glPopMatrix();
			}
			temp2DWigetList.clear();
			glEnable(GL_DEPTH_TEST);

			// 5-3. �� �ٽ� ��� (���� ���� �߰��� �� Ȱ��ȭ ����)
			//glEnable(GL_LIGHTING);
		}
		glPopMatrix();

		// 6. ���۷� �÷���
		glFlush();
	}
	// 7. ���� �������� ���ų� �ϴ� ������ pDrawList�� NULL�� ���, ī�޶� ���� ��� false��ȯ
	else {
		return false;
	}

	// 8. �׸��� �۾��� ���������� ��ġ�� true ��ȯ
	return true;
}

bool DrawManager::swap() {
	return SwapBuffers(*pHDC);
}
