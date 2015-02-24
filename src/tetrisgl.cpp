/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <iostream>
#include <stdlib.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <cwiid.h>

using namespace std;

#include "globals.h"
#include "glInfo.h"

static char fontpath[] = "/usr/local/share/tetrisgl/fonts/SF Collegiate Solid Bold.ttf";
static char win_title[30] = { 0 };
static int win_width = WIN_WIDTH;
static int win_height = WIN_HEIGHT;

void game_reset();

/// Time functions / -variables
time_t time_start;
time_t time_end;

/// OpenGL
void setup_opengl( int w, int h );

/// tetris.cpp functions
void current_next_cube( Tetris::glObjectMatrix & current_object,
						Tetris::glObjectMatrix & next_object,
						Tetris::glObjectMatrixList & cube_list );

/// current matrix object
Tetris::glObjectMatrix current_object;
Tetris::glObjectMatrix next_object;
/// list of all matrix objects
Tetris::glObjectMatrixList cube_list;

GLuint playgroundList;

/// Wii Controller
int cwiid_init();
void cwiid_callback( cwiid_wiimote_t *wiimote,
								int msg_count,
								union cwiid_mesg mesg_array[],
								struct timespec *timestamp );
void cwiid_btn( struct cwiid_btn_mesg *mesg );
void cwiid_acc( struct cwiid_acc_mesg *mesg );
void cwiid_nunchuk( struct cwiid_nunchuk_mesg *mesg );

static uint8_t battery_value = 0.0f;
static cwiid_wiimote_t *wiimote = NULL;
static bdaddr_t * bdaddr = BDADDR_ANY;
static struct acc_cal wm_cal;

struct stick {
	char valid;
	uint8_t x;
	uint8_t y;
	uint8_t max;
};

static struct stick nc_stick;

/**
 * Initializes the wii controller connection.
 * @return 0 when the connection is established, otherwise 1
 */
int cwiid_init() {
	char reset_bdaddr = 0;

	if( bacmp( bdaddr, BDADDR_ANY ) == 0 ) {
		reset_bdaddr = 1;
	}

	cout << "Put Wiimote in discoverable mode (press 1+2) and press ENTER" << endl;

	getc(stdin);

	if( (wiimote = cwiid_open( bdaddr, CWIID_FLAG_MESG_IFC)) == NULL) {
		cout << "Unable to connect: No connection!" << endl;
		return -1;
	} else if( cwiid_set_mesg_callback(wiimote, &cwiid_callback)) {
		if( cwiid_close(wiimote)) {
			cout << "Error on disconnect!" << endl;
		}
		wiimote = NULL;
		cout << "No connection!" << endl;
		return -1;
	} else {
		cout << "Connected!" << endl;
		if( cwiid_get_acc_cal(wiimote, CWIID_EXT_NONE, &wm_cal)) {
			cout << "Unable to retrieve accelerometer!" << endl;
		}
		cwiid_request_status(wiimote);
	}

	if( reset_bdaddr ) {
		bdaddr = BDADDR_ANY;
	}

	return 0;
}

/**
 * Callback functions which is executed on events by
 * the Nintendo Wii controller.
 * @param wiimote
 * @param msg_count
 * @param mesg_array
 * @param timestamp
 * @see http://abstrakraft.org/cwiid/wiki/libcwiid
 */
void cwiid_callback(
	cwiid_wiimote_t *wiimote,
	int msg_count,
	union cwiid_mesg mesg_array[],
	struct timespec *timestamp ) {

	char battery[14];
	char *ext_str = NULL;
	static enum cwiid_ext_type ext_type = CWIID_EXT_NONE;

	for( int i=0; i<msg_count; i++ ) {
		switch(mesg_array[i].type) {
			case CWIID_MESG_STATUS: {
				snprintf( battery, 14, "Battery: %d%%",
						(int) (100.0f * mesg_array[i].status_mesg.battery / CWIID_BATTERY_MAX));
				cout << "Battery: " << battery << endl;
			} break;

			case CWIID_MESG_BTN: {
				cwiid_btn(&mesg_array[i].btn_mesg);
			} break;

			case CWIID_MESG_ACC: {
				cwiid_acc(&mesg_array[i].acc_mesg);
			} break;

			case CWIID_MESG_NUNCHUK: {
				cwiid_nunchuk(&mesg_array[i].nunchuk_mesg);
			} break;

			case CWIID_MESG_CLASSIC: {
			} break;

			case CWIID_MESG_ERROR: {
			} break;

			default:
				break;
		}
	} /// for 0 to msg_count
}

/**
 * Buttons:
 * CWIID_BTN_LEFT
 * CWIID_BTN_RIGHT
 * CWIID_BTN_UP
 * CWIID_BTN_DOWN
 * CWIID_BTN_HOME
 * CWIID_BTN_A
 * CWIID_BTN_B
 * CWIID_BTN_MINUS
 * CWIID_BTN_PLUS
 * CWIID_BTN_1
 * CWIID_BTN_2
 * @param mesg
 * @see http://abstrakraft.org/cwiid/wiki/libcwiid
 */
