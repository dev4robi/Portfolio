#ifndef __MY_OPENGL_H__
#define __MY_OPENGL_H__

#include "Engine/StdHeader.h"

struct GL_Fog {
	GLboolean fogUsing;		// �Ȱ� ��� ����
	GLfloat fogDensity;		// �Ȱ� �е�
	GLfloat fogFxStart;		// �Ȱ� �Լ� ���� �Ÿ�
	GLfloat fogFxEnd;		// �ϰ� �Լ� ���� �Ÿ�
	GLfloat fogColor4[4];	// �Ȱ� RGBA ����
	GLfloat fogColorIndex;	// �Ȱ� ���� �ε���
	GLuint fogMode;			// GL_EXP, GL_EXP2, GL_LINEAR
	GLuint fogHint;			// �Ȱ� ��Ʈ(����)
};

#endif