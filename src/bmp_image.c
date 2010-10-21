/*
 * File: bmp_image.c
 *
 * Authors:
 * 	Christian Felipe Álvarez
 * 	Lisardo Prieto González
 *
 */

#include "bmp_image.h"


/*
 * Create an image with size (rows x cols) in *image.
 * Pixels have to be initialized later (they don't have a default value). 
 * Returns TRUE if everything has gone OK, FALSE otherwise.
 */
int BMP_Create_Color_Image( tColorImage *image, unsigned int rows, unsigned int cols ) {
	image->rows = rows;
	image->cols = cols;
	image->image = (tColorPixel*) malloc( image->rows * image->cols * sizeof(tColorPixel));

	if (image->image == 0) 
		return( FALSE );
	else 
		return( TRUE );
}


/*Write the image in the file whose path is filename*/
int BMP_Write_Color_Image( char *filename, tColorImage *image ) {
	FILE *f;
	unsigned char buf[ 100 ];
	unsigned int row, col;
	long longtmp;
	short shortmp;
	int padding;
	int fallo;
	f = fopen(filename, "wb");
	
	/*If we can not open the file*/
	if (f == NULL) 
		return( FALSE ); 
	
	/* Write the header */
	buf[0] = 'B';
	buf[1] = 'M';
	fallo = fwrite( buf, 1, 2, f );
	
	padding = ((int)((double)image->cols / 4.0 + 0.5))*4 - image->cols;
	longtmp = image->cols*image->rows*3 + padding*image->rows + 54;
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	
	longtmp = 0;
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 54;
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 40;
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = image->cols;
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = image->rows;
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	shortmp = 1;
	fallo = fwrite( &shortmp, sizeof(shortmp), 1, f );
	shortmp = 24;
	fallo = fwrite( &shortmp, sizeof(shortmp), 1, f );
	longtmp = 0;
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = image->rows*image->cols;
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 2834;					/* Width resolution */
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 2834;					/* Height resolution */
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 0;
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
	longtmp = 0;
	fallo = fwrite( &longtmp, sizeof(longtmp), 1, f );
		
	/*We write pixels*/
	for (row=0; row < image->rows; row++)
	{
	   for (col=0; col < image->cols; col++)
	   { 
		buf[2] = image->image[ row * image->cols + col][0];
		buf[1] = image->image[ row * image->cols + col][1];
		buf[0] = image->image[ row * image->cols + col][2];
		fallo = fwrite( buf, 1, 3, f );
	   }
	   padding = ((int)((double)image->cols / 4.0 + 0.5))*4 - image->cols;
	   if (padding > 0) fallo = fwrite( buf, 1, padding, f ); 
	}
	fclose( f );
	if(fallo < 0) return FALSE;
        return( TRUE );
}

/*Free image memory*/
void BMP_Free_Color_Image( tColorImage *image ) {
	free( image->image );
}

