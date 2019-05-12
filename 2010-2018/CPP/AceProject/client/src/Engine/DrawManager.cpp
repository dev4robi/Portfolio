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
	// 1. 메인 윈도우의 HDC와 HGLRC를 포인터로 가리킨다
	if (hdc != NULL && hglrc != NULL) {
		pHDC = hdc;
		pHGLRC = hglrc;
	}
	else {
		MessageBox(NULL, TEXT("DC null pointer error."), TEXT("DrawManager::DrawManager() error"), (MB_OK | MB_ICONINFORMATION));
		exit(1);
	}

	// 2. 해상도를 설정한다
	pResolution = resolution;

	// 3. OpenGL을 초기화한다
	if (initGL() == true) {

	}
	else {
		MessageBox(NULL, TEXT("OpenGL initialization error."), TEXT("DrawManager::DrawManager() error"), (MB_OK | MB_ICONINFORMATION));
		exit(1);
	}
}

DrawManager::~DrawManager() {
	// 카메라 객체를 파괴
	if (pCamera != NULL) {
		delete pCamera;
		pCamera = NULL;
	}

	// 그 외 참조하는 포인터를 널로 변경
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
	glShadeModel(GL_SMOOTH);							// 부드러운 쉐이딩 사용
	glClearColor(0.0, 0.0, 0.0, 0.0);					// 기본 배경은 검은색 배경
	glClearDepth(1.0);									// 깊이 버퍼 설정
	glEnable(GL_DEPTH_TEST);							// 깊이테스트를 사용
	glDepthFunc(GL_LEQUAL);								// 사용할 깊이 테스트의 종류
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 최고 성능의 원근 계산 사용

	return true;	// 초기화를 성공적으로 완료
}

bool DrawManager::draw() {
	// 1. 그려야 할 프레임의 위젯 리스트와 카메라를 찾음
	GameFrame *pFrame = (g_pGameManager->getGameFramePtr());
	pDrawList = (pFrame->getDrawListPtr());
	pEffectList = (pFrame->getEffectListPtr());
	pCamera = getCameraFromFrame();

	// 2. 그리기 리스트 포인터가 있는 경우 (GameFrame의 widgetList를 참조하고 있는 경우)
	if (pCamera != NULL && pDrawList != NULL) {
		// 3-1. 2D리스트를 위한 리스트 생성 및 버퍼 초기화
		static list<GameWidget*> temp2DWigetList;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// 3-2. GL옵션
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_LINE_SMOOTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

		// 3-3. 뷰포트 초기화
		Size2D size(g_pGameManager->getGameWndPtr()->getWndSize());
		glViewport(0, 0, size.getWidth(), size.getHeight());
		
		// 4. 3D위젯과 이펙트 그리기
		glPushMatrix();
		{
			// 4-1. 매트릭스 초기화 및 카메라 적용
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glLoadMatrixf(&pCamera->getProjectionMatrix()[0][0]);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glLoadMatrixf(&pCamera->getViewMatrix()[0][0]);

			// 하늘 상자 그리기
			glPushMatrix();
			{
				Vector3 camPos = pCamera->getPosition();
				glTranslatef(camPos.x, camPos.y, camPos.z);
				if (pFrame->getSkyboxPtr() != NULL) pFrame->getSkyboxPtr()->draw();
			}
			glPopMatrix();

			// 지형 그리기
			glPushMatrix();
			{
				if (pFrame->getTerrainPtr() != NULL) pFrame->getTerrainPtr()->draw();
			}
			glPopMatrix();

			// 4-2. 이펙트 그리기
			static list<GL_Effect*> tempEffectList;
			list<GL_Effect*>::iterator iterEffect = pEffectList->begin();
			glPushMatrix();
			{
				// 매트릭스 초기화 및 카메라 적용
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glLoadMatrixf(&pCamera->getProjectionMatrix()[0][0]);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glLoadMatrixf(&pCamera->getViewMatrix()[0][0]);
				for (iterEffect; iterEffect != pEffectList->end(); ++iterEffect) {
					glPushMatrix();
					{
						if ((*iterEffect)->getDrawAfterWidget() == false) { // 위젯 그리기 이전에 그려지는 옵션 활성화 시
							(*iterEffect)->draw();
						}
						else { // 위젯 그리기 이전에 그려지는 옵션 비 활성화 시 별도에 리스트에 담아 위젯을 그린 뒤 그리기 수행
							tempEffectList.push_back(*iterEffect);
						}
					}
					glPopMatrix();
				}
			}
			glPopMatrix();

			// 4-3. 리스트의 처음부터 끝까지 반복자를 통하여 draw() 가상 함수를 호출 (3D object)
			list<GameWidget*>::iterator iter3D;
			for (iter3D = pDrawList->begin(); iter3D != pDrawList->end(); ++iter3D) {
				if ((*iter3D)->getDimension() == false) { // 3D 위젯인 경우 그리기 작업 수행
						glPushMatrix();
						{
							if ((*iter3D)->getVisible() == true) (*iter3D)->draw();
						}
						glPopMatrix();
					}
					else { // 2D 위젯인 경우 별도의 리스트에 담아서 추후에 그린다
						temp2DWigetList.push_back(*iter3D);
				}
 			}

#ifdef __DEBUG__ 
			// 4-4. 충돌 감지 오브젝트들을 그려줌 (테스트용)
			for (list<CollisionObject*>::iterator iterCol = g_colList.begin(); iterCol != g_colList.end(); ++iterCol) {
				glPushMatrix();
				{
					(*iterCol)->draw();
				}
				glPopMatrix();
			}
#endif
			// 4-5. 위젯 그린 이후에 그릴 이펙트를 그림
			if (tempEffectList.size() > 0) {
				glPushMatrix();
				{
					// 매트릭스 초기화 및 카메라 적용
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

		// 5. 2D위젯 그리기 (정사 투영)
		glPushMatrix();
		{
			// 5-1. 빛과 안개 사용 중지 및 행렬 초기화
			glDisable(GL_LIGHTING);
			glDisable(GL_FOG);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, pResolution->getWidth(), 0.0, pResolution->getHeight(), 0.0, 1.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			// 5-2. 리스트의 처음부터 끝까지 반복자를 통하여 drawWidget() 가상 함수를 호출 (UI)
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

			// 5-3. 빛 다시 사용 (추후 빛을 추가한 후 활성화 요함)
			//glEnable(GL_LIGHTING);
		}
		glPopMatrix();

		// 6. 버퍼로 플러시
		glFlush();
	}
	// 7. 게임 프레임이 없거나 하는 이유로 pDrawList가 NULL인 경우, 카메라가 없는 경우 false반환
	else {
		return false;
	}

	// 8. 그리기 작업을 성공적으로 마치고 true 반환
	return true;
}

bool DrawManager::swap() {
	return SwapBuffers(*pHDC);
}
