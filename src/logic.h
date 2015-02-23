/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifndef __LOGIC_H__
#define __LOGIC_H__

#include "glObjectMatrix.h"

void remove_empty_objects( Tetris::glObjectMatrixList & cube_list );

void remove_line( 	Tetris::glObjectMatrix & current_object,
								Tetris::glObjectMatrixList & cube_list,
		 						cVecList<int> * x_list, cVecList<int> * z_list );

#endif
