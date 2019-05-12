#include "Game/Object/Skybox.h"
#include "Engine/ResourceManager.h"

Skybox::Skybox() {
	// 초기화
	useSkydomeMode = true; // 기본적으로는 스카이돔을 그림

	// 텍스처 가져오기
	// (탑, 봇, 서, 북, 동, 남 순서로 가져옴)
	skyTextureID[0] = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/sky/SkyTop.png");
	skyTextureID[1] = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/sky/SkyBot.png");
	skyTextureID[2] = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/sky/SkyWest.png");
	skyTextureID[3] = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/sky/SkyNorth.png");
	skyTextureID[4] = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/sky/SkyEast.png");
	skyTextureID[5] = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/sky/SkySouth.png");
	skyTextureID[6] = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/sky/skydome.png");
}

Skybox::~Skybox() {

}

bool Skybox::draw() {
	// 그리기 여부 검사
	if (isVisible == false) return false;

	// 그리기 초기화
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslatef(0.0, -0.2, 0.0);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); // 텍스처가 다른 색상의 영향을 받지 않음
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 1. Sky Dome Mode
	if (useSkydomeMode == true) {
		// 반구 정보
		REAL rRad = 1.0f;		// 반구의 반지름
		int iCols = 20;			// 세로줄의 개수 (경도 longitude)
		int iRows = 10;			// 가로줄의 개수 (위도 latitude)

		// 가로세로줄의 개수에 따른 각도 증가분
		REAL dCols = mymath::deg2rad(360.0 / iCols);
		REAL dRows = mymath::deg2rad(90.0 / iRows);

		// QUADS 로 구성된 반구의 하단 XYZ, 상단 XYZ 좌표
		REAL botX, botXP, botY, botZ, botZP;
		REAL topX, topXP, topY, topZ, topZP;

		// 중복 연산 방지를 위한 임시 저장 변수
		REAL dCols_col, dCols_colPlus, dRows_row, dRows_rowPlus;
		REAL colCosX, colCosXP, colSinZ, colSinZP, rowCos, rowCosP;

		// 반구 그리기 작업 시작
		glBindTexture(GL_TEXTURE_2D, skyTextureID[6]); // 스카이돔 텍스처 바인딩
		for (int row = 0; row < iRows; ++row) { // 반구 rows
			// 중복 연산 최소화
			dRows_row = (dRows * row);
			dRows_rowPlus = (dRows * (row + 1));

			// QUADS 상하단 높이
			botY = sinf(dRows_row) * rRad;		// 반구 row 에 따른 Y좌표
			topY = sinf(dRows_rowPlus) * rRad;	// 반구 row + 1 에 따른 Y좌표

			for (int col = 0; col < iCols; ++col) { // 반구 cols
				// 중복 연산 최소화
				dCols_col = (dCols * col);
				dCols_colPlus = (dCols * (col + 1));
				colCosX = cosf(dCols_col);
				colCosXP = cosf(dCols_colPlus);
				colSinZ = sinf(dCols_col);
				colSinZP = sinf(dCols_colPlus);
				rowCos = cosf(dRows_row);
				rowCosP = cosf(dRows_rowPlus);

				// QUADS 하단부 XZ
				botX = colCosX * rowCos * rRad;			// 하단 X
				botXP = colCosXP * rowCos * rRad;		// 하단 X PLUS
				botZ = colSinZ * rowCos * rRad;			// 하단 Z
				botZP = colSinZP * rowCos * rRad;		// 하단 Z PLUS

				// QUADS 상단부 XZ
				topX = colCosX * rowCosP * rRad;		// 상단 X
				topXP = colCosXP * rowCosP * rRad;		// 상단 X PLUS
				topZ = colSinZ * rowCosP * rRad;		// 상단 Z
				topZP = colSinZP * rowCosP * rRad;		// 상단 Z PLUS

				// QUADS 정보로 그리기
				glBegin(GL_QUADS);
				{
					REAL bx, bz, bxp, bzp, tx, tz, txp, tzp;
					bx = (botX + 1.0) / 2.0;
					bz = (botZ + 1.0) / 2.0;
					bxp = (botXP + 1.0) / 2.0;
					bzp = (botZP + 1.0) / 2.0;
					tx = (topX + 1.0) / 2.0;
					tz = (topZ + 1.0) / 2.0;
					txp = (topXP + 1.0) / 2.0;
					tzp = (topZP + 1.0) / 2.0;
					glTexCoord2f(bx, bz);	glVertex3f(botX, botY, botZ);	// 좌하
					glTexCoord2f(bxp, bzp);	glVertex3f(botXP, botY, botZP);	// 우하
					glTexCoord2f(txp, tzp);	glVertex3f(topXP, topY, topZP);	// 우상
					glTexCoord2f(tx, tz);	glVertex3f(topX, topY, topZ);	// 좌상

				//	printf("row: %d, col: %d\n", row, col);
				//	printf("botX: %.2f, botZ: %.2f, botXP: %.2f, botZP: %.2f\n", bx, bz, bxp, bzp);
				//	printf("topX: %.2f, topZ: %.2f, topXP: %.2f, topZP: %.2f\n\n", tx, tz, txp, tzp);
				}
				glEnd();
			}
		}
	}

	// 2. Skybox Mode
	else {
		// 탑
		glBindTexture(GL_TEXTURE_2D, skyTextureID[0]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(-0.5, 0.5, 0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(-0.5, 0.5, -0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(0.5, 0.5, -0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(0.5, 0.5, 0.5);
		}
		glEnd();

		// 봇
		glBindTexture(GL_TEXTURE_2D, skyTextureID[1]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(-0.5, -0.5, -0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(-0.5, -0.5, 0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(0.5, -0.5, 0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(0.5, -0.5, -0.5);
		}
		glEnd();

		// 서
		glBindTexture(GL_TEXTURE_2D, skyTextureID[2]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(-0.5, 0.5, 0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(-0.5, -0.5, 0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(-0.5, -0.5, -0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(-0.5, 0.5, -0.5);
		}
		glEnd();

		// 북
		glBindTexture(GL_TEXTURE_2D, skyTextureID[3]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(-0.5, 0.5, -0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(-0.5, -0.5, -0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(0.5, -0.5, -0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(0.5, 0.5, -0.5);
		}
		glEnd();

		// 동
		glBindTexture(GL_TEXTURE_2D, skyTextureID[4]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(0.5, 0.5, -0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(0.5, -0.5, -0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(0.5, -0.5, 0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(0.5, 0.5, 0.5);
		}
		glEnd();

		// 남
		glBindTexture(GL_TEXTURE_2D, skyTextureID[5]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(0.5, 0.5, 0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(0.5, -0.5, 0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(-0.5, -0.5, 0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(-0.5, 0.5, 0.5);
		}
		glEnd();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	return true;
}

bool Skybox::destroy() {
	return true;
}

bool Skybox::update(REAL timeElapsed) {
	return true;
}
