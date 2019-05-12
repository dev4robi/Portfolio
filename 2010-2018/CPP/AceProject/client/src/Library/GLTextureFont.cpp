#include "Library/GLTextureFont.h"
#include "Engine/DrawManager.h"
#include "Engine/ResourceManager.h"

GLTextureFont::GLTextureFont(string txt, float scale, char *fontName) {
	text = txt;
	setTextScale(scale);
	fontData = 0;

	if (fontName == NULL) {
		fontData = 1; // 1번 좌표셋을 사용하도록 함
		setFontTextureID(g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/font/basicfont.png"));
	}
	else {
		// 텍스처 번호 로드
		setFontTextureID(g_pGameManager->getResourceManagerPtr()->getTextureID(fontName));

		// koverwatch 폰트
		if (strcmp(fontName, "res/Textures/font/koverwatchFont.png") == 0) { // 2번 좌표셋을 사용하도록 함
			fontData = 2;
		}
		// basicfont 폰트
		else {
			fontData = 1; // 1번 좌표셋을 사용하도록 함
			setFontTextureID(g_pGameManager->getResourceManagerPtr()->getTextureID("res/Textures/font/basicfont.png"));
		}
	}


}

GLTextureFont::GLTextureFont(GLTextureFont& obj) {
	text = obj.text;
	textScale = obj.textScale;
	fontTextureID = obj.fontTextureID;
}

GLTextureFont::~GLTextureFont() { }

void GLTextureFont::setText(string txt) {
	text = txt;
}

string GLTextureFont::getText() {
	return text;
}

void GLTextureFont::setTextScale(float scale) {
	textScale = scale;
}

float GLTextureFont::getTextScale() {
	return textScale;
}

void GLTextureFont::setFontTextureID(GLuint texID) {
	fontTextureID = texID;
}

GLuint GLTextureFont::getFontTextureID() {
	return fontTextureID;
}

void GLTextureFont::drawText() {
	// 화면의 현재 해상도를 받아옴
	Size2D *pResolution = g_pGameManager->getDrawManagerPtr()->getResolutionPtr();
	float scrWidth = pResolution->getWidth();
	float screenHeight = pResolution->getHeight();

	glPushMatrix();
	{
		// 문자열의 시작부터 끝까지 돌면서 반복
		for (int i = 0; i < text.size(); ++i) {
			// 입력된 문자가 출력 가능한 아스키코드가 아니면
			if (text[i] < ' ' || text[i] > '~') {
				// 입력된 문자를 '?'로 강제 변경
				text[i] = '?';
			}
			// 출력될 폰트의 크기와 좌표를 설정 (fontBaseX/Y: 폰트 이미지에서의 좌상 X/Y축 좌표, fontWidth/Height: 폰트 이미지의 폭과 높이)
			float fontBaseX = 0.0; float fontWidth = 0.0; float fontBaseY = 0.0; float fontHeight = 0.0;

			findFontBaseXY_FromTexture(text[i], &fontBaseX, &fontBaseY, &fontWidth, &fontHeight); // 폰트 정보를 담은 함수로부터 해당 문자의 베이스 위치를 얻어온다

			// 해상도에 따라 계산된 폰트의 픽셀 크기 (1024 = 폰트 이미지의 크기, 1920, 1080 = 기준 해상도)
			float width = scrWidth * (fontWidth * 1024.0) / 1920.0;
			float height = screenHeight * (fontHeight * 1024.0) / 1080.0;

			// 문자에 따라 위치 보정을 가함
			correctFontBaseXY(text[i], false);

			// 폰트 사이즈 적용
			if (i == 0) glScalef(textScale / 50.0, textScale / 50.0, 1.0);

			// 그리기 시작
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, fontTextureID);
			glBegin(GL_QUADS);
			{	
				glTexCoord2f(fontBaseX, fontBaseY); 							glVertex3f(0.0, height, 0.0);			// 1.좌상  1		4
				glTexCoord2f(fontBaseX, fontBaseY + fontHeight);				glVertex3f(0.0, -height, 0.0);			// 2.좌하  |		|
				glTexCoord2f(fontBaseX + fontWidth, fontBaseY + fontHeight); 	glVertex3f(width * 2.0, -height, 0.0);	// 3.우하  |		|
				glTexCoord2f(fontBaseX + fontWidth, fontBaseY);					glVertex3f(width * 2.0, height, 0.0);	// 4.우상  2------→3
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);

			// 다음 그려질 폰트의 y좌표를 재보정
			correctFontBaseXY(text[i], true);

			// 문자의 크기만큼 우측으로 이동
			glTranslatef(width * 2.0, 0.0, 0.0);
		}
	}
	glPopMatrix();
}

