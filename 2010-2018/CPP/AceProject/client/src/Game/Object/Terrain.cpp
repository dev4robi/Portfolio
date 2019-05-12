#include "Game/Object/Terrain.h"
#include "Engine/ResourceManager.h"
#include "opencv2/opencv.hpp"

Terrain::Terrain(REAL _scale, char *terrainTextureName, char *heightMapName, REAL _heightMin, REAL _heightMax, int _precision) : GameWidget(Vector3(0.0, 0.0, 0.0)) {
	// 시간 초기화
	rElapsedTime = 0.00;

	// 고도 정보 초기화
	pHeightData = NULL;

	// 지형 텍스처 로드
	textureID = g_pGameManager->getResourceManagerPtr()->getTextureID(terrainTextureName);
	
	// 스케일링 설정
	scale = _scale;

	// 최저 최고 높이 설정
	heightMin = _heightMin;
	heightMax = _heightMax;

	// 실속(스톨) 높이 설정
	stallHeight = 7500.0;

	// 정밀도 설정
	precision = _precision;

	// pHeightData 정보를 채워 넣음 (cols, rows, scaledSize, pHeightData가 여기서 채워진다)
	compileHeightData(heightMapName);

	// 안개 정보 초기화
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
	// OpenGL 클리어 색상을 안개 색상으로 설정
	glClearColor(terrainFog.fogColor4[0], terrainFog.fogColor4[1], terrainFog.fogColor4[2], terrainFog.fogColor4[3]);

	// 바다 사용 여부 및 텍스처 번호 설정
	bUseSea = true;
	texture_seaTextureID = g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/map/seaTexture.jpg");
}

Terrain::~Terrain() {
	// 지형 정보 할당 해제
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
		return 0.0; // 맵 범위를 벗어난 곳의 높이 정보는 0.0으로 반환
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
	// 고도 데이터가 존재하는지 확인하여 존재 시 삭제
	if (pHeightData != NULL) {
		delete[]pHeightData;
		pHeightData = NULL;
	}

	// 이미지 로드
	GLubyte *pImgData = NULL;						// 이미지 데이터 포인터
	cv::Mat heightImg = cv::imread(heightMapName);	// 이미지 데이터를 담은 매트릭스
	pImgData = heightImg.ptr();						// 이미지의 시작 지점을 가리키게 함

	// 이미지 오류 확인
	if (pImgData == NULL) {
		MessageBox(NULL, TEXT("Terrain::Terrain() error."), TEXT("Fail to load heightmap."), (MB_OK | MB_ICONINFORMATION));
		throw exception("Fail to load Terrain heightmap file.");
		return false;
	}

	// 정밀도에 의한 행 개수와 열 개수 및 맵 크기 저장
	int imgWidth = heightImg.size().width;
	int imgHeight = heightImg.size().height;

	if (imgWidth % precision != 0 || imgHeight % precision != 0) { // 이미지 크기가 정밀도로 나누어 떨어지지 않을 경우 원본 정밀도로 변경
		precision = 1;
	}
	cols = (imgWidth / precision);
	rows = (imgHeight / precision);
	scaledSize.cx = imgWidth * scale;
	scaledSize.cy = imgHeight * scale;

	// 이미지로부터 높이 데이터를 추출하고, 계산된 고도 데이터를 연산하여 저장
	int i = 0;
	const int jumpBits = (3 * precision); // 고도맵 데이터는 BGR로 이루어진다는 가정 하에 성립
	pHeightData = new REAL[(cols + 1) * (rows + 1)];
	for (int row = 0; row <= rows; ++row) {
		for (int col = 0; col <= cols; ++col) {
			pHeightData[row * cols + col] = getHeightFromFunc(pImgData[i]);	// 고도 정보를 담은 이미지의 BLUE값만 추출하여
			if (col != cols) {
				if (col != (cols - 1)) { // 마지막 열이 아닌 경우의 다음 좌표	// 높이 데이터를 구하는데 사용 (B G R 순서로 데이터가 저장되어 있다)
					i += (jumpBits);
				}
				else { // 마지막 열인 경우의 다음 좌표
					i += ((precision - 1) * 3);
				}
			}
			else {
				i += 3;
			}
		}

		if (row == (rows - 1)) {  // 가장 마지막 행일 경우 예외처리 (마지막 행 -1 행의 픽셀의 값으로 처리)
			i += ((precision - 2) * 3 * imgWidth);
		}
		else { // 그 외엔 공식에 맞게 행 이동
			i += ((precision - 1) * 3 * imgWidth);
		}
	}

	pImgData = NULL;		// 이미지 데이터를 가르키던 포인터를 NULL로 초기화
	heightImg.release();	// 이미지를 닫음

	return true; // 도달 시 true 반환
}

