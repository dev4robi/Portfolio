#include "Game/Object/Terrain.h"
#include "Engine/ResourceManager.h"
#include "opencv2/opencv.hpp"

Terrain::Terrain(REAL _scale, char *terrainTextureName, char *heightMapName, REAL _heightMin, REAL _heightMax, int _precision) : GameWidget(Vector3(0.0, 0.0, 0.0)) {
	// �ð� �ʱ�ȭ
	rElapsedTime = 0.00;

	// �� ���� �ʱ�ȭ
	pHeightData = NULL;

	// ���� �ؽ�ó �ε�
	textureID = g_pGameManager->getResourceManagerPtr()->getTextureID(terrainTextureName);
	
	// �����ϸ� ����
	scale = _scale;

	// ���� �ְ� ���� ����
	heightMin = _heightMin;
	heightMax = _heightMax;

	// �Ǽ�(����) ���� ����
	stallHeight = 7500.0;

	// ���е� ����
	precision = _precision;

	// pHeightData ������ ä�� ���� (cols, rows, scaledSize, pHeightData�� ���⼭ ä������)
	compileHeightData(heightMapName);

	// �Ȱ� ���� �ʱ�ȭ
	terrainFog.fogUsing = true;
	terrainFog.fogDensity = 0.25;
	terrainFog.fogFxStart = scale * 200.0;
	terrainFog.fogFxEnd = terrainFog.fogFxStart * 3.0;
	terrainFog.fogColor4[0] = 0.596;
	terrainFog.fogColor4[1] = 0.663;
	terrainFog.fogColor4[2] = 0.733;
	terrainFog.fogColor4[3] = 1.00;
	terrainFog.fogColorIndex = 0.0;
	terrainFog.fogMode = GL_LINEAR;
	terrainFog.fogHint = GL_NICEST;
	// OpenGL Ŭ���� ������ �Ȱ� �������� ����
	glClearColor(terrainFog.fogColor4[0], terrainFog.fogColor4[1], terrainFog.fogColor4[2], terrainFog.fogColor4[3]);

	// �ٴ� ��� ���� �� �ؽ�ó ��ȣ ����
	bUseSea = true;
	texture_seaTextureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/map/seaTexture.jpg");
}

Terrain::~Terrain() {
	// ���� ���� �Ҵ� ����
	if (pHeightData != NULL) {
		delete[]pHeightData;
	}
	pHeightData = NULL;
}

void Terrain::setPrecision(int _precision) {
	precision = _precision;
}

int Terrain::getPrecision() {
	return precision;
}

SIZE Terrain::getScaledSize() {
	return scaledSize;
}

REAL Terrain::getHeightAtPos(Vector3 xzPos) {
	REAL x = xzPos.x;
	REAL z = xzPos.z;

	SIZE halfSize = { scaledSize.cx / 2.0, scaledSize.cy / 2.0 };
	if (x > halfSize.cx || x < -halfSize.cx || z > halfSize.cy || z < -halfSize.cy) {
		return 0.0; // �� ������ ��� ���� ���� ������ 0.0���� ��ȯ
	}

	REAL sectorX = (((x + halfSize.cx) / precision) / scale), sectorZ = (((z + halfSize.cy) / precision) / scale);
	//printf("sectorX: %.2f, sectorZ: %.2f\n", sectorX, sectorZ);

	// compute the height field coordinates (hflCol0, hflRow0) and
	// (hflCol1, hflRow1) that identify the height field cell directly below the camera.
	int hflCol0 = sectorX;
	int hflRow0 = sectorZ;
	int hflCol1 = hflCol0 + 1;
	int hflRow1 = hflRow0 + 1;

	// get the four corner heights of the cell from the height field/	float h00 = pHeightData[hflCol0 + hflRow0 * cols];
	float h00 = pHeightData[hflCol0 + hflRow0 * cols];
	float h01 = pHeightData[hflCol1 + hflRow0 * cols];
	float h11 = pHeightData[hflCol1 + hflRow1 * cols];
	float h10 = pHeightData[hflCol0 + hflRow1 * cols];

	// calculate the position of the camera relative to the cell.
	// note, that 0 <= tx, ty <= 1.
	float tx = sectorX - float(hflCol0);
	float ty = sectorZ - float(hflRow0);

	// the next step is to perform a bilinear interpolation to compute the height
	// of the terrain directly below the object.
	float txty = tx * ty;

	return h00 * (1.0f - ty - tx + txty)
		+ h01 * (tx - txty)
		+ h11 * txty
		+ h10 * (ty - txty);
}

