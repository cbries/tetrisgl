/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <Magick++.h>
#include <GL/gl.h>
#include <GL/glu.h>

/**
 * @brief Loads a chessboard in a OpenGL list.
 */
GLuint loadTexture( const char * filename );

#endif
