/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#include "glObject.h"

namespace Tetris {

glObject::glObject() {
	_id = __glObjectCounter;
	__glObjectCounter++;
	_texture_id = 0;
	_texture_show = false;
}

glObject::glObject( const glObject & obj ) {
	_id = obj._id;
	_position = obj._position;
	_lowPosition = obj._lowPosition;
	_highPosition = obj._highPosition;
	_direction = obj._direction;
	_rotation = obj._rotation;
	_bgcolor = obj._bgcolor;
	_points = obj._points;
	_texture_id = obj._texture_id;
	_texture_show = obj._texture_show;
}

glObject const & glObject::operator=( const glObject & obj ) {
	__glObjectCounter--;
	_id = obj._id;
	_position = obj._position;
	_lowPosition = obj._lowPosition;
	_highPosition = obj._highPosition;
	_direction = obj._direction;
	_rotation = obj._rotation;
	_bgcolor = obj._bgcolor;
	_points	= obj._points;
	_texture_id = obj._texture_id;
	_texture_show = obj._texture_show;
	return *this;
}

} /// namespace Tetris
