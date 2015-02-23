/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifndef __GLOBJECTMATRIX_H__
#define __GLOBJECTMATRIX_H__

#include "glObject.h"
#include "glCube.h"

#include <GL/gl.h>
#include <iostream>

using namespace std;

namespace Tetris
{

class glObjectMatrix : public glObject {
private:
	int _rows;
	int _columns;
	int _depth;
	int _id;
	glCubeList _cubes;
	cVec<float> _vDelta;

	void _swap( float * v1, float * v2 );

public:
	void _calculateEdgeValues();
	void _orderEdges();

public:
	glObjectMatrix();
	glObjectMatrix( const glObjectMatrix & obj );

	glObjectMatrix const& operator=( const glObjectMatrix& obj );
	bool operator==( glObjectMatrix & obj );

	inline glCubeList * cubes() { return & _cubes; }
	inline int cubeCount() { return _cubes.size(); }

	void rotateX( float angle );
	void rotateY( float angle );
	void rotateZ( float angle );

	void setCubeSectorArea( 	cVec<float> start,
											cVec<float> end,
		   									float delta );

	void updateSectors();

	void setColor( cColor color );
	void setMatrix( int * fields,
							int cols, int rows,
							int depth,
							cColor color,
							int id,
							GLuint texture_id = 0 );
	void draw( bool state = true );
	void drawHelp();
	void reset();

	void removeCubeX( cVec<int> sector );
	void removeCubeZ( cVec<int> sector );
	/// mode = 0 -> X
	/// mode = 1 -> Z
	void removeCube( cVec<int> sector, int mode );

	void updateCubeY( cVec<int> sector, int mode );

	inline int rows() { return _rows; }
	inline int columns() { return _columns; }
	inline int depth() { return _depth; }
	inline int id() { return _id; }

	void cubeInfo() {
		cout << "low:                    high:                      pos:" << endl;
		for( int i=0; i<cubeCount(); i++ )
			cout << cubes()->at(i).lowPosition() << " | "
					<< cubes()->at(i).highPosition() << " | "
					<< cubes()->at(i).position() << endl;
		cout << "---" << endl;
	}

	friend ostream & operator<<( ostream & os, glObjectMatrix & obj );
};

class glObjectMatrixList : public vector<glObjectMatrix> {
};

} /// namespace Tetris

#endif
