/*
 * File: fractals.h
 *
 * 
 * Authors:
 * 	Christian Felipe Álvarez
 * 	Lisardo Prieto González
 *
 */
#ifndef _FRACTALS_H
#define _FRACTALS_H

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "bmp_image.h"
#include "complex.h"

/*
 * Struct to keep an totals
 */
typedef struct {
	int coord[2];
} inter;

/*
 * Struct to keep a list of totals
 */
typedef struct {
	int used;
	int totals;
	inter *i_totals;
	complex *comp;
} list_inter;


/*
 * Determine if a complex number c is in the Mandelbrot set for a maximum number of iterations.
 * Returns max_it if c is in the Mandelbrot set, and the number of iterations needed to discover that c is not in the Mandelbrot set otherwise. 
 */
int in_mandelbrot(complex c, int max_it);

/*
 * Determine if a complex number c is in the Julia J(c) set for a maximum number of iterations. J(c) --> z' = z^exp + c
 * Returns max_it if c is in the Mandelbrot set, and the number of iterations needed to discover that c is not in J(c) set otherwise. 
 */
int in_julia(complex z, complex c, int exp, int max_it);


/*
 * Obtain the increment to draw Julia and Mandelbrot sets in a BMP image
 * 	- longitud is the width or height of the image in pixels. 
 * 	- max & min are maximum and minimum in width or height.
 */
double get_increment(int longitud, double min, double max);

/*
 * Draw the slice of image corresponding to the thread who called the function.
 * params is a buffer where all the parameters are received in the same order that were declared.
 */
void * draw_mandelbrot_thread(void * params);

/*
 * Draw the Mandelbrot set in a (w x h) image by means of many thread as specified.
 * Params:
 * 	- w, h: width and height of the image
 * 	- min, max: complex numbers that represents minimums and maximums real and imaginary numbers
 * 	- max_it: maximum number of iterations to determine if a point is in the Mandelbrot set
 * 	- n_procs: number of threads to draw the image
 */
void draw_mandelbrot(int w, int h, complex min, complex max, int max_it, int n_procs, char *nombreImg);

/*
 * Draw the slice of image corresponding to the thread who called the function.
 * params is a buffer where all the parameters are received in the same order that were declared.
 */
void * draw_julia_thread(void * params);

/*
 * Draw the Julia set in a (w x h) image by means of many thread as specified.
 * Params:
 *  - punto: point that defines the Julia set to draw J(punto)
 *  - exp: function's exponent z' = z^exp 
 * 	- w, h: width and height of the image
 * 	- min, max: complex numbers that represents minimums and maximums real and imaginary numbers
 * 	- max_it: maximum number of iterations to determine if a point is in the Mandelbrot set
 * 	- n_procs: number of threads to draw the image
 */
void draw_julia(complex punto, int exp, int w, int h, complex min, complex max, int max_it, 
	int n_procs, char *nombreImg);

/* Draw an inverted triangle from p point*/
void paint_triangle_inverted(tColorImage *bmp_image, int p, int altura, 
	unsigned char r, unsigned char g, unsigned char b);

/* Draw a triangle from p point */
void paint_triangle(tColorImage *bmp_image, int p, int altura,
	unsigned char r, unsigned char g, unsigned char b);

/* Draw a Sierpinski's triangle recursively */
void draw_sierpinski_recursivo(tColorImage *bmp_image, int punto, int altura, int iteracion, int maxIter, 
	unsigned char r, unsigned char g, unsigned char b);

/* Create and draw a Sierpinski's triangle */
void create_sierpinski(tColorImage* bmp_image, int l, int it, tColorPixel fractal, tColorPixel fondo); 

/* Draw a Sierpinski's triangle and write it in a file named nombreImg */
void draw_sierpinski(int w, int h, int it, tColorPixel fractal, tColorPixel fondo, char *nombreImg); 

/*
 * Create a t-square image recursively drawing a square side l in the centre of the image, from the pixel p. It is done it_max times to generate the fractal.
 */
void paint_square_comp(tColorImage *bmp_image, int p, int l, int it, int it_max, tColorPixel fractal); 

/* Draw a t-square in bmp_image */
void create_tsquare(tColorImage* bmp_image, int l, int it, tColorPixel fractal, tColorPixel fondo);

/* Draw a t-square and write it in a file named nombreImage */
void draw_tsquare(int w, int h, int it, tColorPixel fractal, tColorPixel fondo, char *nombreImg);


#endif
