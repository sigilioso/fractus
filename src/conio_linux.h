/*
 * CONIO_LINUX (header file)
 *
 * Utilities to manipulate text properties on Linux Console
 *
 * Authors:
 * 	Christian Felipe Álvarez
 * 	Lisardo Prieto González
 */

#include <stdio.h>

#ifndef _CONIO_LINUX_H
#define _CONIO_LINUX_H

#define DEFAULT		0

#define FONT_BLACK 	30
#define FONT_RED 	31
#define FONT_GREEN 	32
#define FONT_BROWN	33
#define FONT_BLUE	34
#define FONT_MAGENTA	35
#define FONT_CYAN	36
#define FONT_WHITE	37

#define BG_BLACK 	40
#define BG_RED 		41
#define BG_GREEN 	42
#define BG_BROWN	43
#define BG_BLUE		44
#define BG_MAGENTA	45
#define BG_CYAN		46
#define BG_WHITE	47
#define BG_DEFAULT	49

#define FONT_BOLD		1
#define FONT_HALF_BRIGTH	2
#define FONT_UNDERSCORE		4
#define FONT_BLINK		5
#define FONT_NORMAL_INTENSISTY	22

/*Reset text properties to default*/
void reset2defaults();

/*Do this system call: "reset"*/
void reset_console();

/*Set specified text color*/
void set_font_color(int);

/*Set specified background color*/
void set_bg_color(int);

/*Set specified text and background color*/
void set_font_bg_color(int font, int bg);

/*Set specified font property*/
void set_font_props(int);

/*Set specified color and prop to text*/
void set_font_props_color(int color, int prop);

#endif



