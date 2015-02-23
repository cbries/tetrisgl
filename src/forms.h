/*
 * Copyright (c) 2008 Christian Benjamin Ries
 * License: MIT
 * Website: https://github.com/cbries/tetrisgl
 */
#ifndef __FORMS_H__
#define __FORMS_H__

#include "cColor.h"

/**
 * -----> x  -  cols
 * |
 * |
 *`´
 * y  -  rows
 *
 * Blöcke mit Leerzeile dazwischen ist die Anzahl der
 * des Wertes für z  -  depth.
 */

static int __rechteck[] = {
	0, 0, 1,
	1, 1, 1
};

static int __pyramide[] = {
	0, 1, 0,
	1, 1, 1
};

static int __line[] = {
	1,
	1,
	1,
	1
};

static int __cube[] = {
	1
};

static int __snake[] = {
	1, 1, 0,
	0, 1, 1
};

static int __shortline[] = {
	1,
 	1
};

static int __bigline[] = {
	1, 1, 1, 1, 1
};

// static int __form1[] = {
// 	1, 1, 1,
// 	0, 1, 0,
// 	0, 1, 0,
//
// 	1, 0, 1,
// 	0, 0, 0,
// 	0, 0, 0,
//
// 	1, 1, 1,
// 	0, 1, 0,
// 	0, 1, 0
// };
//
// static int __line[] = {
// 	1,
// 	1,
// 	1,
// 	1
// };
//
// static int __wuerfel2[] = {
// 	1, 1,
// 	1, 1,
//
// 	1, 1,
// 	1, 1
// };
//
// static int __wuerfel3[] = {
// 	1, 1, 1,
// 	1, 1, 1,
// 	1, 1, 1,
//
// 	1, 1, 1,
// 	1, 1, 1,
// 	1, 1, 1,
//
// 	1, 1, 1,
// 	1, 1, 1,
// 	1, 1, 1
// };
//
// static int __snake[] = {
// 	1, 1, 1, 0, 0,
// 	0, 0, 1, 0, 0,
// 	0, 0, 1, 1, 1,
//
// 	1, 1, 1, 0, 0,
// 	0, 0, 1, 0, 0,
// 	0, 0, 1, 1, 1
// };
//
// static int __pyramide[] = {
// 	0, 0, 0,
// 	1, 1, 1,
//
// 	0, 1, 0,
// 	1, 1, 1,
//
// 	0, 0, 0,
// 	1, 1, 1
// };

// static int __cube1[] = {
// 	1
// };

#define FORMS_COUNT 5

enum FORMSID {
		FRM_RECT,
		FRM_PYRAMIDE,
		FRM_LINE,
		FRM_CUBE,
		FRM_SNAKE
};

static struct forms {
	int * frm;
	int cols;
	int rows;
	int depth;
	cColor color;
	FORMSID id;
	int points;
} forms[FORMS_COUNT] = {
/// form, cols, rows, depth
	{ __rechteck, 3, 2, 1, cColor(COLOR_RED), FRM_RECT, 50 },
// 	{ __bigline, 5, 1, 1, cColor(COLOR_BLUE), FRM_PYRAMIDE, 100 },
	{ __pyramide, 3, 2, 1, cColor(COLOR_BLUE), FRM_PYRAMIDE, 100 },
	{ __line, 1, 4, 1, cColor(COLOR_LILA), FRM_LINE, 30 },
	{ __cube, 1, 1, 1, cColor(COLOR_GREEN), FRM_CUBE, 10 },
	{ __snake, 3, 2, 1, cColor(COLOR_YELLOW), FRM_SNAKE, 90 }
// 	{ __shortline, 1, 2, 1, cColor(COLOR_YELLOW), FRM_SNAKE, 20 }

// 	{ __line, 1, 4, 1, cColor(COLOR_GREEN) },
// 	{ __wuerfel2, 2, 2, 2, cColor(COLOR_BLUE) },
// 	{ __wuerfel3, 3, 3, 3, cColor(COLOR_WHITE) },
// 	{ __snake, 5, 3, 2, cColor(COLOR_LILA) },
// 	{ __pyramide, 3, 2, 3, cColor(COLOR_BROWN) }
// 	{ __cube1, 1, 1, 1, cColor(COLOR_LILA) }
};

static struct forms * random_form() {
	int index = rand() % FORMS_COUNT;
	return &forms[index];
}

#endif
