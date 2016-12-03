#include "Texture_Load.h"

void drawBoxFront(int size, bool shaft, GLuint image){
	glPushMatrix(); //Save
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, image);
	glBegin(GL_QUADS);
	if (shaft == true){
		//인호's 샘플코드
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-size, 0, size);   //1
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-size, -(size * 2), size);   //2
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(size, -(size * 2), size);   //3
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(size, 0, size);   //4
	}
	else{
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-size, size, size);   //1
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-size, -size, size);   //2
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(size, -size, size);   //3
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(size, size, size);   //4
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawBoxBack(int size, bool shaft, GLuint image){
	glPushMatrix(); //Save
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, image);
	glBegin(GL_QUADS);
	if (shaft == true){
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-size, 0, -size);   //5
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(size, 0, -size);   //6
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(size, -(size * 2), -size);   //7
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-size, -(size * 2), -size);   //8
	}
	else{
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-size, size, -size);   //5
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(size, size, -size);   //6
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(size, -size, -size);   //7
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-size, -size, -size);   //8
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawBoxLeft(int size, bool shaft, GLuint image){
	glPushMatrix(); //Save
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, image);
	glBegin(GL_QUADS);
	if (shaft == true){
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(-size, 0, size);   //1
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-size, 0, -size);   //5
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-size, -(size * 2), -size);   //8   
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(-size, -(size * 2), size);   //2
	}
	else{
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(-size, size, size);   //1
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-size, size, -size);   //5
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-size, -size, -size);   //8   
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(-size, -size, size);   //2
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void drawBoxRight(int size, bool shaft, GLuint image){
	glPushMatrix(); //Save
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, image);
	glBegin(GL_QUADS);
	if (shaft == true){
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(size, 0, size);   //4
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(size, -(size * 2), size);   //3   
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(size, -(size * 2), -size);   //7
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(size, 0, -size);   //6
	}
	else{
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(size, size, size);   //4
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(size, -size, size);   //3   
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(size, -size, -size);   //7
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(size, size, -size);   //6
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void drawBoxTop(int size, bool shaft, GLuint image){
	glPushMatrix(); //Save
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, image);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	if (shaft == true){
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(-size, 0, size);  //1
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(size, 0, size);   //4
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(size, 0, -size);   //6
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(-size, 0, -size);   //5
	}
	else{
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(-size, size, size);  //1
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(size, size, size);   //4
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(size, size, -size);   //6
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(-size, size, -size);   //5
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawBoxBottom(int size, bool shaft, GLuint image){
	glPushMatrix(); //Save
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, image);
	glBegin(GL_QUADS);
	if (shaft == true){
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-size, -(size * 2), size);   //2
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-size, -(size * 2), -size);   //8
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(size, -(size * 2), -size);   //7
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(size, -(size * 2), size);   //3
	}
	else{
		glTexCoord2d(0.0f, 1.0f);
		glVertex3f(-size, -size, size);   //2
		glTexCoord2d(0.0f, 0.0f);
		glVertex3f(-size, -size, -size);   //8
		glTexCoord2d(1.0f, 0.0f);
		glVertex3f(size, -size, -size);   //7
		glTexCoord2d(1.0f, 1.0f);
		glVertex3f(size, -size, size);   //3
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}