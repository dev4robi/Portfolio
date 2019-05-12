#include "Game/Object/Skybox.h"
#include "Engine/ResourceManager.h"

Skybox::Skybox() {
	// �ʱ�ȭ
	useSkydomeMode = true; // �⺻�����δ� ��ī�̵��� �׸�

	// �ؽ�ó ��������
	// (ž, ��, ��, ��, ��, �� ������ ������)
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
	// �׸��� ���� �˻�
	if (isVisible == false) return false;

	// �׸��� �ʱ�ȭ
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glTranslatef(0.0, -0.2, 0.0);

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); // �ؽ�ó�� �ٸ� ������ ������ ���� ����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 1. Sky Dome Mode
	if (useSkydomeMode == true) {
		// �ݱ� ����
		REAL rRad = 1.0f;		// �ݱ��� ������
		int iCols = 20;			// �������� ���� (�浵 longitude)
		int iRows = 10;			// �������� ���� (���� latitude)

		// ���μ������� ������ ���� ���� ������
		REAL dCols = mymath::deg2rad(360.0 / iCols);
		REAL dRows = mymath::deg2rad(90.0 / iRows);

		// QUADS �� ������ �ݱ��� �ϴ� XYZ, ��� XYZ ��ǥ
		REAL botX, botXP, botY, botZ, botZP;
		REAL topX, topXP, topY, topZ, topZP;

		// �ߺ� ���� ������ ���� �ӽ� ���� ����
		REAL dCols_col, dCols_colPlus, dRows_row, dRows_rowPlus;
		REAL colCosX, colCosXP, colSinZ, colSinZP, rowCos, rowCosP;

		// �ݱ� �׸��� �۾� ����
		glBindTexture(GL_TEXTURE_2D, skyTextureID[6]); // ��ī�̵� �ؽ�ó ���ε�
		for (int row = 0; row < iRows; ++row) { // �ݱ� rows
			// �ߺ� ���� �ּ�ȭ
			dRows_row = (dRows * row);
			dRows_rowPlus = (dRows * (row + 1));

			// QUADS ���ϴ� ����
			botY = sinf(dRows_row) * rRad;		// �ݱ� row �� ���� Y��ǥ
			topY = sinf(dRows_rowPlus) * rRad;	// �ݱ� row + 1 �� ���� Y��ǥ

			for (int col = 0; col < iCols; ++col) { // �ݱ� cols
				// �ߺ� ���� �ּ�ȭ
				dCols_col = (dCols * col);
				dCols_colPlus = (dCols * (col + 1));
				colCosX = cosf(dCols_col);
				colCosXP = cosf(dCols_colPlus);
				colSinZ = sinf(dCols_col);
				colSinZP = sinf(dCols_colPlus);
				rowCos = cosf(dRows_row);
				rowCosP = cosf(dRows_rowPlus);

				// QUADS �ϴܺ� XZ
				botX = colCosX * rowCos * rRad;			// �ϴ� X
				botXP = colCosXP * rowCos * rRad;		// �ϴ� X PLUS
				botZ = colSinZ * rowCos * rRad;			// �ϴ� Z
				botZP = colSinZP * rowCos * rRad;		// �ϴ� Z PLUS

				// QUADS ��ܺ� XZ
				topX = colCosX * rowCosP * rRad;		// ��� X
				topXP = colCosXP * rowCosP * rRad;		// ��� X PLUS
				topZ = colSinZ * rowCosP * rRad;		// ��� Z
				topZP = colSinZP * rowCosP * rRad;		// ��� Z PLUS

				// QUADS ������ �׸���
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
					glTexCoord2f(bx, bz);	glVertex3f(botX, botY, botZ);	// ����
					glTexCoord2f(bxp, bzp);	glVertex3f(botXP, botY, botZP);	// ����
					glTexCoord2f(txp, tzp);	glVertex3f(topXP, topY, topZP);	// ���
					glTexCoord2f(tx, tz);	glVertex3f(topX, topY, topZ);	// �»�

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
		// ž
		glBindTexture(GL_TEXTURE_2D, skyTextureID[0]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(-0.5, 0.5, 0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(-0.5, 0.5, -0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(0.5, 0.5, -0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(0.5, 0.5, 0.5);
		}
		glEnd();

		// ��
		glBindTexture(GL_TEXTURE_2D, skyTextureID[1]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(-0.5, -0.5, -0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(-0.5, -0.5, 0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(0.5, -0.5, 0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(0.5, -0.5, -0.5);
		}
		glEnd();

		// ��
		glBindTexture(GL_TEXTURE_2D, skyTextureID[2]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(-0.5, 0.5, 0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(-0.5, -0.5, 0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(-0.5, -0.5, -0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(-0.5, 0.5, -0.5);
		}
		glEnd();

		// ��
		glBindTexture(GL_TEXTURE_2D, skyTextureID[3]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(-0.5, 0.5, -0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(-0.5, -0.5, -0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(0.5, -0.5, -0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(0.5, 0.5, -0.5);
		}
		glEnd();

		// ��
		glBindTexture(GL_TEXTURE_2D, skyTextureID[4]);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.1, 0.9);	glVertex3f(0.5, 0.5, -0.5);
			glTexCoord2f(0.1, 0.1);	glVertex3f(0.5, -0.5, -0.5);
			glTexCoord2f(0.9, 0.1);	glVertex3f(0.5, -0.5, 0.5);
			glTexCoord2f(0.9, 0.9);	glVertex3f(0.5, 0.5, 0.5);
		}
		glEnd();

		// ��
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