void cwiid_btn( struct cwiid_btn_mesg *mesg )
{
	if( mesg->buttons & CWIID_BTN_2 && !gameover ) {
		paused = !paused;
	}
	if( mesg->buttons & CWIID_BTN_2 && gameover ) {
		game_reset();
	}

	if( mesg->buttons & CWIID_BTN_HOME ) {
		run = false;
	}

	if( paused ) {
		return;
	}

	if( mesg->buttons & CWIID_BTN_LEFT && !(mesg->buttons & CWIID_BTN_B) ) {
		rotate_next_z = -90.0f;
	}
	if( mesg->buttons & CWIID_BTN_RIGHT && !(mesg->buttons & CWIID_BTN_B) ) {
		rotate_next_z = 90.0f;
	}
	if( mesg->buttons & CWIID_BTN_DOWN && !(mesg->buttons & CWIID_BTN_B) ) {
		rotate_next_x = -90.0f;
	}
	if( mesg->buttons & CWIID_BTN_UP && !(mesg->buttons & CWIID_BTN_B) ) {
		rotate_next_x = 90.0f;
	}

	if( mesg->buttons & CWIID_BTN_A ) {
		rotate_next_y = -90.0f;
	}

	if( mesg->buttons & CWIID_BTN_1 ) {
		gl_show_wireframe = true;
	} else {
		gl_show_wireframe = false;
	}
}

/**
 *
 */
static bool acc_move_x = false;
static int acc_move_x_counter = 0;
static bool acc_move_z = false;
static int acc_move_z_counter = 0;

/**
 * Accelerator sensoric callback.
 * @param mesg
 * @see http://abstrakraft.org/cwiid/wiki/libcwiid
 */
void cwiid_acc( struct cwiid_acc_mesg *mesg ) {
	if( paused ) {
		return;
	}

	/// Normale Werte des Beschleunigungssensors
	/// x = 125
	/// y = 125
	/// z = 155
	/// Das maximale Delta für die Kippung in unserem Fall
	/// beträgt: 25
	/// Es soll schnell reagieren, in diesem Fall wird ein Delta
	/// von 10 verwendet.
	/// Nur die x und y Werte kommen in Betracht. x bewegt
	/// die Steine auf der X-Achse im Spiel. y bewegt die
	/// die Steine auf der Z-Achse im Spiel.

	#define STEPCOUNTER 5

	#define DELTA 10
	#define X_NORMAL 125
	#define Y_NORMAL 125

	#define X_MIN 		X_NORMAL - DELTA
	#define X_MAX 		X_NORMAL + DELTA
	#define Y_MIN 		Y_NORMAL - DELTA
	#define Y_MAX 		Y_NORMAL + DELTA

	if( acc_move_x == false ) {
		if( mesg->acc[CWIID_X] <= X_MIN ) {
			acc_move_x = true;
			if( acc_move_x_counter == STEPCOUNTER )	{
				move_x( -1.0f, current_object, cube_list );
				acc_move_x_counter = 0;
			} else {
				acc_move_x_counter++;
			}
		} else if( mesg->acc[CWIID_X] >= X_MAX )
		{
			acc_move_x = true;
			if( acc_move_x_counter == STEPCOUNTER ) {
				move_x( 1.0f, current_object, cube_list );
				acc_move_x_counter = 0;
			} else {
				acc_move_x_counter++;
			}
		}
	} else {
		acc_move_x = false;
	}

	if( acc_move_z == false ) {
		if( mesg->acc[CWIID_Y] <= Y_MIN ) {
			acc_move_z = true;
			if( acc_move_z_counter == STEPCOUNTER ) {
				move_z( 1.0f, current_object, cube_list );
				acc_move_z_counter = 0;
			} else {
				acc_move_z_counter++;
			}
		} else if( mesg->acc[CWIID_Y] >= Y_MAX ) {
			acc_move_z = true;
			if( acc_move_z_counter == STEPCOUNTER ) {
				move_z( -1.0f, current_object, cube_list );
				acc_move_z_counter = 0;
			} else {
				acc_move_z_counter++;
			}
		}
	} else {
		acc_move_z = false;
	}

	#undef DELTA
	#undef X_NORMAL
	#undef Y_NORMAL
	#undef X_MIN
	#undef X_MAX
	#undef Y_MIN
	#undef Y_MAX
}

/**
 * Buttons:
 * CWIID_NUNCHUK_BTN_C
 * CWIID_NUNCHUK_BTN_Z
 */
static bool nunchuk_btn_c_pressed = false;
static bool nunchuk_btn_z_pressed = false;

/**
 * Callback for the nunchuck controller.
 * @param mesg
 * @see http://abstrakraft.org/cwiid/wiki/libcwiid
 */
void cwiid_nunchuk( struct cwiid_nunchuk_mesg *mesg ) {
	if( paused ) {
		return;
	}

	double a_x, a_y, a_z, a;
	double roll, pitch;

	/// Increase the falling velocity
	if( mesg->buttons & CWIID_NUNCHUK_BTN_C ) {
		if( nunchuk_btn_c_pressed == false ) {
			fall_velocity_factor_bck = fall_velocity_factor;
			fall_velocity_factor = 10.0f;
			nunchuk_btn_c_pressed = true;
		}
	} else {
		nunchuk_btn_c_pressed = false;
		fall_velocity_factor = fall_velocity_factor_bck;
	}
	/// Rotate the cube on the y-axis
	if( mesg->buttons & CWIID_NUNCHUK_BTN_Z ) {
		if( nunchuk_btn_z_pressed == false ) {
			rotate_next_y = 90.0f;
			nunchuk_btn_z_pressed = true;
		}
	} else {
		nunchuk_btn_z_pressed = false;
	}

	nc_stick.valid = 1;
	nc_stick.x = mesg->stick[CWIID_X];
	nc_stick.y = mesg->stick[CWIID_Y];
	nc_stick.max = 0xff;

	viewer_rotation.y() = 110.0f * ((double)nc_stick.x/nc_stick.max);
	viewer_rotation.x() = 110.0f * ((double)nc_stick.y/nc_stick.max);
}

/**
 * Calculates the frames per second rate.
 * @return Current fps value
 */
