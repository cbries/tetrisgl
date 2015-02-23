/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#include "texture.h"

#include <iostream>

GLuint loadTexture( const char * filename ) {
	Magick::Image img;
	Magick::Blob blob;

	GLuint texture;
	glGenTextures( 1, &texture );

	try {
		img.read(filename);
		img.magick("RGBA");
		img.write(&blob);
	} catch(Magick::Error &error) {
		std::cout << "a Magick++ error occurred: " << error.what() << std::endl;
	}

	glEnable(GL_TEXTURE_2D);

	glBindTexture( GL_TEXTURE_2D, texture );
	/// GL_NEAREST
	/// GL_LINEAR
	/// GL_LINEAR_MIPMAP_LINEAR
	/// GL_NEAREST_MIPMAP_NEAREST
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
	/// GL_CLAMP_TO_EDGE
	/// GL_REPEAT
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	gluBuild2DMipmaps(
		GL_TEXTURE_2D,
		4,
		img.columns(), img.rows(),
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		blob.data() );

	glDisable(GL_TEXTURE_2D);

#ifdef DEBUG
	printf("texture id: %d\n", texture);
#endif
	return texture;
}
