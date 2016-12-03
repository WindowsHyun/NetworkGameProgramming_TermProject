#include "Texture_Load.h"

void head_Texture(GLuint object[]){
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Steave/Head/Front.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Steave/Head/Back.bmp");

	//왼쪽
	Load_TextureBMP(object, 2, "ImageData/Steave/Head/Left.bmp");

	//오른쪽
	Load_TextureBMP(object, 3, "ImageData/Steave/Head/Right.bmp");

	//위
	Load_TextureBMP(object, 4, "ImageData/Steave/Head/Top.bmp");

	//아래
	Load_TextureBMP(object, 5, "ImageData/Steave/Head/Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

void body_Texture(GLuint object[]){
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Steave/Body/Front.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Steave/Body/Back.bmp");

	//왼쪽
	Load_TextureBMP(object, 2, "ImageData/Steave/Body/Left.bmp");

	//오른쪽
	Load_TextureBMP(object, 3, "ImageData/Steave/Body/Right.bmp");

	//위
	Load_TextureBMP(object, 4, "ImageData/Steave/Body/Top.bmp");

	//아래
	Load_TextureBMP(object, 5, "ImageData/Steave/Body/Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

void arm_Texture(GLuint object[], GLuint object2[]){
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Steave/Arm/Front_Top.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Steave/Arm/Back_Top.bmp");

	// 왼쪽
	Load_TextureBMP(object, 2, "ImageData/Steave/Arm/Left_Top.bmp");

	// 오른쪽
	Load_TextureBMP(object, 3, "ImageData/Steave/Arm/Right_Top.bmp");

	// 위
	Load_TextureBMP(object, 4, "ImageData/Steave/Arm/Top.bmp");

	// 아래
	Load_TextureBMP(object, 5, "ImageData/Steave/Arm/Bottom.bmp");

	// 정면
	Load_TextureBMP(object2, 0, "ImageData/Steave/Arm/Front_Bottom.bmp");

	// 후면
	Load_TextureBMP(object2, 1, "ImageData/Steave/Arm/Back_Bottom.bmp");

	// 왼쪽
	Load_TextureBMP(object2, 2, "ImageData/Steave/Arm/Left_Bottom.bmp");

	// 오른쪽
	Load_TextureBMP(object2, 3, "ImageData/Steave/Arm/Right_Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

void leg_Texture(GLuint object[], GLuint object2[]){
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Steave/Leg/Front_Top.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Steave/Leg/Back_Top.bmp");

	// 왼쪽
	Load_TextureBMP(object, 2, "ImageData/Steave/Leg/Left_Top.bmp");

	// 오른쪽
	Load_TextureBMP(object, 3, "ImageData/Steave/Leg/Right_Top.bmp");

	// 위
	Load_TextureBMP(object, 4, "ImageData/Steave/Leg/Top.bmp");

	// 아래
	Load_TextureBMP(object, 5, "ImageData/Steave/Leg/Bottom.bmp");

	// 정면
	Load_TextureBMP(object2, 0, "ImageData/Steave/Leg/Front_Bottom.bmp");

	// 후면
	Load_TextureBMP(object2, 1, "ImageData/Steave/Leg/Back_Bottom.bmp");

	// 왼쪽
	Load_TextureBMP(object2, 2, "ImageData/Steave/Leg/Left_Bottom.bmp");

	// 오른쪽
	Load_TextureBMP(object2, 3, "ImageData/Steave/Leg/Right_Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}