float step_fps() {
	if( m_fps == 0.0f ) {
		m_fps = INT_MAX;
	}
	return -(1.0f / m_fps) * fall_velocity_factor;
}

/**
 * Restarts a running game and sets
 * all game relevant value to a standard value.
 */
void game_reset() {
	level = 1;
	lines = 0;
	points = 0;
	fall_velocity_factor = 2.0f;
	fall_velocity_factor_bck = 0.0f;
	paused = false;
	gameover = false;
	cube_list.clear();
}

/**
 * A callback function for keyboard events.
 * @param keysym key code/symbol
 * @see http://www.libsdl.org/cgi/docwiki.cgi/SDLKey
 */
static void handle_key_down( SDL_keysym* keysym ) {
    switch( keysym->sym ) {
		case 'w':
			if( viewer_position.z() <= -15.0f && viewer_position.z() >= -40.0f ) {
				viewer_position.z() += 1.0f;
			}
			if( viewer_position.z() > -15.0f ) {
				viewer_position.z() = -15.0f;
			}
			if( viewer_position.z() < -40.0f ) {
				viewer_position.z() = -40.0f;
			}
		break;
		case 's':
			if( viewer_position.z() <= -15.0f && viewer_position.z() >= -40.0f ) {
				viewer_position.z() -= 1.0f;
			}
			if( viewer_position.z() > -15.0f ) {
				viewer_position.z() = -15.0f;
			}
			if( viewer_position.z() < -40.0f ) {
				viewer_position.z() = -40.0f;
			}
		break;

		case 'r':
			viewer_rotation.y() += 5.0f;
		break;
		case 't':
			viewer_rotation.y() -= 5.0f;
		break;
		case 'f':
			viewer_rotation.x() += 5.0f;
		break;
		case 'g':
			viewer_rotation.x() -= 5.0f;
		break;

		case SDLK_LEFT:
			current_object.rotateZ(90.0f);
		break;
		case SDLK_RIGHT:
			current_object.rotateZ(-90.0f);
		break;

		case SDLK_DOWN:
			current_object.rotateX(90.0f);
		break;
		case SDLK_UP:
			current_object.rotateX(-90.0f);
		break;

		case SDLK_DELETE:
			current_object.rotateY(90.0f);
		break;
		case SDLK_PAGEDOWN:
			current_object.rotateY(-90.0f);
		break;

		#define STEP 1.0f
		case 'd':
			move_x(	STEP, 	current_object, cube_list );
		break;
		case 'a':
			move_x(	-STEP, 	current_object, cube_list );
		break;
		case 'q':
			move_z(	STEP, 	current_object, cube_list );
		break;
		case 'e':
			move_z(	-STEP, 	current_object, cube_list );
		break;
		#undef STEP

		case SDLK_RETURN: {
			int r = -1;

			if( !gameover ) {
				r = calculate_objects(	-0.45f,
						current_object,
						next_object,
			 			cube_list );
			}

			if(r == SET) {
				gameover = check_gameover( current_object, wuerfel_start.y() );

				current_object.updateSectors();

				Tetris::glObjectMatrix co = current_object;

				current_next_cube( current_object, next_object, cube_list );

				cVecList<int> x_list, z_list;

				remove_line( co, cube_list, &x_list, &z_list );

				lines += x_list.size() / 10;
				lines += z_list.size() / 10;

				remove_empty_objects( cube_list );

				Tetris::glObjectMatrixList::iterator sit = cube_list.begin();
				Tetris::glObjectMatrixList::iterator eit = cube_list.end();
				for( ; sit != eit; sit++ ) {
					for( int i=0; i < x_list.size(); i++ ) {
						sit->updateCubeY( x_list.at(i), 0 );
					}

					for( int i=0; i < z_list.size(); i++ ) {
						sit->updateCubeY( z_list.at(i), 1 );
					}
				}
			}

		} break;

		case 'p':
			paused = !paused;
		break;

		case '.':
			SDL_WM_ToggleFullScreen(winMain);
		break;

		case 'h':
			printf("-- Help --\n");
			printf("------> Viewer options\n");
			printf("%15s   viewer +z\n", "w");
			printf("%15s   viewer -z\n", "s");
			printf("\n------> Movement options\n");
			printf("%15s   zoom in\n", "w");
			printf("%15s   zoom out\n", "s");
			printf("\n------> Object options\n");
			printf("%15s   object y step\n", "Return");
			printf("%15s   object rotate +z\n", "Arrow Left");
			printf("%15s   object rotate -z\n", "Arrow Right");
			printf("%15s   object rotate -x\n", "Arrow Up");
			printf("%15s   object rotate +x\n", "Arrow Down");
			printf("%15s   object rotate +y\n", "Remove");
			printf("%15s   object rotate -y\n", "Page Down");
			printf("\n------> Game options\n");
			printf("%15s   quit\n", "Escape");
			printf("%15s   toggle fullscreen\n", ".");
			printf("%15s   toggle pause\n", "p");
			printf("%15s   restart the game\n", "Space");
			printf("\n------> Autor\n");
			printf(" Christian Benjamin Ries (C) 2008\n");
			printf(" Christian.Ries@linux-sources.de\n");
		break;

		case SDLK_ESCAPE:
			run = false;
		break;

		case SDLK_SPACE:
			if( gameover ) {
				game_reset();
			}
			break;
		default:
			break;
	}
}

/**
 * Handles all events which are received by
 * Nintendo Wii controller, Nunchuck, keyboard.
 */
