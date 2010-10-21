/*
 * File: complex.h
 *
 * Types and operations for complex numbers.
 *
 * Authors:
 * 	Christian Felipe Álvarez
 * 	Lisardo Prieto González
 *
 */

#ifndef _COMPLEX_H
#define _COMPLEX_H

#include <math.h>

/*Struct to define a complex number*/
typedef struct {
	double real;
	double img;
} complex;

/*Return the sum of two complex numbers*/
complex add_comp(complex, complex);

/*Return the multiplication of two complex numbers*/
complex times_comp(complex, complex);

/*Return the square of a complex number*/
complex square_comp(complex);

/*Return the exponentation of a complex number*/
complex power_comp(complex,int);

/*Return the module of a complex number */
double mod_comp(complex);

/*Return the squared module of a complex number*/
double mod_square_comp(complex);

/*Return a two exponentation*/
int two_to_the(int);

#endif
