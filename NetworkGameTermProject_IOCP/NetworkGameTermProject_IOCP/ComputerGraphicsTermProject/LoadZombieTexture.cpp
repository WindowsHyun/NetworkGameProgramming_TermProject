#include "main.h"

void zombie_head_Texture(GLuint object[]) {
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Zombie/Head/Front.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Zombie/Head/Back.bmp");

	//왼쪽
	Load_TextureBMP(object, 2, "ImageData/Zombie/Head/Left.bmp");

	//오른쪽
	Load_TextureBMP(object, 3, "ImageData/Zombie/Head/Right.bmp");

	//위
	Load_TextureBMP(object, 4, "ImageData/Zombie/Head/Top.bmp");

	//아래
	Load_TextureBMP(object, 5, "ImageData/Zombie/Head/Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

void zombie_body_Texture(GLuint object[]) {
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Zombie/Body/Front.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Zombie/Body/Back.bmp");

	//왼쪽
	Load_TextureBMP(object, 2, "ImageData/Zombie/Body/Left.bmp");

	//오른쪽
	Load_TextureBMP(object, 3, "ImageData/Zombie/Body/Right.bmp");

	//위
	Load_TextureBMP(object, 4, "ImageData/Zombie/Body/Top.bmp");

	//아래
	Load_TextureBMP(object, 5, "ImageData/Zombie/Body/Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

void zombie_arm_Texture(GLuint object[], GLuint object2[]) {
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Zombie/Arm/Front_Top.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Zombie/Arm/Back_Top.bmp");

	// 왼쪽
	Load_TextureBMP(object, 2, "ImageData/Zombie/Arm/Left_Top.bmp");

	// 오른쪽
	Load_TextureBMP(object, 3, "ImageData/Zombie/Arm/Right_Top.bmp");

	// 위
	Load_TextureBMP(object, 4, "ImageData/Zombie/Arm/Top.bmp");

	// 아래
	Load_TextureBMP(object, 5, "ImageData/Zombie/Arm/Bottom.bmp");

	// 정면
	Load_TextureBMP(object2, 0, "ImageData/Zombie/Arm/Front_Bottom.bmp");

	// 후면
	Load_TextureBMP(object2, 1, "ImageData/Zombie/Arm/Back_Bottom.bmp");

	// 왼쪽
	Load_TextureBMP(object2, 2, "ImageData/Zombie/Arm/Left_Bottom.bmp");

	// 오른쪽
	Load_TextureBMP(object2, 3, "ImageData/Zombie/Arm/Right_Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

void zombie_leg_Texture(GLuint object[], GLuint object2[]) {
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Zombie/Leg/Front_Top.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Zombie/Leg/Back_Top.bmp");

	// 왼쪽
	Load_TextureBMP(object, 2, "ImageData/Zombie/Leg/Left_Top.bmp");

	// 오른쪽
	Load_TextureBMP(object, 3, "ImageData/Zombie/Leg/Right_Top.bmp");

	// 위
	Load_TextureBMP(object, 4, "ImageData/Zombie/Leg/Top.bmp");

	// 아래
	Load_TextureBMP(object, 5, "ImageData/Zombie/Leg/Bottom.bmp");

	// 정면
	Load_TextureBMP(object2, 0, "ImageData/Zombie/Leg/Front_Bottom.bmp");

	// 후면
	Load_TextureBMP(object2, 1, "ImageData/Zombie/Leg/Back_Bottom.bmp");

	// 왼쪽
	Load_TextureBMP(object2, 2, "ImageData/Zombie/Leg/Left_Bottom.bmp");

	// 오른쪽
	Load_TextureBMP(object2, 3, "ImageData/Zombie/Leg/Right_Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

