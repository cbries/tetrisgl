/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#include "globals.h"
#include "glObjectMatrix.h"

#include <iostream>
#include <iomanip>

#include <GL/gl.h>

namespace Tetris {

/**
 * P R I V A T E
 */
void glObjectMatrix::_swap( float * v1, float * v2 ) {
	float v = *v1;
	*v1 = *v2;
	*v2 = v;
}

void glObjectMatrix::_calculateEdgeValues() {
	lowPosition() = _cubes[(int)cubeCount()/2].lowPosition();
	highPosition() = _cubes[(int)cubeCount()/2].highPosition();

	for( int i=0; i < cubeCount(); i++ ) {
		cVec<float> low = _cubes.at(i).lowPosition();
		cVec<float> high = _cubes.at(i).highPosition();

		if( low.x() < lowPosition().x() ) {
			lowPosition().setX(low.x());
		}
		if( high.x() > highPosition().x() ) {
			highPosition().setX(high.x());
		}

		if( low.y() < lowPosition().y() ) {
			lowPosition().setY(low.y());
		}
		if( high.y() > highPosition().y() ) {
			highPosition().setY(high.y());
		}

		if( low.z() < lowPosition().z() ) {
			lowPosition().setZ(low.z());
		}
		if( high.z() > highPosition().z() ) {
			highPosition().setZ(high.z());
		}
	}
}

void glObjectMatrix::_orderEdges() {
	for( int i=0; i < cubeCount(); i++ )
	{
		#define L _cubes.at(i).lowPosition()
		#define H _cubes.at(i).highPosition()

		if( L.x() > H.x() ) {
			float t = L.x();
			L.x() = L.x();
			L.x() = t;
		}

		if( L.y() > H.y() ) {
			float t = L.y();
			L.y() = L.y();
			L.y() = t;
		}

		if( L.z() > H.z() ) {
			float t = L.z();
			L.z() = L.x();
			L.z() = t;
		}

		#undef H
		#undef L
	}
}

/**
 * P U B L I C
 */
glObjectMatrix::glObjectMatrix() {
	reset();
}

glObjectMatrix::glObjectMatrix( const glObjectMatrix & obj )
	: glObject(obj),
		_rows(obj._rows), _columns(obj._columns),
		_depth(obj._depth), _id(obj._id),
		_cubes(obj._cubes), _vDelta(obj._vDelta) {
	if( this != &obj ) {
		this->glObject::operator=(obj);
	}
}

glObjectMatrix const&
glObjectMatrix::operator=( const glObjectMatrix& obj ) {
	if( this != &obj ) {
		this->glObject::operator=(obj);
	}
	_rows = obj._rows;
	_columns = obj._columns;
	_depth = obj._depth;
	_id = obj._id;
	_cubes = obj._cubes;
	_vDelta = obj._vDelta;
	return *this;
}

bool glObjectMatrix::operator==( glObjectMatrix & obj ) {
	if( objectId() == obj.objectId() ) {
		return true;
	}
	return false;
}

void glObjectMatrix::rotateX( float angle ) {
	float r = DEG2RAD(angle);
	bool move = false;

	for( int i=0; i<cubeCount(); i++ ) {
		cVec<float> v = _cubes.at(i).position();

		/// Rotation um die X-Achse,
		/// X ist konstant!!!
		float y = cos(r) * v.y() - sin(r) * v.z();
		float z = sin(r) * v.y() - cos(r) * v.z();

		if( fabs(z - (int) z) == 0.5f ) {
			move = true;
		}

		if( y < 0.1f && y > -0.1f ) {
			y = 0.0f;
		}

		if( z < 0.1f && z > -0.1f ) {
			z = 0.0f;
		}

		_cubes.at(i).position().setY(y);
		_cubes.at(i).position().setZ(z);
	}

	if( move ) {
		for( int i=0; i<cubeCount(); i++ ) {
			cVec<float> v = _cubes.at(i).position();
			_cubes.at(i).position().z() += 0.5f;;
		}
	}

	_calculateEdgeValues();
	_orderEdges();
}

void glObjectMatrix::rotateY( float angle ) {
	float r = DEG2RAD(angle);
	bool move = false;

	for( int i=0; i<cubeCount(); i++ ) {
		cVec<float> v = _cubes.at(i).position();

		/// Rotation um die Y-Achse,
		/// Y ist konstant!!!

		float x = cos(r) * v.x() - sin(r) * v.z();
		float z = sin(r) * v.x() - cos(r) * v.z();

		if( fabs(x - (int) x) == 0.5f ) {
			move = true;
		}

		if( x < 0.1f && x > -0.1f ) {
			x = 0.0f;
		}

		if( z < 0.1f && z > -0.1f ) {
			z = 0.0f;
		}

		_cubes.at(i).position().setX(x);
		_cubes.at(i).position().setZ(z);
	}

	if( move ) {
		for( int i=0; i<cubeCount(); i++ ) {
			cVec<float> v = _cubes.at(i).position();
			_cubes.at(i).position().x() += 0.5f;;
		}
	}

	_calculateEdgeValues();
	_orderEdges();
}

void glObjectMatrix::rotateZ( float angle ) {
	float r = DEG2RAD(angle);
	bool move = false;

	for( int i=0; i<cubeCount(); i++ ) {
		cVec<float> v = _cubes.at(i).position();

		/// Rotation um die Z-Achse,
		/// Z ist konstant!!!

		float x = cos(r) * v.x() - sin(r) * v.y();
		float y = sin(r) * v.x() - cos(r) * v.y();

		if( fabs(x - (int) x) == 0.5f ) {
			move = true;
		}

		if( x < 0.1f && x > -0.1f ) {
			x = 0.0f;
		}

		if( y < 0.1f && y > -0.1f ) {
			y  = 0.0f;
		}

		_cubes.at(i).position().setX(x);
		_cubes.at(i).position().setY(y);
	}

	if( move ) {
		for( int i=0; i<cubeCount(); i++ ) {
			cVec<float> v = _cubes.at(i).position();
			_cubes.at(i).position().x() += 0.5f;
		}
	}

	_calculateEdgeValues();
	_orderEdges();
}

void
glObjectMatrix::setCubeSectorArea(
		cVec<float> start,
		cVec<float> end,
		float delta ) {
	for( int i = 0; i < cubeCount(); i++ ) {
		cubes()->at(i).setSectorArea( start, end, delta );
	}
}

void glObjectMatrix::updateSectors() {
	for( int i = 0; i < cubeCount(); i++ ) {
		cubes()->at(i).setSector();
	}
}

void glObjectMatrix::setColor( cColor color ) {
	for( int i=0; i<cubeCount(); i++ ) {
		_cubes.at(i).bgcolor() = color;
	}
}

void glObjectMatrix::setMatrix(
		int * fields,
		int cols, int rows,
		int depth,
		cColor color,
		int id,
		GLuint texture_id ) {
	reset();

	_rows = rows;
	_columns = cols;
	_depth = depth;
	_id = id;
	bgcolor() = color;

	float L = 1.0f;

	int indexObj = 0;
	int xy = cols * rows;

	/**
	 * Calculates position of cubes.
	 */
	for( int z=0; z<depth; z++ ) {
		int x = 0;
		int y = 0;
		for( int i=0; i<xy; i++ ) {
			int index = i +(z * xy);

			y = i / cols;
			x = i - y * cols;

			/// Contains the field a 1, then their is
			/// a cube.
			if( fields[index] == 1 ) {
				float xv = x * L + (L / 2.0f);
				float yv = y * L + (L / 2.0f);
				float zv = z * L + (L / 2.0f);

				glCube c;

				c.position() = cVec<float>( xv, yv, zv );

				c.bgcolor() = color;

				c.lowPosition() = c.position() - c.lowPositionAbs();
				c.highPosition() = c.position() + c.highPositionAbs();

				c.setTextureId(texture_id);

				_cubes.push_back(c);
			}

		}	/// xy
	}	/// d


	/**
	 * Calculates the min and max values of a cube. The edges.
	 */
	_calculateEdgeValues();

	/**
	 * Calculates the midth of a cube.
	 */
	_vDelta.setX( (highPosition().x() - lowPosition().x()) / 2.0f );
	_vDelta.setY( (highPosition().y() - lowPosition().y()) / 2.0f);
	_vDelta.setZ( (highPosition().z() - lowPosition().z()) / 2.0f );

	/// Moves all cubes by a delta value,
	/// the midth is really the midth.
	for( int i=0; i < cubeCount(); i++ )
	{
		_cubes.at(i).lowPosition() 	= _cubes.at(i).lowPosition() - _vDelta;
		_cubes.at(i).highPosition() 	= _cubes.at(i).highPosition() - _vDelta;
		_cubes.at(i).position() 			= _cubes.at(i).position() - _vDelta;
	}

	draw();
}

void glObjectMatrix::draw( bool state ) {
	if(_cubes.size() <= 0) {
		return;
	}

	for( int i=0; i<cubeCount(); i++ ) {
		if( state ) {
			glColor3f( 1.0f, 0.0f, 1.0f );
		}

		/// Dieser Teil berechnet die Position der Cubes
		/// relativ zu dem Mittelpunkt des Objektes.
		/// Wenn dieser Teil fehlt, dann wird keine Animation
		/// des Objektes dargestellt.

		cVec<float> bck = _cubes.at(i).position();

		/// Berechnung der relativen Position des einzelnen Cubes
		/// zum Mittelpunkt der Formation.
		cVec<float> p;

		p = position() - _cubes.at(i).position();
		_cubes.at(i).position() = p;
		_cubes.at(i).setPoints();

		/// einzelne Cubes zeichnen
		if( state ) {
			glColor3f( 0.5f, 0.5f, 0.5f );
			_cubes.at(i).draw();
		}

		_cubes.at(i).position() = bck;
	}

	_calculateEdgeValues();
}

void glObjectMatrix::drawHelp() {
	glLineWidth(3);
	glColor3f( 1.0f, 0.0f, 0.0f );
	glBegin(GL_LINES);
	{
		glVertex3f( lowPosition().x(), 5.0f, lowPosition().z() );
		glVertex3f( lowPosition().x(), 0.0f, lowPosition().z() );

		glVertex3f( highPosition().x(), 5.0f, highPosition().z() );
		glVertex3f( highPosition().x(), 0.0f, highPosition().z() );

		glVertex3f( highPosition().x(), 5.0f, lowPosition().z() );
		glVertex3f( highPosition().x(), 0.0f, lowPosition().z() );

		glVertex3f( lowPosition().x(), 5.0f, highPosition().z() );
		glVertex3f( lowPosition().x(), 0.0f, highPosition().z() );
	}
	glEnd();
}

void glObjectMatrix::reset() {
	_rows = _columns = _depth = -1;
	_cubes.clear();
}

void glObjectMatrix::removeCubeX( cVec<int> sector ) {
	removeCube( sector, 0 );
}

void glObjectMatrix::removeCubeZ( cVec<int> sector ) {
	removeCube( sector, 1 );
}

void glObjectMatrix::removeCube( cVec<int> sector, int mode ) {
	glCubeList::iterator it = cubes()->begin();
	for( ; it != cubes()->end(); it++ ) {
		if( sector.y() == it->sector().y() &&
		 	 sector.z() == it->sector().z() &&
		  	 mode == 0) {
			cubes()->erase(it);
			return;
		}

		if( sector.y() == it->sector().y() &&
			 sector.x() == it->sector().x() &&
			 mode == 1) {
			cubes()->erase(it);
			return;
		}
	}
}

void glObjectMatrix::updateCubeY( cVec<int> sector, int mode ) {
	glCubeList::iterator it = cubes()->begin();
	for( ; it != cubes()->end(); it++ ) {
		if( it->sector().y() > sector.y() &&
    		it->sector().z() == sector.z() &&
		  	mode == 0 ) {

			it->lowPosition().y() += 1.0f;
			it->highPosition().y() += 1.0f;
			it->position().y() += 1.0f;
			it->sector().y() -= 1;
		}

		if( it->sector().y() > sector.y() &&
			it->sector().x() == sector.x() &&
			mode == 1 ) {

			it->lowPosition().y() += 1.0f;
			it->highPosition().y() += 1.0f;
			it->position().y() += 1.0f;
			it->sector().y() -= 1;
		}
	}
}

ostream & operator<<( ostream & os, glObjectMatrix & obj ) {
	return os;
}

} /// namespace Tetris
