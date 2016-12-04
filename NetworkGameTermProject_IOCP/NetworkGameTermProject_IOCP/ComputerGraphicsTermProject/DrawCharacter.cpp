#include "main.h"

//void drawCharacter(Player_Socket *player_socket, bool sight, int character_up_state, float right_leg_x, float right_leg_z, float left_leg_x, float left_leg_z, float right_knee_x, float left_knee_x)
void drawCharacter( Player_Socket *player_socket, Player_Animation *Ani, bool &gunfire )
{
	glPushMatrix();//±×¸®±â ½ºÅ¸Æ®
	glTranslated( player_socket->x, player_socket->y + 140, player_socket->z );
	glRotatef( player_socket->camxrotate + 180, 0, 1, 0 );


	drawGun( player_socket, gunfire );
	/*glutWireCube(250);

	glPushMatrix();
	glColor3f(0, 0, 255);
	glTranslated(0, -140, 0);
	glBegin(GL_QUADS);
	glVertex3f(100, 0, 100);
	glVertex3f(100, 0, -100);
	glVertex3f(-100, 0, -100);
	glVertex3f(-100, 0, 100);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 255, 255);
	glTranslated(0, -140, 0);
	glBegin(GL_QUADS);
	glVertex3f(0, 250, 100);
	glVertex3f(300, 250, 100);
	glVertex3f(300, 0, 100);
	glVertex3f(0, 0, 100);
	glEnd();
	glPopMatrix();*/


	glPushMatrix(); //Save ¸Ó¸®
	glTranslated( 0, 75, 0 );
	glRotatef( -(player_socket->camyrotate + 90) / 3, 1, 0, 0 );
	glScaled( 1.0, 1.0, 0.7 );
	drawBoxFront( 30, false, character_head_object[0] );
	drawBoxBack( 30, false, character_head_object[1] );
	drawBoxLeft( 30, false, character_head_object[2] );
	drawBoxRight( 30, false, character_head_object[3] );
	drawBoxTop( 30, false, character_head_object[4] );
	drawBoxBottom( 30, false, character_head_object[5] );
	glPopMatrix(); //¸Ó¸® end

	glPushMatrix(); //Save ¸öÅë
	glScaled( 1.0, 1.5, 0.5 );
	drawBoxFront( 30, false, character_body_object[0] );
	drawBoxBack( 30, false, character_body_object[1] );
	drawBoxLeft( 30, false, character_body_object[2] );
	drawBoxRight( 30, false, character_body_object[3] );
	drawBoxTop( 30, false, character_body_object[4] );
	drawBoxBottom( 30, false, character_body_object[5] );
	glPopMatrix(); //¸öÅë end


	glPushMatrix(); //Save ¿À¸¥ÆÈ ¾î²²
	glTranslated( -45, 44, 0 );
	glRotatef( -player_socket->camyrotate + 210, 1, 0, 0 );
	glRotatef( 30, 0, 1, 0 );
	glRotatef( 0, 0, 0, 1 );
	glScaled( 0.5, 0.75, 0.5 );
	drawBoxFront( 30, true, character_arm_top_object[0] );
	drawBoxBack( 30, true, character_arm_top_object[1] );
	drawBoxLeft( 30, true, character_arm_top_object[2] );
	drawBoxRight( 30, true, character_arm_top_object[3] );
	drawBoxTop( 30, true, character_arm_top_object[4] );
	drawBoxBottom( 30, true, character_arm_top_object[5] );
	glScaled( 2, 1.333333, 2 );

	glPushMatrix(); //Save ¿À¸¥ÆÈ ÆÈ²ÞÄ¡
	glTranslated( 0, -45, 0 );
	glRotatef( -30, 1, 0, 0 );
	glPushMatrix();
	glScaled( 0.5, 0.75, 0.5 );
	drawBoxFront( 30, true, character_arm_bottom_object[0] );
	drawBoxBack( 30, true, character_arm_bottom_object[1] );
	drawBoxLeft( 30, true, character_arm_bottom_object[2] );
	drawBoxRight( 30, true, character_arm_bottom_object[3] );
	drawBoxTop( 30, true, character_arm_top_object[5] );
	drawBoxBottom( 30, true, character_arm_top_object[5] );
	glPopMatrix();
	glPopMatrix();//¿À¸¥ÆÈ ÆÈ²ÞÄ¡ Á¾·á
	glPopMatrix();//¿À¸¥ÆÈ ¾î²² Á¾·á


	glPushMatrix(); //Save ¿ÞÆÈ ¾î²² 
	glTranslated( 45, 44, 0 );
	glRotatef( -player_socket->camyrotate - 90, 1, 0, 0 );
	glRotatef( 70, 0, 1, 0 );
	glRotatef( -75, 0, 0, 1 );
	glScaled( 0.5, 0.75, 0.5 );
	drawBoxFront( 30, true, character_arm_top_object[0] );
	drawBoxBack( 30, true, character_arm_top_object[1] );
	drawBoxLeft( 30, true, character_arm_top_object[2] );
	drawBoxRight( 30, true, character_arm_top_object[3] );
	drawBoxTop( 30, true, character_arm_top_object[4] );
	drawBoxBottom( 30, true, character_arm_top_object[5] );
	glScaled( 2, 1.333333, 2 );

	glPushMatrix(); //Save ¿ÞÆÈ ÆÈ²ÞÄ¡
	glTranslated( 0, -45, 0 );
	glRotatef( 25, 1, 0, 0 );
	glScaled( 0.5, 0.75, 0.5 );
	drawBoxFront( 30, true, character_arm_bottom_object[0] );
	drawBoxBack( 30, true, character_arm_bottom_object[1] );
	drawBoxLeft( 30, true, character_arm_bottom_object[2] );
	drawBoxRight( 30, true, character_arm_bottom_object[3] );
	drawBoxTop( 30, true, character_arm_top_object[5] );
	drawBoxBottom( 30, true, character_arm_top_object[5] );
	glPopMatrix();//¿ÞÆÈ ÆÈ²ÞÄ¡ Á¾·á
	glPopMatrix();//¿ÞÆÈ¾î²² Á¾·á



	glPushMatrix(); //Save ¿À¸¥ÂÊ °ñ¹Ý
	glTranslated( -15, -40, 0 );
	glRotatef( Ani->right_leg_x, 1, 0, 0 );
	glRotatef( 0, 0, 1, 0 );
	glRotatef( Ani->right_leg_z, 0, 0, 1 );
	glScaled( 0.5, 0.75, 0.5 );
	drawBoxFront( 30, true, character_leg_top_object[0] );
	drawBoxBack( 30, true, character_leg_top_object[1] );
	drawBoxLeft( 30, true, character_leg_top_object[2] );
	drawBoxRight( 30, true, character_leg_top_object[3] );
	drawBoxTop( 30, true, character_leg_top_object[4] );
	drawBoxBottom( 30, true, character_leg_top_object[5] );
	glScaled( 2, 1.333333, 2 );

	glPushMatrix(); //Save ¿À¸¥ÂÊ ¹«¸­
	glTranslated( 0, -45, 0 );
	glRotatef( Ani->right_knee_x, 1, 0, 0 );
	glScaled( 0.5, 0.75, 0.5 );
	drawBoxFront( 30, true, character_leg_bottom_object[0] );
	drawBoxBack( 30, true, character_leg_bottom_object[1] );
	drawBoxLeft( 30, true, character_leg_bottom_object[2] );
	drawBoxRight( 30, true, character_leg_bottom_object[3] );
	drawBoxTop( 30, true, character_leg_top_object[4] );
	drawBoxBottom( 30, true, character_leg_top_object[5] );
	glPopMatrix();//¿À¸¥ÂÊ ¹«¸­ Á¾·á

	glPopMatrix();//¿À¸¥ÂÊ °ñ¹Ý Á¾·á


	glPushMatrix(); //Save ¿ÞÂÊ °ñ¹Ý
	glTranslated( 15, -40, 0 );
	glRotatef( Ani->left_leg_x, 1, 0, 0 );
	glRotatef( 0, 0, 1, 0 );
	glRotatef( Ani->left_leg_z, 0, 0, 1 );
	glScaled( 0.5, 0.75, 0.5 );
	drawBoxFront( 30, true, character_leg_top_object[0] );
	drawBoxBack( 30, true, character_leg_top_object[1] );
	drawBoxLeft( 30, true, character_leg_top_object[2] );
	drawBoxRight( 30, true, character_leg_top_object[3] );
	drawBoxTop( 30, true, character_leg_top_object[4] );
	drawBoxBottom( 30, true, character_leg_top_object[5] );
	glScaled( 2, 1.333333, 2 );

	glPushMatrix(); //Save ¿ÞÂÊ ¹«¸­
	glTranslated( 0, -45, 0 );
	glRotatef( Ani->left_knee_x, 1, 0, 0 );
	glScaled( 0.5, 0.75, 0.5 );
	drawBoxFront( 30, true, character_leg_bottom_object[0] );
	drawBoxBack( 30, true, character_leg_bottom_object[1] );
	drawBoxLeft( 30, true, character_leg_bottom_object[2] );
	drawBoxRight( 30, true, character_leg_bottom_object[3] );
	drawBoxTop( 30, true, character_leg_top_object[4] );
	drawBoxBottom( 30, true, character_leg_top_object[5] );
	glPopMatrix(); //¿ÞÂÊ ¹«¸­ Á¾·á

	glPopMatrix(); //¿ÞÂÊ°ñ¹Ý Á¾·á

	glPopMatrix(); //±×¸®±â Á¾·á
}


