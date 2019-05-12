#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Engine/StdHeader.h"
#include "Engine/GameWidget.h"

class Skybox : public GameWidget {
private:
	bool useSkydomeMode;					// ��ī�̵� ��� ���� (true �� ��ī�� ����, false�� ��ī�� �ڽ��� �׸�)
	GLuint skyTextureID[7];					// ��ī�̹ڽ� �ؽ�ó ��ȣ (������� ž, ��, ��, ��, ��, ��, ��ī�̹ڽ� ����)

public:
	Skybox();								// ������
	~Skybox();								// �Ҹ���

	virtual bool draw();					// �׸���
	virtual bool destroy();					// �ı�
	virtual bool update(REAL timeElapsed);	// ������Ʈ
};

#endif