static void process_events( void ) {
	if(wiimote) {
		if( rotate_next_x != 0.0f ) {
			current_object.rotateX(rotate_next_x);
		}
		if( rotate_next_y != 0.0f ) {
			current_object.rotateY(rotate_next_y);
		}
		if( rotate_next_z != 0.0f ) {
			current_object.rotateZ(rotate_next_z);
		}

		rotate_next_x = 0.0f;
		rotate_next_y = 0.0f;
		rotate_next_z = 0.0f;

		struct cwiid_state state;
		if( cwiid_get_state(wiimote, &state)) {
			// nothing
		}

		if( state.buttons & CWIID_BTN_LEFT && state.buttons & CWIID_BTN_B ) {
			viewer_position.x() += 1.0f/m_fps * 20.0f;
		}
		if( state.buttons & CWIID_BTN_RIGHT && state.buttons & CWIID_BTN_B ) {
			viewer_position.x() -= 1.0f/m_fps * 20.0f;
		}
		if( state.buttons & CWIID_BTN_UP && state.buttons & CWIID_BTN_B ) {
			viewer_position.z() += 1.0f/m_fps * 20.0f;
		}
		if( state.buttons & CWIID_BTN_DOWN && state.buttons & CWIID_BTN_B ) {
			viewer_position.z() -= 1.0f/m_fps * 20.0f;
		}

		if( battery_value != state.battery ) {
			battery_value = state.battery;
			char battery[14];
			snprintf( battery, 14, "Battery: %d%%",
						(int) (100.0f * battery_value / CWIID_BATTERY_MAX));
			snprintf( win_title, WIN_TITLE_MAX, WIN_TITLE, battery );
			SDL_WM_SetCaption(win_title, NULL);
		}
	}

	SDL_Event event;
 	while( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
	        case SDL_KEYDOWN:
	            handle_key_down( &event.key.keysym );
            break;
			case SDL_VIDEORESIZE: {
				setup_opengl( event.resize.w, event.resize.h );
			} break;
    	    case SDL_QUIT:
	    	    run = false;
            break;
        }
    }
}

/**
 * Mathematical function
 * @param x
 * @return
 */
int nextpoweroftwo(int x) {
	double logbase2 = log(x) / log(2);
	return round(pow(2,ceil(logbase2)));
}

/**
 * Verify the type of cube.
 * @param frmid id of a cube
 * @return id of a texture for the cube
 */
GLuint cube_texture_id( FORMSID frmid ) {
	switch(frmid) {
		case FRM_CUBE: 		return tex_obj_cube;
		case FRM_PYRAMIDE: 	return tex_obj_pyramide;
		case FRM_RECT: 		return tex_obj_rect;
		case FRM_LINE:		return tex_obj_line;
		case FRM_SNAKE:		return tex_obj_snake;
	}
	return 0;
}

/**
 * Determines the next cube object in the matrix of
 * placed objects-
 * @param current_object actual cube
 * @param next_object stores the next cube object
 * @param cube_list list of all cubes
 */
void current_next_cube(
	Tetris::glObjectMatrix & current_object,
	Tetris::glObjectMatrix & next_object,
	Tetris::glObjectMatrixList & cube_list ) {

		cube_list.push_back(current_object);

		points += points_step;
		points_step = points_next_step;

		current_object.reset();
		current_object = next_object;

		struct forms * f = random_form();
		next_object.position() = wuerfel_start;
		next_object.setMatrix( f->frm,
						   	f->cols, f->rows,
	  						f->depth, f->color,
	  						f->id,
	  						cube_texture_id(f->id) );
		next_object.rotateX(90.0f);
		next_object.rotateY(90.0f);
		next_object.rotateZ(90.0f);
		next_object.setCubeSectorArea(
					cVec<float>( PLAYGROUND_STARTX, PLAYGROUND_STARTY, PLAYGROUND_STARTZ ),
					cVec<float>( PLAYGROUND_ENDX, PLAYGROUND_ENDY, PLAYGROUND_ENDZ ),
					1.0f );
		points_next_step = f->points;
}

/**
 * Changes the draw mode, draw as wireframe of
 * filled areas.
 * @param state true=wireframe, false=solid
 */
void draw_mode( bool state ) {
	if(state) {
		/// GL_FRONT_AND_BACK
		/// GL_FRONT
		/// GL_BACK
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_FILL);
// 		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

/**
 * Displays strings
 * @param text
 * @param font
 * @param color
 * @param location
 */
void SDL_GL_RenderText(
	char *text,
    TTF_Font *font,
    SDL_Color color,
    SDL_Rect *location) {

	SDL_Surface *initial;
	SDL_Surface *intermediary;
	SDL_Rect rect;
	int w,h;
	GLuint texture;

	glDisable(GL_ALPHA_TEST);

	/* Use SDL_TTF to render our text */
	initial = TTF_RenderText_Blended(font, text, color);

	if( initial == NULL ) {
		printf("TTF_RenderText_Blended failed: %s", TTF_GetError());
	}

	/* Convert the rendered text to a known format */
	w = nextpoweroftwo(initial->w);
	h = nextpoweroftwo(initial->h);

	intermediary = SDL_CreateRGBSurface(0, w, h, 32,
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	if(SDL_BlitSurface(initial, 0, intermediary, 0) < 0) {
		cout << "SDL_BlitSurface failed!" << endl;
	}

	/* Tell GL about our new texture */
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA,
			GL_UNSIGNED_BYTE, intermediary->pixels );

	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* prepare to render our texture */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);

	/* Draw a quad at location */
	glBegin(GL_QUADS);
		/* Recall that the origin is in the lower-left corner
 * 		   That is why the TexCoords specify different corners
 * 		   		   than the Vertex coors seem to. */
		glTexCoord2f(0.0f, 1.0f);
			glVertex2f(location->x    , location->y);
		glTexCoord2f(1.0f, 1.0f);
			glVertex2f(location->x + w, location->y);
		glTexCoord2f(1.0f, 0.0f);
			glVertex2f(location->x + w, location->y + h);
		glTexCoord2f(0.0f, 0.0f);
			glVertex2f(location->x    , location->y + h);
	glEnd();

	/* Bad things happen if we delete the texture before it finishes */
	glFinish();

	/* return the deltas in the unused w,h part of the rect */
	location->w = initial->w;
	location->h = initial->h;

	/* Clean up */
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &texture);

	glEnable(GL_ALPHA_TEST);
}

