#define _CRT_SECURE_NO_WARNINGS
#include "Texture_Load.h"

AUX_RGBImageRec *LoadBMP(char *Filename){				// Loads A Bitmap Image
	FILE *File = NULL;									// File Handle
	if (!Filename){									// Make Sure A Filename Was Given
		return NULL;									// If Not Return NULL
	}
	File = fopen(Filename, "r");							// Check To See If The File Exists
	if (File){											// Does The File Exist?
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}
	return NULL;										// If Load Failed Return NULL
}

void Load_TextureBMP(GLuint object[], int num, char *imagedata){
	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture
	memset(TextureImage, 0, sizeof(void *) * 1);           	// Set The Pointer To NULL
	if (TextureImage[0] = LoadBMP(imagedata)){
		glGenTextures(1, &object[num]);					// Create The Texture
		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, object[num]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	if (TextureImage[0]){									// If Texture Exists
		if (TextureImage[0]->data){							// If Texture Image Exists
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}
		free(TextureImage[0]);								// Free The Image Structure
	}
}