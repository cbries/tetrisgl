/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
 
#include "cColor.h"

#include <iostream>

float cColor::_farben[COLORCNT][4] = {
	{ 1.0f, 0.0f, 0.0f, 0.0f },	/// rot
	{ 0.0f, 1.0f, 0.0f, 0.0f },	/// gruen
	{ 0.0f, 0.0f, 1.0f, 0.0f },	/// blau
	{ 1.0f, 1.0f, 1.0f, 0.0f },	/// weiß
	{ 0.0f, 0.0f, 0.0f, 0.0f },	/// schwarz
	{ 0.4f, 0.0f, 0.0f, 0.0f },	/// brown
	{ 0.6f, 0.2f, 1.0f, 0.0f },	/// lila
	{ 1.0f, 1.0f, 0.0f, 0.0f }	/// gelb
};

cColor::cColor() {
	setRGBA( rand() % 1000 / 1000.0f,
					rand() % 1000 / 1000.0f,
					rand() % 1000 / 1000.0f, 1.0f );
}

cColor::cColor( float r, float g, float b, float a ) {
	setRGBA( r, g, b, a );
}

cColor::cColor( CCOLOR color ) {
	setColor(color);
}

cColor::cColor( const cColor & c ) {
	_r = c._r;
	_g = c._g;
	_b = c._b;
	_alpha = c._alpha;
}

void cColor::setRGBA( float r, float g, float b, float a ) {
	_r = r;
	_g = g;
	_b = b;
	_alpha = a;
}

void cColor::setColor( CCOLOR color ) {
	setRGBA(	_farben[color][0],
		_farben[color][1],
		_farben[color][2],
		_farben[color][3]
	);
}

void cColor::colors() {
	for( int i=0; i<COLORCNT; i++ ) {
		std::cout << " v( "
					<< _farben[COLORCNT][0] << " / "
					<< _farben[COLORCNT][1] << " / "
					<< _farben[COLORCNT][2] << " )" << std::endl;
	}
}