void init_Character_Texture() {
	head_Texture( character_head_object );
	body_Texture( character_body_object );
	arm_Texture( character_arm_top_object, character_arm_bottom_object );
	leg_Texture( character_leg_top_object, character_leg_bottom_object );
}

void drawGun( Player_Socket *player_socket, bool &gunfire )
{

	glPushMatrix();//ÃÑ ½ÃÀÛ
	glColor3f( 0, 0, 1 );
	glTranslatef( 0, 40, 0 );
	glRotatef( -player_socket->camyrotate - 90, 1, 0, 0 );
	//glRotatef(-60, 0, 1, 0);
	glTranslatef( -25, 0, 120 );
	glScalef( 0.15, 0.2, 1.0 );
	glutSolidCube( 120 );//¸öÃ¼

	glPushMatrix();//¼ÕÀâÀÌ
	glColor3f( 0, 0, 1 );
	glTranslatef( 0, -100, -45 );
	glScalef( 6.6, 6.6, 1.0 );
	glScalef( 1.0, 1.2, 1.0 );
	glutSolidCube( 10 );
	glPopMatrix();

	glPushMatrix();//ÅºÃ¢
	glColor3f( 1, 0, 0 );
	glTranslatef( 0, -100, 0 );
	glScalef( 6.6, 6.6, 1.0 );
	glScalef( 1.0, 2.0, 1.0 );
	glutSolidCube( 15 );
	glPopMatrix();

	glPushMatrix();//ÃÑ±¸
	glColor3f( 1, 0, 0 );
	glTranslatef( 0, 10, 70 );
	glScalef( 6.6, 6.6, 1.0 );
	glScalef( 1.0, 1.0, 5.0 );
	glutSolidCube( 5 );
	glPopMatrix();


	glPushMatrix();//°³¸Ó¸® ÁöÁö´ë1
	glColor3f( 1, 0, 0 );
	glTranslatef( 0, 25, -70 );
	glScalef( 6.6, 6.6, 1.0 );
	glScalef( 1.0, 1.0, 9.0 );
	glutSolidCube( 3 );
	glPopMatrix();
	glPushMatrix();//°³¸Ó¸® ÁöÁö´ë2
	glColor3f( 1, 0, 0 );
	glTranslatef( 0, 0, -70 );
	glScalef( 6.6, 6.6, 1.0 );
	glScalef( 1.0, 1.0, 9.0 );
	glutSolidCube( 3 );
	glPopMatrix();

	glPushMatrix();//°³¸Ó¸® ¹ÞÄ§
	glColor3f( 1, 0, 0 );
	glTranslatef( 0, 0, -85 );
	glScalef( 6.6, 6.6, 1.0 );
	glScalef( 2.0, 5.5, 1.0 );
	glutSolidCube( 3 );
	glPopMatrix();



	glPushMatrix();//°¡´Æ¼è
	glColor3f( 0, 1, 0 );
	glTranslatef( 0, 70, 55 );
	glScalef( 6.6, 6.6, 1.0 );
	glScalef( 1.0, 1.0, 1.0 );
	glutSolidCube( 2 );
	glPopMatrix();

	glPushMatrix();//°¡´ÆÁÂ1
	glColor3f( 0, 1, 0 );
	glTranslatef( 10, 70, -55 );
	glScalef( 6.6, 6.6, 1.0 );
	glScalef( 1.0, 1.0, 1.0 );
	glutSolidCube( 2 );
	glPopMatrix();

	glPushMatrix();//°¡´ÆÁÂ2
	glColor3f( 0, 1, 0 );
	glTranslatef( -10, 70, -55 );
	glScalef( 6.6, 6.6, 1.0 );
	glScalef( 1.0, 1.0, 1.0 );
	glutSolidCube( 2 );
	glPopMatrix();

	if ( gunfire )
	{
		glPushMatrix();//°¡´ÆÁÂ2
		glTranslatef( 0, 10, 75 );

		glPushMatrix();
		glBegin( GL_TRIANGLES );
		glTranslatef( 0, 40, 0 );
		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex3f( 0, 200, 0 );
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( 30, 0, 0 );
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( -30, 00, 0 );
		glEnd();
		glPopMatrix();


		glPushMatrix();
		glRotatef( 72.0f, 0, 0, 1 );
		glBegin( GL_TRIANGLES );
		glTranslatef( 0, 40, 0 );
		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex3f( 0, 200, 0 );
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( 30, 0, 0 );
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( -30, 00, 0 );
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glRotatef( (72.0f * 2), 0, 0, 1 );
		glBegin( GL_TRIANGLES );
		glTranslatef( 0, 40, 0 );
		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex3f( 0, 200, 0 );
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( 30, 0, 0 );
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( -30, 00, 0 );
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glRotatef( (72.0f * 3), 0, 0, 1 );
		glBegin( GL_TRIANGLES );
		glTranslatef( 0, 40, 0 );
		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex3f( 0, 200, 0 );
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( 30, 0, 0 );
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( -30, 00, 0 );
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glRotatef( (72.0f * 4), 0, 0, 1 );
		glBegin( GL_TRIANGLES );
		glTranslatef( 0, 40, 0 );
		glColor3f( 1.0f, 0.0f, 0.0f );
		glVertex3f( 0, 200, 0 );
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( 30, 0, 0 );
		glColor3f( 1.0f, 1.0f, 0.0f );
		glVertex3f( -30, 00, 0 );
		glEnd();
		glPopMatrix();


		glPopMatrix();
	}


	glPopMatrix();//ÃÑ Á¾·á
}