/**
 * Draws the complete playing area.
 */
static void draw_screen() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/**
 	 * Spielfenster
	 */
	int w = win_width - MENU_WIDTH;
	int h = win_height;
	glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                GLfloat fAspect = (GLfloat)w/(GLfloat)h;
                gluPerspective( 40.0f, fAspect, 0.1f, -20.0f );
		glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glTranslatef( viewer_position.x(), viewer_position.y(), viewer_position.z() );
	glRotatef( viewer_rotation.x(), 1.0f, 0.0f, 0.0f );
	glRotatef( viewer_rotation.y(), 0.0f, 1.0f, 0.0f );

// 	glDisable(GL_TEXTURE_2D);
// 	glLineWidth(5);
// 	glBegin(GL_LINES);
// 		glColor3f( 1.0f, 0.0f, 0.0f );
// 		glVertex3f( 0.0f, 0.0f, 0.0f );
// 		glVertex3f( 15.0f, 0.0f, 0.0f );
//
// 		glColor3f( 0.0f, 1.0f, 0.0f );
// 		glVertex3f( 0.0f, 0.0f, 0.0f );
// 		glVertex3f( 0.0f, 15.0f, 0.0f );
//
// 		glColor3f( 0.0f, 0.0f, 1.0f );
// 		glVertex3f( 0.0f, 0.0f, 0.0f );
// 		glVertex3f( 0.0f, 0.0f, 15.0f );
// 	glEnd();
// 	glEnable(GL_TEXTURE_2D);