void GLTextureFont::findFontBaseXY_FromTexture(char c, float *bx, float *by, float *fw, float *fh) {
	// c: 입력된 문자	// bx: 폰트의 텍스처 좌상x	// by: 폰트의 텍스처 좌상y
	// fw: 폰트 폭		// fh: 폰트 높이

	int fontX = 0, fontY = 0; // 폰트의 좌상단 좌표
	int fontW = 0, fontH = 0; // 폰트의 폭과 높이

	// basicfont.png (1)
	if (fontData == 1)
	{
		// autoBX/Y: 비례식에 맞게 자동 계산된 폰트의 텍스처 좌상단 XY좌표
		float autoBX = (((c - 32) % 10) * 102) - 1; // 102(문자 픽셀의 공통된 최대 크기), -1(폰트 상단, 좌단으로 1픽셀씩 추가)
		float autoBY = (((c - 32) / 10) * 102) - 1;

		if (autoBX < 0.0) autoBX = 0.0;
		if (autoBY < 0.0) autoBY = 0.0;

		switch (c) {
		case ' ':	fontW = 45;		fontH = 78;		break;
		case '!':	fontW = 12;		fontH = 76;		break;
		case '"':	fontW = 34;		fontH = 25;		break;
		case '#':	fontW = 64;		fontH = 76;		break;
		case '$': 	fontW = 48;		fontH = 88;		break;
		case '%': 	fontW = 71;		fontH = 79;		break;
		case '&': 	fontW = 62;		fontH = 79;		break;
		case '\'': 	fontW = 26;		fontH = 29;		break;
		case '(': 	fontW = 29;		fontH = 95;		break;
		case ')': 	fontW = 30;		fontH = 95;		break;
		case '*': 	fontW = 45;		fontH = 38;		break;
		case '+': 	fontW = 61;		fontH = 61;		break;
		case ',': 	fontW = 20;		fontH = 30;		break;
		case '-': 	fontW = 48;		fontH = 9;		break;
		case '.': 	fontW = 22;		fontH = 14;		break;
		case '/': 	fontW = 38;		fontH = 91;		break;
		case '0': 	fontW = 56;		fontH = 79;		break;
		case '1': 	fontW = 45;		fontH = 79;		break;
		case '2': 	fontW = 50;		fontH = 78;		break;
		case '3': 	fontW = 49;		fontH = 79;		break;
		case '4': 	fontW = 54;		fontH = 76;		break;
		case '5': 	fontW = 45;		fontH = 78;		break;
		case '6': 	fontW = 53;		fontH = 80;		break;
		case '7': 	fontW = 51;		fontH = 76;		break;
		case '8': 	fontW = 53;		fontH = 80;		break;
		case '9': 	fontW = 55;		fontH = 80;		break;
		case ':': 	fontW = 26;		fontH = 56;		break;
		case ';': 	fontW = 19;		fontH = 72;		break;
		case '<': 	fontW = 65;		fontH = 61;		break;
		case '=': 	fontW = 65;		fontH = 28;		break;
		case '>': 	fontW = 60;		fontH = 62;		break;
		case '?': 	fontW = 48;		fontH = 78;		break;
		case '@': 	fontW = 82;		fontH = 97;		break;
		case 'A': 	fontW = 72;		fontH = 76;		break;
		case 'B': 	fontW = 49;		fontH = 76;		break;
		case 'C': 	fontW = 64;		fontH = 80;		break;
		case 'D': 	fontW = 66;		fontH = 76;		break;
		case 'E': 	fontW = 49;		fontH = 76;		break;
		case 'F': 	fontW = 47;		fontH = 79;		break;
		case 'G': 	fontW = 66;		fontH = 80;		break;
		case 'H': 	fontW = 58;		fontH = 78;		break;
		case 'I': 	fontW = 32;		fontH = 78;		break;
		case 'J': 	fontW = 40;		fontH = 92;		break;
		case 'K': 	fontW = 55;		fontH = 78;		break;
		case 'L': 	fontW = 52;		fontH = 78;		break;
		case 'M': 	fontW = 73;		fontH = 78;		break;
		case 'N': 	fontW = 61;		fontH = 78;		break;
		case 'O': 	fontW = 75;		fontH = 81;		break;
		case 'P': 	fontW = 50;		fontH = 78;		break;
		case 'Q': 	fontW = 82;		fontH = 94;		break;
		case 'R': 	fontW = 56;		fontH = 78;		break;
		case 'S': 	fontW = 47;		fontH = 81;		break;
		case 'T': 	fontW = 65;		fontH = 78;		break;
		case 'U': 	fontW = 65;		fontH = 80;		break;
		case 'V': 	fontW = 67;		fontH = 79;		break;
		case 'W': 	fontW = 87;		fontH = 78;		break;
		case 'X': 	fontW = 66;		fontH = 78;		break;
		case 'Y': 	fontW = 67;		fontH = 78;		break;
		case 'Z': 	fontW = 58;		fontH = 78;		break;
		case '[': 	fontW = 26;		fontH = 97;		break;
		case '\\': 	fontW = 36;		fontH = 93;		break;
		case ']': 	fontW = 22;		fontH = 98;		break;
		case '^': 	fontW = 58;		fontH = 58;		break;
		case '_': 	fontW = 55;		fontH = 12;		break;
		case '`': 	fontW = 40;		fontH = 20;		break;
		case 'a': 	fontW = 53;		fontH = 61;		break;
		case 'b': 	fontW = 53;		fontH = 84;		break;
		case 'c': 	fontW = 48;		fontH = 61;		break;
		case 'd': 	fontW = 55;		fontH = 84;		break;
		case 'e': 	fontW = 50;		fontH = 61;		break;
		case 'f': 	fontW = 42;		fontH = 84;		break;
		case 'g': 	fontW = 56;		fontH = 81;		break;
		case 'h': 	fontW = 47;		fontH = 83;		break;
		case 'i': 	fontW = 20;		fontH = 78;		break;
		case 'j': 	fontW = 40;		fontH = 99;		break;
		case 'k': 	fontW = 48;		fontH = 83;		break;
		case 'l': 	fontW = 35;		fontH = 83;		break;
		case 'm': 	fontW = 78;		fontH = 60;		break;
		case 'n': 	fontW = 50;		fontH = 60;		break;
		case 'o': 	fontW = 58;		fontH = 61;		break;
		case 'p': 	fontW = 48;		fontH = 81;		break;
		case 'q': 	fontW = 48;		fontH = 81;		break;
		case 'r': 	fontW = 32;		fontH = 61;		break;
		case 's': 	fontW = 40;		fontH = 62;		break;
		case 't': 	fontW = 40;		fontH = 72;		break;
		case 'u': 	fontW = 49;		fontH = 61;		break;
		case 'v': 	fontW = 57;		fontH = 60;		break;
		case 'w': 	fontW = 78;		fontH = 60;		break;
		case 'x': 	fontW = 59;		fontH = 60;		break;
		case 'y': 	fontW = 59;		fontH = 80;		break;
		case 'z': 	fontW = 48;		fontH = 60;		break;
		case '{': 	fontW = 28;		fontH = 99;		break;
		case '|': 	fontW = 15;		fontH = 99;		break;
		case '}': 	fontW = 29;		fontH = 99;		break;
		case '~': 	fontW = 58;		fontH = 24;		break;
		default:	fontW = 42;		fontH = 78;		// char c = '?'
		}

		// 픽셀 좌표를 GL텍스처 좌표로 변경해줌
		*bx = (float)autoBX / 1024.0;		*by = (float)autoBY / 1024.0;
		*fw = (float)fontW / 1024.0;		*fh = (float)fontH / 1024.0;
	}

	// koverwatchfont.png (2)
	else if (fontData == 2)
	{
		switch (c) {
		case' ':  fontX = 0, fontY = 0, fontW = 29, fontH = 140; break;
		case'!':  fontX = 29, fontY = 0, fontW = 26, fontH = 140; break;
		case'"':  fontX = 55, fontY = 0, fontW = 44, fontH = 140; break;
		case'#':  fontX = 99, fontY = 0, fontW = 70, fontH = 140; break;
		case'$':  fontX = 169, fontY = 0, fontW = 57, fontH = 140; break;
		case'%':  fontX = 226, fontY = 0, fontW = 104, fontH = 140; break;
		case'&':  fontX = 330, fontY = 0, fontW = 56, fontH = 140; break;
		case'\'': fontX = 386, fontY = 0, fontW = 22, fontH = 140; break;
		case'(':  fontX = 408, fontY = 0, fontW = 31, fontH = 140; break;
		case')':  fontX = 469, fontY = 0, fontW = 31, fontH = 140; break;
		case'*':  fontX = 501, fontY = 0, fontW = 53, fontH = 140; break;
		case'+':  fontX = 554, fontY = 0, fontW = 57, fontH = 140; break;
		case',':  fontX = 611, fontY = 0, fontW = 25, fontH = 140; break;
		case'-':  fontX = 636, fontY = 0, fontW = 36, fontH = 140; break;
		case'.':  fontX = 672, fontY = 0, fontW = 36, fontH = 140; break;
		case'/':  fontX = 726, fontY = 0, fontW = 45, fontH = 140; break;
		case'0':  fontX = 0, fontY = 150, fontW = 57, fontH = 140; break;
		case'1':  fontX = 57, fontY = 150, fontW = 37, fontH = 140; break;
		case'2':  fontX = 94, fontY = 150, fontW = 56, fontH = 140; break;
		case'3':  fontX = 150, fontY = 150, fontW = 56, fontH = 140; break;
		case'4':  fontX = 206, fontY = 150, fontW = 55, fontH = 140; break;
		case'5':  fontX = 261, fontY = 150, fontW = 54, fontH = 140; break;
		case'6':  fontX = 315, fontY = 150, fontW = 58, fontH = 140; break;
		case'7':  fontX = 373, fontY = 150, fontW = 45, fontH = 140; break;
		case'8':  fontX = 418, fontY = 150, fontW = 57, fontH = 140; break;
		case'9':  fontX = 475, fontY = 150, fontW = 58, fontH = 140; break;
		case':':  fontX = 533, fontY = 150, fontW = 27, fontH = 140; break;
		case';':  fontX = 560, fontY = 150, fontW = 28, fontH = 140; break;
		case'<':  fontX = 588, fontY = 150, fontW = 45, fontH = 140; break;
		case'=':  fontX = 633, fontY = 150, fontW = 56, fontH = 140; break;
		case'>':  fontX = 689, fontY = 150, fontW = 45, fontH = 140; break;
		case'?':  fontX = 734, fontY = 150, fontW = 56, fontH = 140; break;
		case'@':  fontX = 790, fontY = 150, fontW = 80, fontH = 140; break;
		case'a':case'A':  fontX = 0, fontY = 300, fontW = 56, fontH = 140; break;
		case'b':case'B':  fontX = 56, fontY = 300, fontW = 58, fontH = 140; break;
		case'c':case'C':  fontX = 114, fontY = 300, fontW = 56, fontH = 140; break;
		case'd':case'D':  fontX = 170, fontY = 300, fontW = 58, fontH = 140; break;
		case'e':case'E':  fontX = 228, fontY = 300, fontW = 44, fontH = 140; break;
		case'f':case'F':  fontX = 272, fontY = 300, fontW = 44, fontH = 140; break;
		case'g':case'G':  fontX = 316, fontY = 300, fontW = 59, fontH = 140; break;
		case'h':case'H':  fontX = 375, fontY = 300, fontW = 59, fontH = 140; break;
		case'i':case'I':  fontX = 434, fontY = 300, fontW = 28, fontH = 140; break;
		case'j':case'J':  fontX = 462, fontY = 300, fontW = 40, fontH = 140; break;
		case'k':case'K':  fontX = 502, fontY = 300, fontW = 53, fontH = 140; break;
		case'l':case'L':  fontX = 555, fontY = 300, fontW = 42, fontH = 140; break;
		case'm':case'M':  fontX = 597, fontY = 300, fontW = 73, fontH = 140; break;
		case'n':case'N':  fontX = 670, fontY = 300, fontW = 58, fontH = 140; break;
		case'o':case'O':  fontX = 728, fontY = 300, fontW = 61, fontH = 140; break;
		case'p':case'P':  fontX = 789, fontY = 300, fontW = 56, fontH = 140; break;
		case'q':case'Q':  fontX = 845, fontY = 300, fontW = 61, fontH = 140; break;
		case'r':case'R':  fontX = 906, fontY = 300, fontW = 56, fontH = 140; break;
		case's':case'S':  fontX = 962, fontY = 300, fontW = 55, fontH = 140; break;
		case't':case'T':  fontX = 0, fontY = 450, fontW = 44, fontH = 140; break;
		case'u':case'U':  fontX = 44, fontY = 450, fontW = 59, fontH = 140; break;
		case'v':case'V':  fontX = 103, fontY = 450, fontW = 55, fontH = 140; break;
		case'w':case'W':  fontX = 158, fontY = 450, fontW = 84, fontH = 140; break;
		case'x':case'X':  fontX = 242, fontY = 450, fontW = 50, fontH = 140; break;
		case'y':case'Y':  fontX = 292, fontY = 450, fontW = 52, fontH = 140; break;
		case'z':case'Z':  fontX = 344, fontY = 450, fontW = 45, fontH = 140; break;
		case'[':  fontX = 389, fontY = 450, fontW = 34, fontH = 140; break;
		case'\\': fontX = 450, fontY = 450, fontW = 45, fontH = 140; break;
		case']':  fontX = 523, fontY = 450, fontW = 34, fontH = 140; break;
		case'^':  fontX = 557, fontY = 450, fontW = 62, fontH = 140; break;
		case'_':  fontX = 615, fontY = 450, fontW = 50, fontH = 140; break;
		case'`':  fontX = 665, fontY = 450, fontW = 23, fontH = 140; break;
		case'{':  fontX = 688, fontY = 446, fontW = 42, fontH = 140; break;
		case'|':  fontX = 730, fontY = 450, fontW = 26, fontH = 140; break;
		case'}':  fontX = 756, fontY = 446, fontW = 42, fontH = 140; break;
		case'~':  fontX = 800, fontY = 450, fontW = 60, fontH = 140; break;
		default:  fontX = 734, fontY = 150, fontW = 56, fontH = 140; // c => '?'
		}

		// 픽셀 좌표를 GL텍스처 좌표로 변경해줌
		*bx = (float)fontX / 1024.0;		*by = (float)fontY / 1024.0;
		*fw = (float)fontW / 1024.0;		*fh = (float)fontH / 1024.0;
	}
}

