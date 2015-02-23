/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "globals.h"

/// Collision / Variables
enum MUSICTRACK { NONE, SET };
enum CHKMODE { 	CHK_NONE=0x00,
				CHK_X=0x01,
				CHK_Y=0x02,
				CHK_XY=CHK_X|CHK_Y,
				CHK_Z=0x04,
				CHK_XZ=CHK_X|CHK_Z,
				CHK_YZ=CHK_Y|CHK_Z,
				CHK_ALL = CHK_X | CHK_Y | CHK_Z
};
static bool gameover = false;

/// Movements
void move_x( float step,
			Tetris::glObjectMatrix & current_object,
			Tetris::glObjectMatrixList & cube_list );

void move_z( float step,
			Tetris::glObjectMatrix & current_object,
			Tetris::glObjectMatrixList & cube_list);

int check_xyz(
			CHKMODE m,
			Tetris::glObjectMatrix & current,
			Tetris::glObjectMatrixList & cube_list,
			float ystep = 0.0f );

bool check_gameover(
			Tetris::glObjectMatrix & current,
			float maxy );

MUSICTRACK calculate_objects(
			float ss,
			Tetris::glObjectMatrix & current_object,
			Tetris::glObjectMatrix & next_object,
			Tetris::glObjectMatrixList & cube_list);

#endif