// 	glColor3f( 0.3f, 1.0f, 1.0f );

	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, tex_floor_id );
	glBegin(GL_QUADS);
		#define W 60.0f
		for( float dx=-W; dx<W; dx+=10.0f ) {
				for( float dz=-W; dz<W; dz+=10.0f ) {
					glNormal3f( 0.0f, 1.0f, 0.0f );
					glTexCoord2f( 0.0f, 0.0f );
						glVertex3f( dx, -0.25f, dz+10.0f );


					glTexCoord2f( 0.0f, 1.0f );
						glVertex3f( dx+10.0f, -0.25f, dz+10.0f );


					glTexCoord2f( 1.0f, 1.0f );
						glVertex3f( dx+10.0f, -0.25f, dz );


					glTexCoord2f( 1.0f, 0.0f );
						glVertex3f( dx, -0.25f, dz );
				}
		}
		#undef W
	glEnd();
	glBindTexture( GL_TEXTURE_2D, tex_chess_id );
	glCallList(playgroundList);

	glUseProgram(shader_cubes);

	draw_mode(gl_show_wireframe);
	glDisable(GL_TEXTURE_2D);
		for( int i=0; i<cube_list.size(); i++ ) {
			cube_list.at(i).draw();
		}
		current_object.draw();
		current_object.drawHelp();
	glEnable(GL_TEXTURE_2D);
	draw_mode(false);

	glUseProgram(0);

	/// Show game over image
	Enter2DMode( 0, 0, WIN_WIDTH, WIN_HEIGHT );

	glEnable(GL_TEXTURE_2D);
 	glEnable(GL_BLEND);
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	glBindTexture(GL_TEXTURE_2D, tex_menu_score);
	glBegin(GL_QUADS);
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( 10.0f, win_height-10.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( 180.0f, win_height-10.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( 180.0f, win_height-80.0f );
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( 10.0f, win_height-80.0f );
	glEnd();

	SDL_Color color;
	SDL_Rect position;

	glBlendFunc(GL_ONE, GL_ONE);
	{
		char buf[13];

		snprintf( buf, 13, "%012d", points );

		glEnable(GL_BLEND);
		color.r = 255;
		color.g = 255;
		color.b = 255;
		position.x = 22;
		position.y = win_height - 80;
		SDL_GL_RenderText(buf, font, color, &position);
	}
	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, tex_menu_time);
	glBegin(GL_QUADS);
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( win_width-340.0f, 10.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( win_width-180.0f, 10.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( win_width-180.0f, 80.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( win_width-340.0f, 80.0f );
	glEnd();

	glBlendFunc(GL_ONE, GL_ONE);
	{
		time(&time_end);

		time_t time_diff = time_end - time_start;

		int hour = time_diff / 3600;
		int rest  = time_diff  % 3600;
		int min  = rest / 60;
		int sec   = rest % 60;

		char buf[9];
		snprintf(buf, 9, "%02d:%02d:%02d", hour, min, sec );

		glEnable(GL_BLEND);
		position.x = win_width-297;
		position.y = 13;
		SDL_GL_RenderText(buf, font, color, &position);
	}
	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, tex_menu_level);
	glBegin(GL_QUADS);
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( win_width-85.0f, 10.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( win_width-10.0f, 10.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( win_width-10.0f, 80.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( win_width-85.0f, 80.0f );
	glEnd();

	glBlendFunc(GL_ONE, GL_ONE);
	{
		char buf[3];

		snprintf( buf, 3, "%2d", level );

		glEnable(GL_BLEND);
		position.x = win_width-55;
		position.y = 13;
		SDL_GL_RenderText(buf, font, color, &position);
	}
	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, tex_menu_lines);
	glBegin(GL_QUADS);
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( win_width-175.0f, 10.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( win_width-90.0f, 10.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( win_width-90.0f, 80.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( win_width-175.0f, 80.0f );
	glEnd();

	glBlendFunc(GL_ONE, GL_ONE);
	{
		char buf[3];

		snprintf( buf, 3, "%2d", lines );

		glEnable(GL_BLEND);
		position.x = win_width-135;
		position.y = 13;
		SDL_GL_RenderText(buf, font, color, &position);
	}
	glDisable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, tex_menu_next);
	glBegin(GL_QUADS);
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( win_width-180.0f, win_height-150.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( win_width-10.0f, win_height-150.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( win_width-10.0f, win_height-10.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( win_width-180.0f, win_height-10.0f );
	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBindTexture(GL_TEXTURE_2D, tex_tux);
	glBegin(GL_QUADS);
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( 10.0f, 142.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( 110.0f, 142.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( 110.0f, 10.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( 10.0f, 10.0f );
	glEnd();
	glDisable(GL_BLEND);

	switch(next_object.id()) {
		case FRM_RECT: 			glBindTexture(GL_TEXTURE_2D, tex_menu_rect); break;
		case FRM_PYRAMIDE: glBindTexture(GL_TEXTURE_2D, tex_menu_pyramide); break;
		case FRM_LINE: 			glBindTexture(GL_TEXTURE_2D, tex_menu_line); break;
		case FRM_CUBE: 		glBindTexture(GL_TEXTURE_2D, tex_menu_cube); break;
		case FRM_SNAKE:		glBindTexture(GL_TEXTURE_2D, tex_menu_snake); break;
	}
	glBegin(GL_QUADS);
		glTexCoord2f( 0.0f, 1.0f ); glVertex2f( win_width-175.0f, win_height-132.0f );
		glTexCoord2f( 1.0f, 1.0f ); glVertex2f( win_width-14.0f, win_height-132.0f );
		glTexCoord2f( 1.0f, 0.0f ); glVertex2f( win_width-14.0f, win_height-30.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex2f( win_width-175.0f, win_height-30.0f );
	glEnd();

	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	if( paused ) {
		glBindTexture( GL_TEXTURE_2D, tex_paused_id );
	} else {
		glBindTexture( GL_TEXTURE_2D, tex_gameover_id );
	}

	if(gameover || paused) {
		#define L 300.0f
		#define SX (WIN_WIDTH / 3.0f)
		#define SY (WIN_HEIGHT / 2.0f - L/2.0f)

		glColor4f( 1.0f, 1.0f, 1.0f, 0.9f );
		glBegin( GL_QUADS );
			glTexCoord2f( 0.0f, 1.0f );	glVertex2f( SX, SY );
			glTexCoord2f( 0.0f, 0.0f );	glVertex2f( SX, SY+L );
			glTexCoord2f( 1.0f, 0.0f );	glVertex2f( SX+L, SY+L );
			glTexCoord2f( 1.0f, 1.0f ); 	glVertex2f( SX+L, SY );
		glEnd();

		#undef L
		#undef SX
		#undef SY
	}
	Leave2DMode();

    SDL_GL_SwapBuffers( );

	m_fpsFrames++;

	if( m_fps > 0 && m_fps != m_fpsOld ) {
		m_fpsOld = m_fps;
	}
}

/**
 * Initializes the OpenGL state machine with default values.
 * @param w width of the window
 * @param h height of the window
 */
void setup_opengl( int w, int h ) {
	win_width = w;
	win_height = h;

	srand(time(NULL));

	glewInit();
	if (glewIsSupported("GL_VERSION_2_0")) {
		printf("Ready for OpenGL 2.0\n");
	} else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}

	if(!TTF_WasInit() && TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	font = TTF_OpenFont(fontpath, 24);
	if(!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(2);
	}

	/// Shader laden
	shader_cubes		= setShaders("/usr/local/share/tetrisgl/shader/toon");

	/// Texturen laden
	cout << "Textures ";
	tex_chess_id 		= loadTexture("/usr/local/share/tetrisgl/textures/schachbrett.jpg");
	tex_gameover_id 	= loadTexture("/usr/local/share/tetrisgl/textures/gameover.jpg");
	tex_paused_id 		= loadTexture("/usr/local/share/tetrisgl/textures/paused_inv.jpg");
	tex_floor_id 		= loadTexture("/usr/local/share/tetrisgl/textures/tiles_floor009.jpg");
	tex_menu_level 		= loadTexture("/usr/local/share/tetrisgl/textures/menu-level.png");
	tex_menu_lines		= loadTexture("/usr/local/share/tetrisgl/textures/menu-lines.png");
	tex_menu_next 		= loadTexture("/usr/local/share/tetrisgl/textures/menu-next.png");
	tex_menu_score 		= loadTexture("/usr/local/share/tetrisgl/textures/menu-score.png");
	tex_menu_time 		= loadTexture("/usr/local/share/tetrisgl/textures/menu-time.png");
	tex_menu_cube		= loadTexture("/usr/local/share/tetrisgl/textures/menu-next-cube.png");
	tex_menu_line		= loadTexture("/usr/local/share/tetrisgl/textures/menu-next-line.png");
	tex_menu_pyramide	= loadTexture("/usr/local/share/tetrisgl/textures/menu-next-pyramide.png");
	tex_menu_rect		= loadTexture("/usr/local/share/tetrisgl/textures/menu-next-rect.png");
	tex_menu_snake		= loadTexture("/usr/local/share/tetrisgl/textures/menu-next-snake.png");
	tex_obj_cube		= loadTexture("/usr/local/share/tetrisgl/textures/cube-green.png");
	tex_obj_line		= loadTexture("/usr/local/share/tetrisgl/textures/cube-lila.png");
	tex_obj_pyramide	= loadTexture("/usr/local/share/tetrisgl/textures/cube-blue.png");
	tex_obj_rect		= loadTexture("/usr/local/share/tetrisgl/textures/cube-red.png");
	tex_obj_snake		= loadTexture("/usr/local/share/tetrisgl/textures/cube-yellow.png");
	tex_tux				= loadTexture("/usr/local/share/tetrisgl/textures/tux.png");
	cout << "ok!" << endl;

	/// erste Cube Form
	cout << "Cubes ";
	struct forms * f = random_form();
	current_object.position() = wuerfel_start;
	current_object.setMatrix(
		f->frm,
		f->cols, f->rows,
		f->depth, f->color,
		f->id,
		cube_texture_id(f->id) );

	current_object.rotateX(90.0f);
	current_object.rotateY(90.0f);
	current_object.rotateZ(90.0f);
	current_object.setCubeSectorArea(
		cVec<float>( PLAYGROUND_STARTX, PLAYGROUND_STARTY, PLAYGROUND_STARTZ ),
		cVec<float>( PLAYGROUND_ENDX, PLAYGROUND_ENDY, PLAYGROUND_ENDZ ),
	   	1.0f );
	points_step = f->points;

	/// nächste Cube Form
	f = random_form();
	next_object.position() = wuerfel_start;
	next_object.setMatrix(
		f->frm,
		f->cols, f->rows,
		f->depth, f->color,
		f->id,
		cube_texture_id(f->id) );

	next_object.rotateX(90.0f);
	next_object.rotateY(90.0f);
	next_object.rotateZ(90.0f);
	next_object.setCubeSectorArea(
		cVec<float>( PLAYGROUND_STARTX, PLAYGROUND_STARTY, PLAYGROUND_STARTZ ),
		cVec<float>( PLAYGROUND_ENDX, PLAYGROUND_ENDY, PLAYGROUND_ENDZ ),
		1.0f );
	points_next_step = f->points;
	cout << "ok!" << endl;

	cout << "Playground ";
	playgroundList = loadPlayground(tex_chess_id);
	cout << " ok!" << endl;

    glShadeModel( GL_SMOOTH );

    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1);

	/// Licht
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

	/// FOG
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv (GL_FOG_COLOR, fogColor);
	glFogf (GL_FOG_DENSITY, fogDensity);
	glHint (GL_FOG_HINT, GL_NICEST);
	glFogf( GL_FOG_END, fogFarDistance);
	glFogf( GL_FOG_START, fogNearDistance);
	glFogi(GL_FOG_COORD_SRC, GL_FRAGMENT_DEPTH);
	glEnable (GL_FOG);

	/// Material
	glEnable( GL_COLOR_MATERIAL );

    glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	glEnable( GL_DEPTH_TEST );

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glClearColor( fogColor[0], fogColor[1], fogColor[2], fogColor[3] );
}

/**
 * Calcultes the ticks and adjust the frame rate, so all
 * movements are equal on different hardware architectures.
 */
void idle_func() {
	// get the current tick value
    unsigned int tick;
    tick = Tick::getTick();

    // calculate the amount of elapsed seconds
    float elapsedSeconds;
    elapsedSeconds = (float)(tick - m_lastTick) / 1000.0f;

    // adjust fps counter
    m_fpsDuration += elapsedSeconds;
    if(m_fpsDuration >= 1.0f) {
		m_fps = (int)((float)m_fpsFrames / m_fpsDuration);
        m_fpsDuration = 0.0f;
        m_fpsFrames = 0;
    }

    static double start;
    static double firstTime, lastTime;
    start = Tick::getTime();

    static bool bFirst = true;
    if (bFirst) {
    	firstTime = start;
    } else {
        lastTime = start;
    }

    double stop = Tick::getTime();

    stop -= start;
    static double cumul = 0;
    cumul += stop;

    if (!bFirst) {
    	m_averageCPUTime = cumul / float(lastTime - firstTime) * 100;
    }
    bFirst = false;

    m_lastTick = tick;
}

/**
 * Closes a open audio channel.
 */
void my_audio_finished(int channel) {
	if( channel == channel_set ) {
		play_music_set = false;
	}
	if( channel == channel_theme ) {
		// nothing
	}
}

#ifdef DEBUG
void checkGLerror(int value=0) {
	GLenum err = glGetError();
	if(err!=GL_NO_ERROR) {
		cout << "id: " << value << endl;
	}
	switch(err) {
		case GL_INVALID_ENUM: cout << "GL_INVALID_ENUM" << endl; break;
		case GL_INVALID_VALUE: cout << "GL_INVALID_VALUE" << endl; break;
		case GL_INVALID_OPERATION: cout << "GL_INVALID_OPERATION" << endl; break;
		case GL_STACK_OVERFLOW: cout << "GL_STACK_OVERFLOW" << endl; break;
		case GL_STACK_UNDERFLOW: cout << "GL_STACK_UNDERFLOW" << endl; break;
		case GL_OUT_OF_MEMORY: cout << "GL_OUT_OF_MEMORY" << endl; break;
	}
}
#endif

/**
 * Let us play! :p
 */
int main(int argc, char *argv[]) {
	if( argc > 1 ) {
		for( int i=1; i<argc; i++ ) {
			if(!strcmp(argv[i], "--help")) {
				cout << "Usage: tetrisgl [--help | --wii]" << endl;
				cout << "Copyright (C) 2008 Christian Benjamin Ries" << endl;
				return -1;
			}

			if(!strcmp(argv[i], "--audio")) {
				play_music = true;
			}

			if(!strcmp(argv[i], "--wii")) {
				if( cwiid_init() < 0 ) {
					cout << "CWiid failed!" << endl;
					return 1;
				}

				/** LEDs setzen
 				 */
				uint8_t LED_state = CWIID_LED1_ON; /*|
						CWIID_LED2_ON |
						CWIID_LED3_ON |
						CWIID_LED4_ON;*/

				if (cwiid_set_led(wiimote, LED_state)) {
					cout << "Leds failed!" << endl;
				}

				/** Report setzen
				 */
				uint8_t RPT_mode = CWIID_RPT_STATUS |
						CWIID_RPT_BTN |
						CWIID_RPT_ACC |
						CWIID_RPT_EXT;

				if(cwiid_set_rpt_mode(wiimote, RPT_mode)) {
					cout << "Report failed!" << endl;
				}
			}
		} /// for
	}

	cout << "Loading..." << endl;

	if( SDL_Init(
			SDL_INIT_AUDIO |
			SDL_INIT_TIMER |
			SDL_INIT_VIDEO |
			SDL_INIT_JOYSTICK ) < 0) {
		cout << "Could not initialize SDL: " << SDL_GetError() << endl;
		SDL_Quit();
	}

	winMain = SDL_SetVideoMode(
				win_width, win_height, WIN_DEPTH,
				SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF |
				SDL_OPENGL/* | SDL_RESIZABLE*/ );

	cout << "glInfo ";
	glInfo glInfo;
	if(!glInfo.getInfo()) {
		cout << "failed!" << endl;
	} else {
		cout << "ok!" << endl;
	}
	glInfo.printSelf();

	if( winMain == NULL ) {
		cout << "Could not initialize SDL Video: " << SDL_GetError() << endl;
		SDL_Quit();
	}

	Mix_Chunk *sound = NULL;

	if( play_music ) {
		cout << "Sound ";

		int audio_rate = 44100;		/// 44100 (CD)
		Uint16 audio_format = AUDIO_S16SYS;
		int audio_channels = 2;		/// Stereo
		int audio_buffers = 4096;	/// Music cache

		if(Mix_OpenAudio(
				audio_rate, audio_format,
		   		audio_channels, audio_buffers) != 0) {
			fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
			exit(1);
		}

		sound = Mix_LoadWAV("/usr/local/share/tetrisgl/music/tetris-theme1.wav");
		if(sound == NULL) {
			fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
		}

		channel_theme = Mix_PlayChannel(-1, sound, -1);
		if(channel_theme == -1) {
			fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
		}
		cout << "ok!" << endl;
	}

	snprintf( win_title, WIN_TITLE_MAX, WIN_TITLE, "Battery: undef" );
	SDL_WM_SetCaption(win_title, NULL);

	/// Setup OpenGL

	/* Required setup stuff */
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, win_width / (float)win_height, 0.1f, 50.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
#ifdef DEBUG
	checkGLerror(3);
#endif

	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	setup_opengl( WIN_WIDTH, WIN_HEIGHT );

	/* Irrelevant stuff for this demo */
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/* Required if you want alpha-blended textures (for our fonts) */
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	time(&time_start);

	cout << "Loading: ok" << endl;

    while( run ) {
		if(play_music) {
			Mix_Playing(channel_theme);
		}

		idle_func();
#ifdef DEBUG
		checkGLerror(1);
#endif

		process_events();
#ifdef DEBUG
		checkGLerror(2);
#endif

		if(!gameover && !paused) {
			int r = calculate_objects(
						step_fps(),
						current_object,
						next_object,
						cube_list );
			switch(r) {
				case SET: {
					gameover = check_gameover( current_object, wuerfel_start.y() );

					current_object.updateSectors();

					Tetris::glObjectMatrix co = current_object;

					current_next_cube( current_object, next_object, cube_list );

					cVecList<int> x_list, z_list;

					remove_line( co, cube_list, &x_list, &z_list );

					remove_empty_objects( cube_list );

					Tetris::glObjectMatrixList::iterator sit = cube_list.begin();
					Tetris::glObjectMatrixList::iterator eit = cube_list.end();
					for( ; sit != eit; sit++ ) {
						for( int i=0; i < x_list.size(); i++ ) {
							sit->updateCubeY( x_list.at(i), 0 );
						}

						for( int i=0; i < z_list.size(); i++ ) {
							sit->updateCubeY( z_list.at(i), 1 );
						}
					}

					if(play_music) {
						play_music_set = true;
						Mix_ChannelFinished(my_audio_finished);
						Mix_Chunk *sound = Mix_LoadWAV("/usr/local/share/tetrisgl/music/block-set1.wav");

						if(sound == NULL) {
							fprintf(stderr, "Unable to load WAV file: %s\n",
							Mix_GetError());
						}

						channel_set = Mix_PlayChannel(-1, sound, 0);
						if(channel_set == -1) {
							fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
						}
					}

				} break;
				case NONE:
				default:
					;
			}
		}

		draw_screen();
#ifdef DEBUG
		checkGLerror(4);
#endif
    } /// while(run)

	if(wiimote) {
		cout << "Close wiimote: ";
		cwiid_close(wiimote);
		cout << "ok!" << endl;
	}

	TTF_CloseFont(font); font = NULL;
	TTF_Quit();
	Mix_FreeChunk(sound);
	Mix_CloseAudio();
	SDL_Quit();
}