REAL Terrain::getStallHeight() {
	return stallHeight;
}

bool Terrain::compileHeightData(char *heightMapName) {
	// �� �����Ͱ� �����ϴ��� Ȯ���Ͽ� ���� �� ����
	if (pHeightData != NULL) {
		delete[]pHeightData;
		pHeightData = NULL;
	}

	// �̹��� �ε�
	GLubyte *pImgData = NULL;						// �̹��� ������ ������
	cv::Mat heightImg = cv::imread(heightMapName);	// �̹��� �����͸� ���� ��Ʈ����
	pImgData = heightImg.ptr();						// �̹����� ���� ������ ����Ű�� ��

	// �̹��� ���� Ȯ��
	if (pImgData == NULL) {
		MessageBox(NULL, TEXT("Terrain::Terrain() error."), TEXT("Fail to load heightmap."), (MB_OK | MB_ICONINFORMATION));
		throw exception("Fail to load Terrain heightmap file.");
		return false;
	}

	// ���е��� ���� �� ������ �� ���� �� �� ũ�� ����
	int imgWidth = heightImg.size().width;
	int imgHeight = heightImg.size().height;

	if (imgWidth % precision != 0 || imgHeight % precision != 0) { // �̹��� ũ�Ⱑ ���е��� ������ �������� ���� ��� ���� ���е��� ����
		precision = 1;
	}
	cols = (imgWidth / precision);
	rows = (imgHeight / precision);
	scaledSize.cx = imgWidth * scale;
	scaledSize.cy = imgHeight * scale;

	// �̹����κ��� ���� �����͸� �����ϰ�, ���� �� �����͸� �����Ͽ� ����
	int i = 0;
	const int jumpBits = (3 * precision); // ���� �����ʹ� BGR�� �̷�����ٴ� ���� �Ͽ� ����
	pHeightData = new REAL[(cols + 1) * (rows + 1)];
	for (int row = 0; row <= rows; ++row) {
		for (int col = 0; col <= cols; ++col) {
			pHeightData[row * cols + col] = getHeightFromFunc(pImgData[i]);	// �� ������ ���� �̹����� BLUE���� �����Ͽ�
			if (col != cols) {
				if (col != (cols - 1)) { // ������ ���� �ƴ� ����� ���� ��ǥ	// ���� �����͸� ���ϴµ� ��� (B G R ������ �����Ͱ� ����Ǿ� �ִ�)
					i += (jumpBits);
				}
				else { // ������ ���� ����� ���� ��ǥ
					i += ((precision - 1) * 3);
				}
			}
			else {
				i += 3;
			}
		}

		if (row == (rows - 1)) {  // ���� ������ ���� ��� ����ó�� (������ �� -1 ���� �ȼ��� ������ ó��)
			i += ((precision - 2) * 3 * imgWidth);
		}
		else { // �� �ܿ� ���Ŀ� �°� �� �̵�
			i += ((precision - 1) * 3 * imgWidth);
		}
	}

	pImgData = NULL;		// �̹��� �����͸� ����Ű�� �����͸� NULL�� �ʱ�ȭ
	heightImg.release();	// �̹����� ����

	return true; // ���� �� true ��ȯ
}