Vector3 Terrain::findRandomSafePos() {
	// 지형 정보를 가져옴
	Terrain *pMap = this;
	// 부활 지점 ( 맵 크기의 80% 부근에서 원형으로 무작위로 지정 )
	REAL ptX = (pMap->getScaledSize().cx / 2.0) * 0.8, ptZ = (pMap->getScaledSize().cy / 2.0) * 0.8;
	Vector3 respawnPos(cosf(mymath::deg2rad(mymath::randomInt(0, 360))) * ptX, 0.0, sinf(mymath::deg2rad(mymath::randomInt(0, 360))) * ptZ);
	// 부활 높이는 해당 지점 높이의 최고 높이를 최저로 두고 무작위로 지정
	REAL minHeight = pMap->getHeightAtPos(respawnPos);
	respawnPos.y = minHeight + mymath::randomInt(750, pMap->getStallHeight() * 0.25);

	return respawnPos;
}

bool Terrain::draw() {
	// 그리기 조건 검사
	if (isVisible != true || isHiding == true) return false;

	// 안개 적용
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

	// 월드 좌표 이동
	glTranslatef(position.x, position.y, position.z);

	// 색상 지정
	glColor4f(1.0, 1.0, 1.0, 1.0);

	/* 바다 그리기 */
	glDisable(GL_DEPTH_TEST);
	REAL dx = scaledSize.cx * 2 / 8.0, dy = scaledSize.cy * 2 / 8.0;		// 바다 텍스처 사이 사이의 거리
	REAL tx = (rElapsedTime > 500.0 ? rElapsedTime - 500.0 : rElapsedTime); // 텍스처 움직임 좌표
	tx *= 0.002;
	glPushMatrix();
	{
		glTranslatef(scaledSize.cx, 2.0, -scaledSize.cy);	// 텍스처 좌상단 위치를 지형의 좌상단으로 이동
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

	/* 지형 그리기*/
	glEnable(GL_DEPTH_TEST);

	// 지면 넓이 스케일링
	glScalef(scale, 1.0, scale);

	// 피벗을 지형 중간에 두기 위한 보정
	REAL halfPre = (precision / 2.0);
	glTranslatef(-(cols * halfPre), 0.0, -(rows * halfPre));

	// 텍스처 혹은 와이어프레임 적용
	if (textureID != 0) { // 텍스처 적용
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	else { // 와이어프레임 적용
		glDisable(GL_TEXTURE_2D);
		glColor4f(0.0, 0.5, 0.25, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// 맵 정보로 그리기 시작
	// TRIANGLE_STRIP : 그리기 순서 참고
	//
	//									→[col]
	//	  1      3       5      7		    
	//	  2(9)   4(11)   6(13)  8(15) ...			↓↓↓↓
	//	 10(17) 12(19)  14(21) 16(23)				↗↗↗↗
	//	 18     20      22     24
	//	 ...
	//
	// ↓[row]

	int pr = 0, pc = 0;					// pHeightData 에서 정보를 가져오기 위한 변수
	REAL r_p = 0.0, c_p = 0.0;			// 연산 최적화를 위한 변수 (r_p = row + precision / c_p = col + precision)
	REAL texU = 0.0, texV = 0.0;		// 텍스처 좌표를 위한 변수
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
				if (textureID == 0) { // 와이어프레임 사용 그리기 (고도별 색상)
					REAL cr1 = (pHeightData[(pr)* cols + (pc)] / heightMax), cr2 = (pHeightData[(pr + 1) * cols + (pc)] / heightMax);
					REAL cr3 = (pHeightData[(pr)* cols + (pc + 1)] / heightMax), cr4 = (pHeightData[(pr + 1) * cols + (pc + 1)] / heightMax);
					glColor4f(1.25 * cr1, 0.5 / (cr1 + 0.01), 0.15, 1.0); glTexCoord2f(texU, texV);					glVertex3f(col, pHeightData[(pr)* cols + (pc)], row);	glNormal3f(0.0, 1.0, 0.0);
					glColor4f(1.25 * cr2, 0.5 / (cr2 + 0.01), 0.15, 1.0); glTexCoord2f(texU, texV + texVP); 			glVertex3f(col, pHeightData[(pr + 1) * cols + (pc)], r_p);	glNormal3f(0.0, 1.0, 0.0);
					glColor4f(1.25 * cr3, 0.5 / (cr3 + 0.01), 0.15, 1.0); glTexCoord2f(texU + texUP, texV);			glVertex3f(c_p, pHeightData[(pr)* cols + (pc + 1)], row);	glNormal3f(0.0, 1.0, 0.0);
					glColor4f(1.25 * cr4, 0.5 / (cr4 + 0.01), 0.15, 1.0); glTexCoord2f(texU + texUP, texV + texVP);	glVertex3f(c_p, pHeightData[(pr + 1) * cols + (pc + 1)], r_p);	glNormal3f(0.0, 1.0, 0.0);
				}
				else { // 텍스처 사용 그리기
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

	// 초기화
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_TEXTURE_2D);

	// 성공 반환
	return true;
}

bool Terrain::destroy() {
	return true;
}

bool Terrain::update(REAL timeElapsed) {
	// 경과 시간 업데이트
	rElapsedTime += timeElapsed;

	return true;
}

REAL Terrain::getHeightFromFunc(int val) {
	// 선형 방정식
	return (val * ((abs(heightMax) + abs(heightMin)) / 255.0)) + heightMin;
}
