/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#include "globals.h"
#include "playground.h"
#include "texture.h"

GLuint loadPlayground( GLuint tid ) {
	glBindTexture( GL_TEXTURE_2D, tid );

	GLuint dl = glGenLists(1);

	glNewList( dl, GL_COMPILE );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	glColor4f( 0.5f, 0.5f, 0.5f, 0.95f );

	glMaterialfv( GL_FRONT, GL_AMBIENT, mat_chrom_ambient );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_chrom_diffus );
	glMaterialfv( GL_FRONT, GL_SPECULAR, mat_chrom_specular );
	glMaterialfv( GL_FRONT, GL_SHININESS, &mat_chrom_shininess );

	glBegin( GL_QUADS );
	{
		glNormal3f( 0.0f, 1.0f, 0.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f(	PLAYGROUND_STARTX, PLAYGROUND_STARTY, PLAYGROUND_STARTZ );
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f(	PLAYGROUND_STARTX, PLAYGROUND_STARTY, PLAYGROUND_ENDZ );
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f( PLAYGROUND_ENDX, PLAYGROUND_STARTY, PLAYGROUND_ENDZ );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( PLAYGROUND_ENDX, PLAYGROUND_STARTY, PLAYGROUND_STARTZ );
	}
	glEnd();

	glDisable(GL_BLEND);

	glEndList();

	glEnable( GL_TEXTURE_2D );

	return dl;
}
