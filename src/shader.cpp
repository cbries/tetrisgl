/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#include "shader.h"
#include "textfile.h"

GLuint setShaders( std::string filename ) {
	GLuint v, f;
	char *vs = NULL,*fs = NULL,*fs2 = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexStr = filename + ".vert";
	std::string fragmentStr = filename + ".frag";

	vs = textFileRead( (char*)vertexStr.c_str() );
	fs = textFileRead( (char*)fragmentStr.c_str() );

	const char * ff = fs;
	const char * ff2 = fs2;
	const char * vv = vs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs); free(fs);

	glCompileShader(v);
	glCompileShader(f);

	GLuint p = glCreateProgram();

	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);

	return p;
}
