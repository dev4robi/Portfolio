#ifndef __GL_TEXTURE_FONT_H__
#define __GL_TEXTURE_FONT_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : GLTextureFont.h(v1)
*	- Last Update : 2016.07.14
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �ؽ�ó�� �̿��Ͽ� ����, Ư����ȣ, ���� ����(��)�� ���.
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): �ؽ�ó ��Ʈ Ŭ���� ���� ���� (2016.07.14)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

class GLTextureFont {
private:
	string text;			// ����� �ؽ�Ʈ
	float textScale;		// ����� �ؽ�Ʈ ũ��
	GLuint fontTextureID;	// ��Ʈ�� ���� �ؽ�ó ���� ��ȣ
	int fontData;			// ��Ʈ ��ǥ�� ������ ��ȣ

public:
	GLTextureFont(string str, float scale, char *fontName = NULL);	// ������
	GLTextureFont(GLTextureFont& obj);		// ���� ������
	~GLTextureFont();						// �Ҹ���

	void setText(string txt);				// �ش� ���ڷ� �ؽ�Ʈ�� ����
	string getText();						// ����� ���� Ŭ������ �����Ͽ� ��ȯ
	void setTextScale(float scale);			// ��Ʈ ũ�⸦ ����
	float getTextScale();					// ��Ʈ ũ�⸦ ��ȯ
	void setFontTextureID(GLuint texID);	// ��Ʈ �ؽ�ó�� ����
	GLuint getFontTextureID();				// ��Ʈ �ؽ�ó ���̵� ��ȯ
	REAL findCenterOfTextPosX();			// ���ڿ��� �߰� X��ǥ�� ��ȯ

	void drawText();						// ���ڸ� �׸�

private:
	void findFontBaseXY_FromTexture(char c, float *bx, float *by,
											float *fw, float *fh);	// �ؽ�ó�κ��� �ش� ������ ��ġ�� ũ�⸦ ��ȯ�ؿ�
	void correctFontBaseXY(char c, bool inverse);					// ���ں��� �׷��� ��ġ�� ������ inverse �Ű������� y�� ��ǥ�� �ǵ���
};

#endif