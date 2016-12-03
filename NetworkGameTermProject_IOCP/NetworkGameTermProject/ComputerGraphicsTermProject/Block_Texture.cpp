#include "Texture_Load.h"

void nomal_Texture(GLuint object[]){
	// 정면
	Load_TextureBMP(object, 0,  "ImageData/Block/Nomal/Around.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Block/Nomal/Around.bmp");

	//왼쪽
	Load_TextureBMP(object, 2, "ImageData/Block/Nomal/Around.bmp");

	//오른쪽
	Load_TextureBMP(object, 3, "ImageData/Block/Nomal/Around.bmp");

	//위
	Load_TextureBMP(object, 4, "ImageData/Block/Nomal/Top.bmp");

	//아래
	Load_TextureBMP(object, 5, "ImageData/Block/Nomal/Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

void tree_Texture(GLuint object[]){
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Block/Tree/Around.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Block/Tree/Around.bmp");

	//왼쪽
	Load_TextureBMP(object, 2, "ImageData/Block/Tree/Around.bmp");

	//오른쪽
	Load_TextureBMP(object, 3, "ImageData/Block/Tree/Around.bmp");

	//위
	Load_TextureBMP(object, 4, "ImageData/Block/Tree/Top.bmp");

	//아래
	Load_TextureBMP(object, 5, "ImageData/Block/Tree/Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

void brick_Texture(GLuint object[]){
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Block/Brick/Around.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Block/Brick/Around.bmp");

	//왼쪽
	Load_TextureBMP(object, 2, "ImageData/Block/Brick/Around.bmp");

	//오른쪽
	Load_TextureBMP(object, 3, "ImageData/Block/Brick/Around.bmp");

	//위
	Load_TextureBMP(object, 4, "ImageData/Block/Brick/Top.bmp");

	//아래
	Load_TextureBMP(object, 5, "ImageData/Block/Brick/Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

void wooden_Texture(GLuint object[]){
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Block/Wooden/Around.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Block/Wooden/Around.bmp");

	//왼쪽
	Load_TextureBMP(object, 2, "ImageData/Block/Wooden/Around.bmp");

	//오른쪽
	Load_TextureBMP(object, 3, "ImageData/Block/Wooden/Around.bmp");

	//위
	Load_TextureBMP(object, 4, "ImageData/Block/Wooden/Top.bmp");

	//아래
	Load_TextureBMP(object, 5, "ImageData/Block/Wooden/Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}

void stone_Texture(GLuint object[]){
	// 정면
	Load_TextureBMP(object, 0, "ImageData/Block/Stone/Around.bmp");

	// 후면
	Load_TextureBMP(object, 1, "ImageData/Block/Stone/Around.bmp");

	//왼쪽
	Load_TextureBMP(object, 2, "ImageData/Block/Stone/Around.bmp");

	//오른쪽
	Load_TextureBMP(object, 3, "ImageData/Block/Stone/Around.bmp");

	//위
	Load_TextureBMP(object, 4, "ImageData/Block/Stone/Top.bmp");

	//아래
	Load_TextureBMP(object, 5, "ImageData/Block/Stone/Bottom.bmp");

	// 텍스처 모드 설정
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	// 텍스처 매핑 활성화
}