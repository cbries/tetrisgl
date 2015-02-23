/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#include "movement.h"

#include "forms.h"

void move_x(
		float step,
		Tetris::glObjectMatrix & current_object,
		Tetris::glObjectMatrixList & cube_list ) {

	Tetris::glObjectMatrix objTmp(current_object);

	Tetris::glCubeList * c = objTmp.cubes();

	for( int i=0; i<objTmp.cubeCount(); i++ ) {
		c->at(i).lowPosition().x() = current_object.cubes()->at(i).lowPosition().x() + step;
		c->at(i).highPosition().x() = current_object.cubes()->at(i).highPosition().x() + step;

		c->at(i).lowPosition().y() = current_object.cubes()->at(i).lowPosition().y();
		c->at(i).highPosition().y() = current_object.cubes()->at(i).highPosition().y();

		c->at(i).lowPosition().z() = current_object.cubes()->at(i).lowPosition().z();
		c->at(i).highPosition().z() = current_object.cubes()->at(i).highPosition().z();
	}

	int ret = check_xyz( CHK_ALL, objTmp, cube_list );

	if( step < 0 ) {
		if( current_object.lowPosition().x() + step < PLAYGROUND_STARTX || ret == CHK_ALL ) {
			return;
		}
	} else {
		if( current_object.highPosition().x() + step > PLAYGROUND_ENDX || ret == CHK_ALL ) {
			return;
		}
	}

	current_object.lowPosition().x() += step;
	current_object.highPosition().x() += step;
	current_object.position().x() += step;
}

void move_z(
		float step,
		Tetris::glObjectMatrix & current_object,
		Tetris::glObjectMatrixList & cube_list ) {

	Tetris::glObjectMatrix objTmp = current_object;
	Tetris::glCubeList * c = objTmp.cubes();
	for( int i=0; i<objTmp.cubeCount(); i++ ) {
		c->at(i).lowPosition().x() = current_object.cubes()->at(i).lowPosition().x();
		c->at(i).highPosition().x() = current_object.cubes()->at(i).highPosition().x();

		c->at(i).lowPosition().y() = current_object.cubes()->at(i).lowPosition().y();
		c->at(i).highPosition().y() = current_object.cubes()->at(i).highPosition().y();

		c->at(i).lowPosition().z() = current_object.cubes()->at(i).lowPosition().z() + step;
		c->at(i).highPosition().z() = current_object.cubes()->at(i).highPosition().z() + step;
	}

	int ret = check_xyz( CHK_ALL, objTmp, cube_list );

	if( step < 0 ) {
		if( current_object.lowPosition().z() + step < PLAYGROUND_STARTZ || ret == CHK_ALL ) {
			return;
		}
	} else {
		if( current_object.highPosition().z() + step > PLAYGROUND_ENDZ || ret ==  CHK_ALL ) {
			return;
		}
	}

	current_object.lowPosition().z() += step;
	current_object.highPosition().z() += step;
	current_object.position().z() += step;
}

int check_xyz(
		CHKMODE m,
		Tetris::glObjectMatrix & current,
		Tetris::glObjectMatrixList & cube_list,
		float ystep ) {

	int ret = CHK_NONE;

	#define V 0.02f
	for( int i = 0; i < current.cubeCount(); i++ ) {
		#define CURRENT current.cubes()->at(i)

		float x_c_low = CURRENT.lowPosition().x();
		float y_c_low = CURRENT.lowPosition().y();
		float z_c_low = CURRENT.lowPosition().z();

		float x_c_high = CURRENT.highPosition().x();
		float y_c_high = CURRENT.highPosition().y();
		float z_c_high = CURRENT.highPosition().z();

		/// TODO trickige Stelle die viel Mist machen kann!
		/// Wenn nur folgende Bedingung verwendet wird,
		///		y_c_low < 0.0f
		/// dann funktioniert u.a. die Sektoren Berechnung nicht mehr!
		if( y_c_low <= 0.0f ) {
			return CHK_ALL;
		}

		x_c_low += V;	x_c_high -= V;
		y_c_low += V;	y_c_high -= V;
		z_c_low += V; 	z_c_high -= V;

		vector<Tetris::glObjectMatrix>::iterator it = cube_list.begin();
		for( ; it != cube_list.end(); it++ ) {
			for( int j = 0; j < it->cubeCount(); j++ ) {
				#define CHECK it->cubes()->at(j)

				if( m & CHK_X ) {
					float x_low = CHECK.lowPosition().x();
					float x_high = CHECK.highPosition().x();

					if( x_c_low >= x_low && x_c_low <= x_high ) {
						ret |= CHK_X;
					}
				}

				if( m & CHK_Z ) {
					float z_low = CHECK.lowPosition().z();
					float z_high = CHECK.highPosition().z();

					if( z_c_low >= z_low && z_c_low <= z_high ) {
						ret |= CHK_Z;
					}
				}

				float dy = 0.0f;

				if( m & CHK_Y ) {
					float y_low = CHECK.lowPosition().y();
					float y_high = CHECK.highPosition().y();

					dy = y_c_low - y_high;

					float dd = y_low - y_c_high;

					if( dy < fabs(ystep) && dd < 0.0f ) {
						ret |= CHK_Y;
					}
				}

				#undef CHECK

				if( ret == m ) {
					for( int index = 0; index < current.cubeCount(); index++ ) {
#define L current.cubes()->at(index).lowPosition()
#define H current.cubes()->at(index).highPosition()
#define P current.cubes()->at(index).position()
							L.y()	+= dy - 1.0f;
							H.y()	+= dy - 1.0f;
							P.y()	+= dy;
#undef P
#undef H
#undef L
					}
					return ret;
				} else {
					ret = CHK_NONE;
				}
			}
		}
		#undef CURRENT
	}
	#undef V
	return ret;
}

bool check_gameover( Tetris::glObjectMatrix & current_object,
					float maxy ) {
	for( int i=0; i<current_object.cubeCount(); i++ ) {
		if( current_object.cubes()->at(i).highPosition().y() >= maxy ) {
			return true;
		}
	}
	return false;
}

MUSICTRACK calculate_objects(
	float ss,
	Tetris::glObjectMatrix & current_object,
	Tetris::glObjectMatrix & next_object,
	Tetris::glObjectMatrixList & cube_list) {

	int ret = check_xyz( CHK_ALL, current_object, cube_list, ss );

	if( ret == CHK_ALL ) {
		float dy = current_object.lowPosition().y();
		if( dy < 0.0f ) {
			Tetris::glCubeList::iterator sit = current_object.cubes()->begin();
			Tetris::glCubeList::iterator eit = current_object.cubes()->end();

			for( ; sit != eit; sit++ ) {
				sit->lowPosition().y() += dy;
				sit->highPosition().y() += dy;
				sit->position().y() += dy;
			}
		}

		if(play_music_set == false ) {
			return SET;
		} else {
			return NONE;
		}
	} else {
		current_object.position().y() += ss;
	}

	return NONE;
}
