/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifndef __GLOBALS_H__
#define __GLOBALS_H__

/// OpenGL
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

/// SDL
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

/// Ries
#include "glCube.h"
#include "glObjectMatrix.h"
#include "playground.h"
#include "functions.h"
#include "movement.h"
#include "shadow.h"
#include "texture.h"
#include "forms.h"
#include "logic.h"
#include "tick.h"
#include "shader.h"

#define WIN_WIDTH 1024
#define WIN_HEIGHT 768
// #define MENU_WIDTH 140
#define MENU_WIDTH 0
#define WIN_DEPTH 16
#define WIN_TITLE "Tetris GL (%s)"
#define WIN_TITLE_MAX 30

// #define PLAYGROUND_STARTX -4.5f
// #define PLAYGROUND_ENDX 5.5f
// #define PLAYGROUND_STARTZ -4.5f
// #define PLAYGROUND_ENDZ 5.5f

#define PLAYGROUND_STARTX 0.0f
#define PLAYGROUND_ENDX 10.0f
#define PLAYGROUND_STARTY 0.0f
#define PLAYGROUND_ENDY 10.0f
#define PLAYGROUND_STARTZ 0.0f
#define PLAYGROUND_ENDZ 10.0f

#define DEG2RAD(v) (M_PI * v / 180.0f)
#define RAD2DEG(v) (180.0f * v / M_PI)

static GLfloat ambientLight[]	= { 0.3f, 0.3f, 0.3f, 1.0f };
static GLfloat diffuseLight[]   	= { 0.7f, 0.7f, 0.7f, 1.0f };
static GLfloat specular[]           = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightPos[]            = { 0.0f, 25.0f, 30.0f , 1.0f };

/// FOG
static GLfloat fogDensity = 0.03f;
static GLfloat fogFarDistance = 70.0f;
static GLfloat fogNearDistance = 50.0f;
static GLfloat fogColor[4] = {0.0, 0.0, 0.0, 0.0};

// 					Ambient 						Diffus 							Spekular 						Shininess
// Chrom 		0.25	0.25	0.25	1.0		0.40	0.40	0.40	1.0		0.77	0.77	0.77	1.0		76.8
static GLfloat mat_chrom_ambient[4] 	= { 0.25f, 0.25f, 0.25f, 1.0f };
static GLfloat mat_chrom_diffus[4] 		= { 0.4f, 0.4f, 0.4f, 1.0f };
static GLfloat mat_chrom_specular[4] 	= { 0.77f, 0.77f, 0.77f, 1.0f };
static GLfloat mat_chrom_shininess		= 76.8f;

/// Bewegungsvariablen
static float rotate_next_x = 0.0f;
static float rotate_next_y = 0.0f;
static float rotate_next_z = 0.0f;

/// OpenGL Variablen
static bool gl_show_wireframe = false;

/// Spielvariablen
static float wuerfel_step_time = 1.0f;
static int level = 1;
static int lines = 0;
static int points = 0;
static int points_step = 0;
static int points_next_step = 0;
static bool run = true;
static float fall_velocity_factor = 2.0f;
static float fall_velocity_factor_bck = 0.0f;
static bool paused = false;

/// Position des Spielers
static cVec<float> viewer_position(-5.0f, 0.0f, -40.0f);
static cVec<float> viewer_rotation(30.0f, 0.0f, 0.0f);

/// Startposition der neuen Würfel
static cVec<float> wuerfel_start(4.5f, 20.0f, 4.5f);

/// Zeit und Frames Berechnung
static unsigned int m_lastTick;
static float m_fpsDuration;
static int m_fpsFrames;
static int m_fpsOld;
static int m_fps;
static float m_averageCPUTime;

#define SS() (1.0f / (m_fps==0.0f?INT_MAX:m_fps)) * fall_velocity_factor

/// Shader
static GLuint shader_cubes;

/// Texturen
static GLuint tex_chess_id;
static GLuint tex_gameover_id;
static GLuint tex_paused_id;
static GLuint tex_floor_id;
static GLuint tex_menu_level;
static GLuint tex_menu_lines;
static GLuint tex_menu_next;
static GLuint tex_menu_score;
static GLuint tex_menu_time;
static GLuint tex_menu_cube;
static GLuint tex_menu_line;
static GLuint tex_menu_pyramide;
static GLuint tex_menu_rect;
static GLuint tex_menu_snake;
static GLuint tex_obj_cube;
static GLuint tex_obj_line;
static GLuint tex_obj_pyramide;
static GLuint tex_obj_rect;
static GLuint tex_obj_snake;
static GLuint tex_tux;

/// SDL Variablen
static bool play_music = false;			/// Gibt an ob Musik gespielt werden soll.
static bool play_music_set = false;		/// Status des Set-Sounds wenn ein Stein gesetzt wurde.
static int channel_theme = -1;
static int channel_set = -1;
static SDL_Surface * winMain;
static TTF_Font * font;

#endif
