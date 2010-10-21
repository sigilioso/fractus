/*
 * File: complex.c
 *
 * Authors:
 * 	Christian Felipe Álvarez
 * 	Lisardo Prieto González
 *
 */

#include "complex.h"

/*Return the sum of two complex numbers*/
complex add_comp(complex c1, complex c2) {
	complex resultado;

	resultado.real = c1.real + c2.real;
	resultado.img = c1.img + c2.img;

	return resultado;
}

/*Return the multiplication of two complex numbers*/
complex times_comp(complex c1, complex c2) {
	complex resultado;

	resultado.real = c1.real*c2.real - c1.img*c2.img;
	resultado.img = c1.real*c2.img + c1.img*c2.real;

	return resultado;
}

/*Return the exponentation of a complex number*/
complex power_comp(complex c, int e) {
	int i = 0;
	for(i = 0; i < e-1; i++)
		c = times_comp(c,c);
	return c;
}

/*Return the square of a complex number*/
complex square_comp(complex c) {
	complex resultado;
	resultado.real = c.real*c.real - c.img*c.img;
	resultado.img = 2*c.real*c.img;
	return resultado;
}

/*Return the module of a complex number */
double mod_comp(complex c) {
	return sqrt(c.real*c.real + c.img*c.img);
}

/*Return the squared module of a complex number*/
double mod_square_comp(complex c) {
	return c.real*c.real + c.img*c.img;
}

/*Return a two exponentation*/
int two_to_the(int n) {
	return n == 0 ? 1 : 2*two_to_the(n-1);
}


