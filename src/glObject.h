/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifndef __GLOBJECT_H__
#define __GLOBJECT_H__

#include <string>
using namespace std;

#include "cVec.h"
#include "cColor.h"

namespace Tetris {

static uint __glObjectCounter = 1;

class glObject {
private:
	uint _id;
	cVec<float> _position;
	cVec<float> _lowPosition;
	cVec<float> _highPosition;
	cVec<float> _direction;
	cVec<float> _rotation;
	cColor _bgcolor;
	cVecList<float> _points;
	uint _texture_id;
	bool _texture_show;

public:
	glObject();
	glObject( const glObject & obj );

	inline uint objectId() const 					{ return _id; }
	inline cVec<float> & position() 			{ return _position; }
	inline cVec<float> & lowPosition() 	{ return _lowPosition; }
	inline cVec<float> lowPositionAbs()
	{
		cVec<float> v( fabs(_lowPosition.x()),
								fabs(_lowPosition.y()),
								fabs(_lowPosition.z()) );
		return v;
	}
	inline cVec<float> & highPosition() 	{ return _highPosition; }
	inline cVec<float> highPositionAbs()
	{
		cVec<float> v( fabs(_highPosition.x()),
								fabs(_highPosition.y()),
								fabs(_highPosition.z()) );
		return v;
	}
	inline cVec<float> & direction() 	{ return _direction; }
	inline cVec<float> & rotation() 		{ return _rotation; }
	inline cColor & bgcolor() 				{ return _bgcolor; }
	inline cVecList<float> & points() 	{ return _points; }

	inline void setTextureId(uint id) {
		if( id != 0 ) {
			_texture_id = id;
			setTextureShow(true);
		}
	}
	inline uint textureId() const 			{ return _texture_id; }

	inline void setTextureShow(bool state) {
		_texture_show = state;
	}
	inline bool textureShow() { return _texture_show; }

	glObject const & operator=( const glObject & obj );
};

} /// namespace Tetris

#endif