Vector3 Terrain::findRandomSafePos() {
	// ���� ������ ������
	Terrain *pMap = this;
	// ��Ȱ ���� ( �� ũ���� 80% �αٿ��� �������� �������� ���� )
	REAL ptX = (pMap->getScaledSize().cx / 2.0) * 0.8, ptZ = (pMap->getScaledSize().cy / 2.0) * 0.8;
	Vector3 respawnPos(cosf(mymath::deg2rad(mymath::randomInt(0, 360))) * ptX, 0.0, sinf(mymath::deg2rad(mymath::randomInt(0, 360))) * ptZ);
	// ��Ȱ ���̴� �ش� ���� ������ �ְ� ���̸� ������ �ΰ� �������� ����
	REAL minHeight = pMap->getHeightAtPos(respawnPos);
	respawnPos.y = minHeight + mymath::randomInt(750, pMap->getStallHeight() * 0.25);

	return respawnPos;
}

bool Terrain::draw() {
	// �׸��� ���� �˻�
	if (isVisible != true || isHiding == true) return false;

	// �Ȱ� ����
	if (terrainFog.fogUsing == true) {
		glFogi(GL_FOG_MODE, terrainFog.fogMode);
		glFogfv(GL_FOG_COLOR, terrainFog.fogColor4);
		glFogf(GL_FOG_DENSITY, terrainFog.fogDensity);
		glHint(GL_FOG_HINT, terrainFog.fogHint);
		glFogf(GL_FOG_START, terrainFog.fogFxStart);
		glFogf(GL_FOG_END, terrainFog.fogFxEnd);
		glEnable(GL_FOG);
	}
	else {
		glDisable(GL_FOG);
	}

	// ���� ��ǥ �̵�
	glTranslatef(position.x, position.y, position.z);

	// ���� ����
	glColor4f(1.0, 1.0, 1.0, 1.0);

	/* �ٴ� �׸��� */
	glDisable(GL_DEPTH_TEST);
	REAL dx = scaledSize.cx * 2 / 8.0, dy = scaledSize.cy * 2 / 8.0;		// �ٴ� �ؽ�ó ���� ������ �Ÿ�
	REAL tx = (rElapsedTime > 500.0 ? rElapsedTime - 500.0 : rElapsedTime); // �ؽ�ó ������ ��ǥ
	tx *= 0.002;
	glPushMatrix();
	{
		glTranslatef(scaledSize.cx, 2.0, -scaledSize.cy);	// �ؽ�ó �»�� ��ġ�� ������ �»������ �̵�
		for (int i = 0; i < 8; ++i) {
			glPushMatrix();
			{
				for (int j = 0; j < 8; ++j) {
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, texture_seaTextureID);
					glBegin(GL_QUADS);
					{
						glTexCoord2f(tx, tx + 1.0);	glVertex3f(0.0, 0.0, 0.0);
						glTexCoord2f(tx, tx);	glVertex3f(-dx, 0.0, 0.0);
						glTexCoord2f(tx + 1.0, tx);	glVertex3f(-dx, 0.0, dy);
						glTexCoord2f(tx + 1.0, tx + 1.0);	glVertex3f(0.0, 0.0, dy);
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);

					glTranslatef(0.0, 0.0, dy);
				}
			}
			glPopMatrix();
			glTranslatef(-dx, 0.0, 0.0);
		}
	}
	glPopMatrix();

	/* ���� �׸���*/
	glEnable(GL_DEPTH_TEST);

	// ���� ���� �����ϸ�
	glScalef(scale, 1.0, scale);

	// �ǹ��� ���� �߰��� �α� ���� ����
	REAL halfPre = (precision / 2.0);
	glTranslatef(-(cols * halfPre), 0.0, -(rows * halfPre));

	// �ؽ�ó Ȥ�� ���̾������� ����
	if (textureID != 0) { // �ؽ�ó ����
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	else { // ���̾������� ����
		glDisable(GL_TEXTURE_2D);
		glColor4f(0.0, 0.5, 0.25, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// �� ������ �׸��� ����
	// TRIANGLE_STRIP : �׸��� ���� ����
	//
	//									��[col]
	//	  1      3       5      7		    
	//	  2(9)   4(11)   6(13)  8(15) ...			�����
	//	 10(17) 12(19)  14(21) 16(23)				�֢֢֢�
	//	 18     20      22     24
	//	 ...
	//
	// ��[row]

	int pr = 0, pc = 0;					// pHeightData ���� ������ �������� ���� ����
	REAL r_p = 0.0, c_p = 0.0;			// ���� ����ȭ�� ���� ���� (r_p = row + precision / c_p = col + precision)
	REAL texU = 0.0, texV = 0.0;		// �ؽ�ó ��ǥ�� ���� ����
	REAL texUP = (1.0 / (cols)), texVP = (1.0 / (rows));
	
	for (int row = 0; row < (rows) * precision; row += precision) {
		r_p = row + precision;
		texV = (row / ((REAL)rows * precision));
		pc = 0;
		for (int col = 0; col < (cols) * precision; col += precision) {
			c_p = col + precision;
			texU = (col / ((REAL)cols * precision));
			glBegin(GL_TRIANGLE_STRIP);
			{	
				if (textureID == 0) { // ���̾������� ��� �׸��� (���� ����)
					REAL cr1 = (pHeightData[(pr)* cols + (pc)] / heightMax), cr2 = (pHeightData[(pr + 1) * cols + (pc)] / heightMax);
					REAL cr3 = (pHeightData[(pr)* cols + (pc + 1)] / heightMax), cr4 = (pHeightData[(pr + 1) * cols + (pc + 1)] / heightMax);
					glColor4f(1.25 * cr1, 0.5 / (cr1 + 0.01), 0.15, 1.0); glTexCoord2f(texU, texV);					glVertex3f(col, pHeightData[(pr)* cols + (pc)], row);	glNormal3f(0.0, 1.0, 0.0);
					glColor4f(1.25 * cr2, 0.5 / (cr2 + 0.01), 0.15, 1.0); glTexCoord2f(texU, texV + texVP); 			glVertex3f(col, pHeightData[(pr + 1) * cols + (pc)], r_p);	glNormal3f(0.0, 1.0, 0.0);
					glColor4f(1.25 * cr3, 0.5 / (cr3 + 0.01), 0.15, 1.0); glTexCoord2f(texU + texUP, texV);			glVertex3f(c_p, pHeightData[(pr)* cols + (pc + 1)], row);	glNormal3f(0.0, 1.0, 0.0);
					glColor4f(1.25 * cr4, 0.5 / (cr4 + 0.01), 0.15, 1.0); glTexCoord2f(texU + texUP, texV + texVP);	glVertex3f(c_p, pHeightData[(pr + 1) * cols + (pc + 1)], r_p);	glNormal3f(0.0, 1.0, 0.0);
				}
				else { // �ؽ�ó ��� �׸���
					glTexCoord2f(texU, texV);					glVertex3f(col, pHeightData[(pr)* cols + (pc)], row);	glNormal3f(0.0, 1.0, 0.0);
					glTexCoord2f(texU, texV + texVP); 			glVertex3f(col, pHeightData[(pr + 1) * cols + (pc)], r_p);	glNormal3f(0.0, 1.0, 0.0);
					glTexCoord2f(texU + texUP, texV);			glVertex3f(c_p, pHeightData[(pr)* cols + (pc + 1)], row);	glNormal3f(0.0, 1.0, 0.0);
					glTexCoord2f(texU + texUP, texV + texVP);	glVertex3f(c_p, pHeightData[(pr + 1) * cols + (pc + 1)], r_p);	glNormal3f(0.0, 1.0, 0.0);
				}
			}
			glEnd();
			++pc;
		}
		++pr;
	}

	// �ʱ�ȭ
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_TEXTURE_2D);

	// ���� ��ȯ
	return true;
}

bool Terrain::destroy() {
	return true;
}

bool Terrain::update(REAL timeElapsed) {
	// ��� �ð� ������Ʈ
	rElapsedTime += timeElapsed;

	return true;
}

REAL Terrain::getHeightFromFunc(int val) {
	// ���� ������
	return (val * ((abs(heightMax) + abs(heightMin)) / 255.0)) + heightMin;
}
