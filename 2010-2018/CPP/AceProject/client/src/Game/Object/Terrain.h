#ifndef __TERRAIN_H__
#define __TERRAIN_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : Terrain(v2)
*	- Last Update : 2016.08.01
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 지형 관련된 클래스.
*
*	- Dependency : [StdHeader.h, GameWidget.h], ResourceManager.h, opencv.hpp
*
*   - Version(v1): 터레인 클래스 기초 설계 (2016.06.11)
*	- Version(v2): 구조 재설계 (2016.08.01)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameWidget.h"
#include "Library/MyOpenGL.h"

class Terrain : public GameWidget {
private:
	REAL rElapsedTime;				// 지형 생성 후 경과시간
	int precision;					// 지형 정밀도 (1 = 이미지와 1:1정밀도, 2 = 이미지와 0.5:0.5 정밀도 - 정밀도와 이미지의 크기는 나누어떨어져야 한다)
	int cols, rows;					// 정밀도 적용 후의 가로 세로 행렬의 개수
	REAL scale;						// 지형 스케일 적용 크기 (Y축은 스케일 하지 않음)
	SIZE scaledSize;				// 스케일링 적용 후의 실제 크기
	REAL *pHeightData;				// 지형 고도 데이터
	REAL heightMin, heightMax;		// 지형 최저, 최대 높이
	REAL stallHeight;				// 실속(스톨) 시작 높이
	GL_Fog terrainFog;				// 지형 안개
	
	bool bUseSea;					// 바다 사용 여부
	GLuint texture_seaTextureID;	// 바다 텍스처 번호

public:
	// 생성자 & 소멸자
	Terrain(REAL _scale, char *terrainTextureName, char *heightMapName,
			REAL _heightMin, REAL _heightMax, int _precision);
	~Terrain();
	
	// 게터 & 세터
	void setPrecision(int _precision);	// 정밀도 설정 (pHeightData 데이터를 재설정하기 때문에 과부하 발생 가능에 유의)
	int getPrecision();					// 정밀도수치 반환
	SIZE getScaledSize();				// 지형의 스케일링된 사이즈를 반환
	REAL getHeightAtPos(Vector3 xzPos);	// X, Z 좌표에서의 지형 높이를 반환
	REAL getStallHeight();				// 실속 높이를 반환

	// 메소드
	Vector3 findRandomSafePos();		// 지형에서 충돌에 안전한 무작위 좌표를 반환

	// 가상 함수
	virtual bool draw();					// 그리기
	virtual bool destroy();					// 파괴
	virtual bool update(REAL timeElapsed);	// 업데이트

private:
	REAL getHeightFromFunc(int val);				// 해당 이미지 높이 데이터(0~255)를 입력받아 계산된 높이 값을 반환
	bool compileHeightData(char *heightMapName);	// 맴버 변수들을 이용하여 그려질 높이 정보를 pHeightData에 저장
};

#endif