void GLTextureFont::correctFontBaseXY(char c, bool inverse) {
	float cx = 0.0, cy = 0.0;

	if (fontData == 1)
	{
		switch (c) {
		case '^':	cx = 50.0;		cy = 0.0;		break;
		case '`':	cx = 0.0;		cy = 40.0;		break;
		case '\"':	cx = 0.0;		cy = 40.0;		break;
		case '\'':	cx = 0.0;		cy = 40.0;		break;
		case ',':	cx = 0.0;		cy = -80.0;		break;
		case '.':	cx = 0.0;		cy = -80.0;		break;
		case 'a':	cx = -5.0;		cy = -20.0;		break;
		case 'c':	cx = 0.0;		cy = -20.0;		break;
		case 'e':	cx = 0.0;		cy = -20.0;		break;
		case 'm':	cx = 0.0;		cy = -15.0;		break;
		case 'n':	cx = 0.0;		cy = -15.0;		break;
		case 'o':	cx = 0.0;		cy = -15.0;		break;
		case 'r':	cx = 0.0;		cy = -20.0;		break;
		case 's':	cx = 0.0;		cy = -15.0;		break;
		case 't':	cx = 0.0;		cy = -15.0;		break;
		case 'u':	cx = 0.0;		cy = -15.0;		break;
		case 'v':	cx = 0.0;		cy = -15.0;		break;
		case 'w':	cx = 0.0;		cy = -15.0;		break;
		case 'x':	cx = 0.0;		cy = -15.0;		break;
		case 'z':	cx = 0.0;		cy = -15.0;		break;
		default:	cx = 0.0;		cy = 0.0;
		}
	}

	// 그리기 좌표 이동
	glTranslatef((inverse == true) ? 0.0 : cx, (inverse == true) ? -cy : cy, 0.0);
}

REAL GLTextureFont::findCenterOfTextPosX() {
	REAL rtPosX = 0.0;
	REAL bx = 0.0, by = 0.0, fw = 0.0, fh = 0.0;

	// 전체 문자열의 절반만큼 문자 하나하나의 폭을 연산
	for (int i = 0; i < text.size(); ++i) {
		findFontBaseXY_FromTexture(text[i], &bx, &by, &fw, &fh);
		rtPosX += fw;
	}

	// 문자열 전체 길이의 중간이 되는 좌표를 반환
	return ((rtPosX * 1024.0) * textScale / 50.0);
}
