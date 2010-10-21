/*
 * File: bmp_image.h
 *
 * Functions to manage BMP images without compression. You can create and write an image if you define the colour of its pixels. 
 * Authors:
 * 	Christian Felipe Álvarez
 * 	Lisardo Prieto González
 *
 */

#ifndef _BMP_IMAGE_H
#define _BMP_IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define TRUE 1
#define FALSE 0

/* Struct to define a colour pixel (RGB)*/
typedef unsigned char tColorPixel[3];

/* Struct tu define an image*/
typedef struct
{
	unsigned int rows, cols; /*Filas y columnas (alto y ancho en pixeles)*/
	tColorPixel *image; /*Puntero que define la bmp_image*/
} tColorImage;

/*
 * Create an image with size (rows x cols) in *image.
 * Pixels have to be initialized later (they don't have a default value). 
 * Returns TRUE if everything has gone OK, FALSE otherwise.
 */
int BMP_Create_Color_Image(tColorImage *image, unsigned int rows, unsigned int cols);

/* Write the image in the file whose path is filename*/
int BMP_Write_Color_Image(char *filename, tColorImage *image);

/*Free image memory*/
void BMP_Free_Color_Image(tColorImage *image);

#endif
