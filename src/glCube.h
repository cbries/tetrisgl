/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifndef __GLCUBE_H__
#define __GLCUBE_H__

#include "glObject.h"

namespace Tetris {

class glCube : public glObject {
private:
	float _length;
	cVec<int> _sector;
	cVec<float> _sectorStart;
	cVec<float>	_sectorEnd;
	float _sectorDelta;

public:
	glCube();
	glCube( const glCube & cube );

	glCube const & operator=( const glCube & obj );

	void setLength( float length = 1.0f ) {
		_length = length;
	}
	float length() { return _length; }

	void setSector();
	inline cVec<int> & sector() { return _sector; }
	void setSectorArea( 	cVec<float> start,
									cVec<float> end,
								  	float delta );

	void setPoints();
	virtual void draw();
};

class glCubeList : public vector<glCube> {
};

} /// namespace Tetris

#endif
