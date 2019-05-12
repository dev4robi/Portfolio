#include "Test/Test2DWidget.h"
#include "Library/GLTextureFont.h"

Test2DWidget::Test2DWidget() {
	set2D(true);
}

Test2DWidget::Test2DWidget(Vector3 pos) : GameWidget(pos) {
	set2D(true);
}

Test2DWidget::~Test2DWidget() {

}

bool Test2DWidget::draw() {
	glMatrixMode(GL_MODELVIEW);

	static const float size = 200.0;

	glTranslatef(getPosX(), getPosY(), getPosZ());

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);
	{
		glColor4f(0.5, 1.0, 0.5, 0.5);
		glTexCoord2f(0.0, 1.0);	glVertex3f(-200, 200, 0.0);
		glTexCoord2f(0.0, 0.0);	glVertex3f(-200, -200, 0.0);
		glTexCoord2f(1.0, 0.0);	glVertex3f(200, -200, 0.0);
		glTexCoord2f(1.0, 1.0);	glVertex3f(200, 200, 0.0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

/*	glTranslatef(-1700, 500.0, 0.0);
	GLTextureFont font2(string(" YZ[\\]^-`abcdefghijklmnopqrstuvwxyz{|}"), 10.0);
	font2.drawText();
	glTranslatef(0.0, -30.0, 0.0);
	GLTextureFont font1(string("! \"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWX"), 10.0);
	font1.drawText();
*/
	return true;
}
