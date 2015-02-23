/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifndef __CCOLOR_H__
#define __CCOLOR_H__ 1

#include <stdlib.h>
#include <string>

#define COLORCNT 8

enum CCOLOR {
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_WHITE,
	COLOR_BLACK,
	COLOR_BROWN,
	COLOR_LILA,
	COLOR_YELLOW
};

/**
 * A class to hold information of colors.
 * @author Christian Benjamin Ries
 * @date 07. July 2008
 */
class cColor
{
private:
	float _r, _g, _b, _alpha;
	static float _farben[COLORCNT][4];

public:
	cColor();
	cColor( float r, float g, float b, float a = 0.0f );
	cColor( CCOLOR color );
	cColor( const cColor & c );

	void setRGBA( float r, float g, float b, float a );
	void setColor( CCOLOR color );
	void colors();

	inline float r() { return _r; }
	inline float g() { return _g; }
	inline float b() { return _b; }
	inline float alpha() { return _alpha; }
};

#endif
