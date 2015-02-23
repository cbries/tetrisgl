/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#include "glCube.h"
#include <GL/gl.h>

namespace Tetris {

glCube::glCube() {
	setLength(1.0f);
	setPoints();
}

glCube::glCube( const glCube & cube )
	: glObject(cube) {
	_length = cube._length;
	_sector	= cube._sector;
	_sectorStart = cube._sectorStart;
	_sectorEnd = cube._sectorEnd;
	_sectorDelta = cube._sectorDelta;
	if( this != &cube ) {
		this->glObject::operator=(cube);
	}
	setPoints();
}

glCube const & glCube::operator=( const glCube & obj ) {
	_length = obj._length;
	_sector	= obj._sector;
	_sectorStart = obj._sectorStart;
	_sectorEnd = obj._sectorEnd;
	_sectorDelta = obj._sectorDelta;
	if( this != &obj ) {
		this->glObject::operator=(obj);
	}
	setPoints();
	return *this;
}

void glCube::setSector() {
	float ymitte = lowPosition().y() + (highPosition().y() - lowPosition().y()) / 2.0f;
	_sector.set( highPosition().x(), roundf(ymitte) + 1 /*yindex + 1*/, highPosition().z() );
}

void
glCube::setSectorArea( 	cVec<float> start,
						cVec<float> end,
						float delta ) {
	_sectorStart = start;
	_sectorEnd = end;
	_sectorDelta = delta;
}

void glCube::draw() {
	glPushMatrix();
	glTranslatef( position().x(), position().y(), position().z() );

	bool show_texture;
	glGetBooleanv( GL_TEXTURE_2D, (GLboolean*)&show_texture );

	int id_texture;
	glGetIntegerv( GL_TEXTURE_BINDING_2D, &id_texture );

	if( textureShow() ) {
		glColor3f( 1.0f, 1.0f, 1.0f );
		glBindTexture( GL_TEXTURE_2D, textureId() );
		glEnable(GL_TEXTURE_2D);
	} else {
		glColor3f( bgcolor().r(), bgcolor().g(), bgcolor().b() );
	}

	#define T1() glTexCoord2f(0.0f, 0.0f)
	#define T2() glTexCoord2f(0.0f, 1.0f)
	#define T3() glTexCoord2f(1.0f, 1.0f)
	#define T4() glTexCoord2f(1.0f, 0.0f)

	#define p(i) points().at(i).xyz()
	glBegin( GL_QUADS );
	{
		/// Vorne
		glNormal3f( 0.0f, 0.0f, 1.0f );
		T1(); glVertex3fv( p(1) );
		T2(); glVertex3fv( p(2) );
		T3(); glVertex3fv( p(3) );
		T4(); glVertex3fv( p(0) );

		/// Hinten
		glNormal3f( 0.0f, 0.0f, -1.0f );
		T1(); glVertex3fv( p(6) );
		T2(); glVertex3fv( p(5) );
		T3(); glVertex3fv( p(4) );
		T4(); glVertex3fv( p(7) );

		/// Links
		glNormal3f( -1.0f, 0.0f, 0.0f );
		T1(); glVertex3fv( p(5) );
		T2(); glVertex3fv( p(1) );
		T3(); glVertex3fv( p(0) );
		T4(); glVertex3fv( p(4) );

		/// Rechts
		glNormal3f( 1.0f, 0.0f, 0.0f );
		T1(); glVertex3fv( p(2) );
		T2(); glVertex3fv( p(6) );
		T3(); glVertex3fv( p(7) );
		T4(); glVertex3fv( p(3) );

		/// Oben
		glNormal3f( 0.0f, 1.0f, 0.0f );
		T1(); glVertex3fv( p(0) );
		T2(); glVertex3fv( p(3) );
		T3(); glVertex3fv( p(7) );
		T4(); glVertex3fv( p(4) );

		/// Unten
		glNormal3f( 0.0f, -1.0f, 0.0f );
		T1(); glVertex3fv( p(2) );
		T2(); glVertex3fv( p(1) );
		T3(); glVertex3fv( p(5) );
		T4(); glVertex3fv( p(6) );
	}
	glEnd();
	#undef p

	#undef T1
	#undef T2
	#undef T3
	#undef T4

	glPopMatrix();

	glBindTexture( GL_TEXTURE_2D, id_texture );
	if(show_texture) {
		glEnable(GL_TEXTURE_2D);
	} else {
		glDisable(GL_TEXTURE_2D);
	}
}

void glCube::setPoints() {
	float s = length() / 2.0f;

	lowPosition().set( position().x() - s, position().y() - s, position().z() - s );
	highPosition().set( position().x() + s, position().y() + s, position().z() + s );

	points().clear();

	#define p( x, y, z ) points().push_back( cVec<float>(x, y, z) )
	p( -s, s, s );		/// 0
	p( -s, -s, s );		/// 1
	p( s, -s, s );		/// 2
	p( s, s, s );		/// 3
	p( -s, s, -s );		/// 4
	p( -s, -s, -s );		/// 5
	p( s, -s, -s );		/// 6
	p( s, s, -s ); 		/// 7
	#undef p
}

} /// namespace Tetris
