#ifndef __TERRAIN_H__
#define __TERRAIN_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : Terrain(v2)
*	- Last Update : 2016.08.01
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���� ���õ� Ŭ����.
*
*	- Dependency : [StdHeader.h, GameWidget.h], ResourceManager.h, opencv.hpp
*
*   - Version(v1): �ͷ��� Ŭ���� ���� ���� (2016.06.11)
*	- Version(v2): ���� �缳�� (2016.08.01)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Engine/GameWidget.h"
#include "Library/MyOpenGL.h"

class Terrain : public GameWidget {
private:
	REAL rElapsedTime;				// ���� ���� �� ����ð�
	int precision;					// ���� ���е� (1 = �̹����� 1:1���е�, 2 = �̹����� 0.5:0.5 ���е� - ���е��� �̹����� ũ��� ����������� �Ѵ�)
	int cols, rows;					// ���е� ���� ���� ���� ���� ����� ����
	REAL scale;						// ���� ������ ���� ũ�� (Y���� ������ ���� ����)
	SIZE scaledSize;				// �����ϸ� ���� ���� ���� ũ��
	REAL *pHeightData;				// ���� �� ������
	REAL heightMin, heightMax;		// ���� ����, �ִ� ����
	REAL stallHeight;				// �Ǽ�(����) ���� ����
	GL_Fog terrainFog;				// ���� �Ȱ�
	
	bool bUseSea;					// �ٴ� ��� ����
	GLuint texture_seaTextureID;	// �ٴ� �ؽ�ó ��ȣ

public:
	// ������ & �Ҹ���
	Terrain(REAL _scale, char *terrainTextureName, char *heightMapName,
			REAL _heightMin, REAL _heightMax, int _precision);
	~Terrain();
	
	// ���� & ����
	void setPrecision(int _precision);	// ���е� ���� (pHeightData �����͸� �缳���ϱ� ������ ������ �߻� ���ɿ� ����)
	int getPrecision();					// ���е���ġ ��ȯ
	SIZE getScaledSize();				// ������ �����ϸ��� ����� ��ȯ
	REAL getHeightAtPos(Vector3 xzPos);	// X, Z ��ǥ������ ���� ���̸� ��ȯ
	REAL getStallHeight();				// �Ǽ� ���̸� ��ȯ

	// �޼ҵ�
	Vector3 findRandomSafePos();		// �������� �浹�� ������ ������ ��ǥ�� ��ȯ

	// ���� �Լ�
	virtual bool draw();					// �׸���
	virtual bool destroy();					// �ı�
	virtual bool update(REAL timeElapsed);	// ������Ʈ

private:
	REAL getHeightFromFunc(int val);				// �ش� �̹��� ���� ������(0~255)�� �Է¹޾� ���� ���� ���� ��ȯ
	bool compileHeightData(char *heightMapName);	// �ɹ� �������� �̿��Ͽ� �׷��� ���� ������ pHeightData�� ����
};

#endif