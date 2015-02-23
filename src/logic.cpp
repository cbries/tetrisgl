/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#include "globals.h"
#include "logic.h"

void remove_empty_objects( Tetris::glObjectMatrixList & cube_list ) {
	Tetris::glObjectMatrixList::iterator sit = cube_list.begin();
	Tetris::glObjectMatrixList::iterator eit = cube_list.end();

	for( ; sit != eit; sit++ ) {
		if( sit->cubeCount() <= 0 ) {
			cube_list.erase(sit);

			/// Iteratoren neu setzen, da nun einer fehlt und
			/// sonst nicht alle richtig leeren gefunden werden!
			sit = cube_list.begin();
			eit = cube_list.end();
		}
	}
}

void remove_line( Tetris::glObjectMatrix & current_object,
				  Tetris::glObjectMatrixList & cube_list,
	   			  cVecList<int> * x_ret_list, cVecList<int> * z_ret_list
		   		) {
	/// Letzten gesetzten Cube durchgehen.
	Tetris::glCubeList::iterator cc_it = current_object.cubes()->begin();
	for( ; cc_it != current_object.cubes()->end(); cc_it++ ) {

#define CCSEC cc_it->sector()

		cVecList<int> x_list;
		cVecList<int> z_list;

		/// Alle bisher gesetzten Cubes durchgehen und deren ...
		Tetris::glObjectMatrixList::iterator cl_it = cube_list.begin();
		for( ; cl_it != cube_list.end(); cl_it++ ) {

			/// ... einzelene Cubes auf Achsengleichheit überprüfen.
			for( int i = 0; i < cl_it->cubeCount(); i++ ) {
#define CLSEC cl_it->cubes()->at(i).sector()
				/// Auf der X-Achse überprüfen.
				if( CLSEC.y() == CCSEC.y() &&
					CLSEC.z() == CCSEC.z() ) {
						x_list.push_back(CLSEC);
				}
				/// Auf der Z-Achse überprüfen.
				if( CLSEC.y() == CCSEC.y() &&
					CLSEC.x() == CCSEC.x() ) {
						z_list.push_back(CLSEC);
				}
#undef CLSEC
			}
		}

		if( x_list.size() == 10 ) {
			x_ret_list->assign( x_list.begin(), x_list.end() );

			cl_it = cube_list.begin();
			Tetris::glObjectMatrixList::iterator cl_it_end = cube_list.end();
			for( ; cl_it != cl_it_end; cl_it++ ) {
					for( int ii = 0; ii < x_list.size(); ii++ ) {
						cl_it->removeCubeX(x_list.at(ii));
					}
			}

			if( current_object.cubeCount() <= 0 ) {
				return;
			}
		}

		if( z_list.size() == 10 ) {
			z_ret_list->assign( z_list.begin(), z_list.end() );

			cl_it = cube_list.begin();
			Tetris::glObjectMatrixList::iterator cl_it_end = cube_list.end();
			for( ; cl_it != cl_it_end; cl_it++ ) {
				for( int ii = 0; ii < z_list.size(); ii++ ) {
					cl_it->removeCubeZ(z_list.at(ii));
				}
			}
		}
#undef CCSEC
	}
}
