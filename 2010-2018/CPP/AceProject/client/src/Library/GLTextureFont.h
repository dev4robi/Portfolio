#ifndef __GL_TEXTURE_FONT_H__
#define __GL_TEXTURE_FONT_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Name : GLTextureFont.h(v1)
*	- Last Update : 2016.07.14
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 텍스처를 이용하여 영문, 특수기호, 숫자 문자(열)을 출력.
*
*	- Dependency : [StdHeader.h]
*
*   - Version(v1): 텍스처 폰트 클래스 기초 설계 (2016.07.14)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"

class GLTextureFont {
private:
	string text;			// 출력할 텍스트
	float textScale;		// 출력할 텍스트 크기
	GLuint fontTextureID;	// 폰트에 사용될 텍스처 파일 번호
	int fontData;			// 폰트 좌표셋 데이터 번호

public:
	GLTextureFont(string str, float scale, char *fontName = NULL);	// 생성자
	GLTextureFont(GLTextureFont& obj);		// 복사 생성자
	~GLTextureFont();						// 소멸자

	void setText(string txt);				// 해당 문자로 텍스트를 설정
	string getText();						// 저장된 문자 클래스를 복사하여 반환
	void setTextScale(float scale);			// 폰트 크기를 설정
	float getTextScale();					// 폰트 크기를 반환
	void setFontTextureID(GLuint texID);	// 폰트 텍스처를 설정
	GLuint getFontTextureID();				// 폰트 텍스처 아이디를 반환
	REAL findCenterOfTextPosX();			// 문자열의 중간 X좌표를 반환

	void drawText();						// 글자를 그림

private:
	void findFontBaseXY_FromTexture(char c, float *bx, float *by,
											float *fw, float *fh);	// 텍스처로부터 해당 문자의 위치와 크기를 반환해옴
	void correctFontBaseXY(char c, bool inverse);					// 문자별로 그려질 위치를 보정함 inverse 매개변수로 y축 좌표를 되돌림
};

#endif