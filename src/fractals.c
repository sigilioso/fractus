/*
 * File: fractals.c
 *
 * Authors:
 * 	Christian Felipe Álvarez
 * 	Lisardo Prieto González
 *
 */

#include "fractals.h"

/*
 * Barriers to synchronize threads 
 */
pthread_barrier_t barrera;
pthread_mutex_t mutex_obtener;

/*
 * List of totals to process 
 */
list_inter lista_totals;

/*
 * Determine if a complex number c is in the Mandelbrot set for a maximum number of iterations.
 * Returns max_it if c is in the Mandelbrot set, and the number of iterations needed to discover that c is not in the Mandelbrot set otherwise. 
 */
int in_mandelbrot(complex c, int max_it) {
	int i;
	complex z;
	
	z.real = .0;
	z.img = .0;

	for(i = 0; i < max_it; i++) {
		if(mod_square_comp(z) > 4)
			return i;
		else
			z = add_comp(square_comp(z),c);
	}
	return max_it;
}

/*
 * Determine if a complex number c is in the Julia J(c) set for a maximum number of iterations. J(c) --> z' = z^exp + c
 * Returns max_it if c is in the Mandelbrot set, and the number of iterations needed to discover that c is not in J(c) set otherwise. 
 */
int in_julia(complex z, complex c, int exp, int max_it) {
	int i;

	for(i = 0; i < max_it; i++){
		if(mod_square_comp(z) > 4)
			return i;
		else
			z = add_comp(power_comp(z,exp),c);
	}
	return max_it;
}

/*
 * Obtain the increment to draw Julia and Mandelbrot sets in a BMP image
 * 	- longitud is the width or height of the image in pixels. 
 * 	- max & min are maximum and minimum in width or height.
 */
double get_increment(int longitud, double min, double max) {
	return (max - min) / ((double)longitud);
}

/* Get a color of a pixel depending to its distance to the Mandelbrot set*/
int color(int it, int max_it) {
	return 255 - ((int)(((double)(max_it - it) / (double)max_it) * 255));
}

/*
 * Draw the slice of image corresponding to the thread who called the function.
 * params is a buffer where all the parameters are received in the same order that were declared.
 */
void * draw_mandelbrot_thread(void * params) {
	/* Params in the buffer */
	complex c; /* first complex number, corresponding to the first pixel in bmp_image*/
	int pixelInicial, pixelFinal; /* First and last pixel of the slice of the bmp_image*/
	double incw, inch; /* Increment in width and height (real and imaginary) to draw the set*/
	tColorImage *bmp_image; /* Image that keeps pixels */
	complex min; /* minimum complex number */
	complex max; /* maximum complex number */
	int max_it; /* Maximum number of iterations to determine if a number is in the Mandelbrot set*/
	int exito; /*To define if we could get the total*/
	int totals[2]; /* totals to calculate */

	/*Thread's variables*/
	int i;
	int it;

	/*Get the params*/
	memcpy(&c,
		params,
		sizeof(complex));
	memcpy(&pixelInicial,
		params+sizeof(complex),
		sizeof(int));
	memcpy(&pixelFinal,
		params+sizeof(complex)+sizeof(int),
		sizeof(int));
	memcpy(&incw,
		params+sizeof(complex)+2*sizeof(int),
		sizeof(double));
	memcpy(&inch,
		params+sizeof(complex)+2*sizeof(int)+sizeof(double),
		sizeof(double));
	memcpy(&bmp_image,
		params+sizeof(complex)+2*sizeof(int)+2*sizeof(double),
		sizeof(tColorImage*));
	memcpy(&min,
		params+sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
		sizeof(complex));
	memcpy(&max,
		params+2*sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
		sizeof(complex));
	memcpy(&max_it,
		params+3*sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
		sizeof(int));

	while (lista_totals.used < lista_totals.totals) {
		exito = 0;
		pthread_mutex_lock(&mutex_obtener);
		if ( lista_totals.used < lista_totals.totals )
		{	
			memcpy (totals,lista_totals.i_totals[lista_totals.used].coord,sizeof(inter));
			memcpy (&c,&lista_totals.comp[lista_totals.used],sizeof(complex));		
			lista_totals.used++;
			exito =1;
		} 
		pthread_mutex_unlock(&mutex_obtener);
		if (exito == 1) {
			/*Drawing by ranks*/
			for (i = totals[0]; (i < totals[1]); i++) {
				/*If the point is in the set we paint it black*/
				if((it = in_mandelbrot(c, max_it)) == max_it) {
					bmp_image->image[i][0] = 0;
					bmp_image->image[i][1] = 0;
					bmp_image->image[i][2] = 0;
				}
				else {
					it = color(it, max_it);
					bmp_image->image[i][0] = it;
					bmp_image->image[i][1] = it;
					bmp_image->image[i][2] = it + 88 < 255 ? it + 88 : 255;
				}
				/*Complex number corresponding to the following pixel*/
				if((i % bmp_image->cols) == 0) {
					c.real = min.real;
					c.img += inch;
				}
				else {
					c.real += incw;
				}
			}
		}
	}
	pthread_barrier_wait(&barrera);
}

/*
 * Draw the Mandelbrot set in a (w x h) image by means of many thread as specified.
 * Params:
 * 	- w, h: width and height of the image
 * 	- min, max: complex numbers that represents minimums and maximums real and imaginary numbers
 * 	- max_it: maximum number of iterations to determine if a point is in the Mandelbrot set
 * 	- n_procs: number of threads to draw the image
 */
void draw_mandelbrot(int w, int h, complex min, complex max, int max_it, int n_procs, char *nombreImg) {
	double incw, inch; /*Increments to calculate complex numbers corresponding to pixels*/
	tColorImage bmp_image, *ptrbmp_image; /*Image*/
	complex c; /*Complex number that will be corresponding to pixels*/
	int pixelInicial[n_procs], pixelFinal[n_procs]; /*First and last pixels for each thread */
	int carga; /*Number of pixels for each thread*/
	void * params; /*Buffer to give params to each thread*/
	pthread_t threads[n_procs]; /*thread to draw the image*/
	int i = 0;

	/*Get the increments*/
	if(w < h) {
		incw = get_increment(w, min.real, max.real);
		inch = get_increment(h, min.img, max.img);
	}
	else {
		incw = get_increment(w, min.real, max.real);
		inch = get_increment(h, min.img, max.img);
	}

	/*Create bmp image*/
	BMP_Create_Color_Image(&bmp_image, h, w);

	/*Calculate thread's load*/
	carga = w*h/n_procs;

	pthread_barrier_init(&barrera, NULL, n_procs);
	
	/*Calculate slices to process*/
	lista_totals.i_totals = malloc (sizeof (inter)*n_procs);
	lista_totals.comp = malloc (sizeof (complex)*n_procs);
	for(i = 0; i < n_procs; i++) {
                /*Get first and last pixel*/
                lista_totals.i_totals[i].coord[0] = i*carga;
		lista_totals.i_totals[i].coord[1] = (i == n_procs -1) ? w*h : (i+1)*carga;
		lista_totals.comp[i].real = min.real + incw * (lista_totals.i_totals[i].coord[0] % w);
		lista_totals.comp[i].img = min.img + inch * (lista_totals.i_totals[i].coord[0] / w);
	}
	lista_totals.totals = n_procs;
	lista_totals.used = 0;

	/*Each thread draws its slice of image*/
	for(i = 0; i < n_procs; i++) {
		/*First and last pixels*/
		pixelInicial[i] = i*carga;
		pixelFinal[i] = (i == n_procs -1) ? w*h : (i+1)*carga;
		//First complex number to draw
		c.real = min.real + incw * (pixelInicial[i] % w);
		c.img = min.img + inch * (pixelInicial[i] / w);
		/*image*/
		ptrbmp_image = &bmp_image;
		/*Give params to the thread*/
		params = (void *)malloc(3*sizeof(complex) + 3*sizeof(int) + 2*sizeof(double) + sizeof(tColorImage *));
		memcpy(params,
			&c,sizeof(complex));
		memcpy(params+sizeof(complex),
			&pixelInicial[i],sizeof(int));
		memcpy(params+sizeof(complex)+sizeof(int),
			&pixelFinal[i],sizeof(int));
		memcpy(params+sizeof(complex)+2*sizeof(int),
			&incw,sizeof(double));
		memcpy(params+sizeof(complex)+2*sizeof(int)+sizeof(double),
			&inch,sizeof(double));
		memcpy(params+sizeof(complex)+2*sizeof(int)+2*sizeof(double),
			&ptrbmp_image,sizeof(tColorImage *));
		memcpy(params+sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
			&min,sizeof(complex));
		memcpy(params+2*sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
			&max,sizeof(complex));
		memcpy(params+3*sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
			&max_it,sizeof(int));

		//Go!
		if (pthread_create(&threads[i], NULL, draw_mandelbrot_thread, params) != 0) {
			printf("Error: No se pudo crear el hilo %d.\n",(i));
			exit(2);
		}

		/*Free params*/
		free(params);
	}
	/*We wait for threads*/
	for(i = 0; i < n_procs; i++) {
		pthread_join(threads[i], NULL);
	}
	
	pthread_barrier_destroy(&barrera);

	free(lista_totals.i_totals);
	free(lista_totals.comp);

	/*Write image to file*/
	BMP_Write_Color_Image(nombreImg, &bmp_image);
	BMP_Free_Color_Image(&bmp_image);
}


/*
 * Dibuja la parte de la bmp_image del conjunto de Julia que le corresponde a cada proceso ligero que invoca la función.
 * Recibe los parámetros en el buffer params, en el mismo orden que están declarados.
 */
void * draw_julia_thread(void * params) {
	/*Parámetros recibidos en el buffer*/
	complex z; /*complex inicial correspondiente al primer píxel del fragmento de bmp_image*/
	int pixelInicial, pixelFinal; /*Píxeles inicial y final que delimitan el fragmento de bmp_image*/
	double incw, inch; /*Valor en el que hay que inclementar la parte real e imaginaria, 
				respectivamente, del complex para cada píxel*/
	tColorImage *bmp_image; /*Puntero a la bmp_image en la que se modifican los píxeles*/
	complex min; /*complex con los componentes real e imaginario mínimos que se pintan en la bmp_image*/
	complex max; /*complex con los componentes real e imaginario máximos que se pintan en la bmp_image*/
	int max_it; /*Número máximo de iteraciones con las que se calcula si los puntos pertenecen 
			al conjunto de Mandelbrot*/
	complex c; /*complex que determina el conjunto de julia*/
	int exp; /*Entero que determina la fórmula del conjunto de julia*/
	int exito; /*Para determinar si se pudo obtener el totals*/
	int totals[2]; /*totals a calcular*/

	/*Variables propias del proceso ligero*/
	int i;
	int it;

	/*Se obtienen los parámetros*/
	memcpy(&z,
		params,
		sizeof(complex));
	memcpy(&pixelInicial,
		params+sizeof(complex),
		sizeof(int));
	memcpy(&pixelFinal,
		params+sizeof(complex)+sizeof(int),
		sizeof(int));
	memcpy(&incw,
		params+sizeof(complex)+2*sizeof(int),
		sizeof(double));
	memcpy(&inch,
		params+sizeof(complex)+2*sizeof(int)+sizeof(double),
		sizeof(double));
	memcpy(&bmp_image,
		params+sizeof(complex)+2*sizeof(int)+2*sizeof(double),
		sizeof(tColorImage*));
	memcpy(&min,
		params+sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
		sizeof(complex));
	memcpy(&max,
		params+2*sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
		sizeof(complex));
	memcpy(&max_it,
		params+3*sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
		sizeof(int));
	memcpy(&c,
		params+3*sizeof(complex)+3*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
		sizeof(complex));
	memcpy(&exp,
		params+4*sizeof(complex)+3*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
		sizeof(int));


	while (lista_totals.used < lista_totals.totals) {
		exito = 0;
		pthread_mutex_lock(&mutex_obtener);
		if ( lista_totals.used < lista_totals.totals )
		{	
			memcpy (totals,lista_totals.i_totals[lista_totals.used].coord,sizeof(inter));
			memcpy (&z,&lista_totals.comp[lista_totals.used],sizeof(complex));		
			lista_totals.used++;
			exito =1;
		} 
		pthread_mutex_unlock(&mutex_obtener);
		if (exito == 1) {
			/*Se va pintando la bmp_image por filas*/
			for (i = totals[0]; (i < totals[1]); i++) {
				/*Si el punto pertenece lo pintamos de negro*/
				if((it = in_julia(z, c, exp, max_it)) == max_it) {
					bmp_image->image[i][0] = 0;
					bmp_image->image[i][1] = 0;
					bmp_image->image[i][2] = 0;
				}
				else {
					it = color(it, max_it);
					bmp_image->image[i][0] = it;
					bmp_image->image[i][1] = it;
					bmp_image->image[i][2] = it + 88 < 255 ? it + 88 : 255;
				}
				/*Se obtiene el complex correspondiente al siguiente pixel*/
				if((i % bmp_image->cols) == 0) {
					z.real = min.real;
					z.img += inch;
				}
				else {
					z.real += incw;
				}
			}
		}
	}
	pthread_barrier_wait(&barrera);
}

/*
 * Se dibuja el conjunto de Julia en una bmp_image del tamaño especificado por medio de tantos hilos como se indique.
 * Parámetros:
 * 	- punto: punto que define el conjunto de julia a dibujar, Julia(punto)
 * 	- exp: exponente para la función con la si un punto pertenece al conjunto z' = z^exp 
 * 	- w, h: ancho y alto de la imágen
 * 	- min, max: complex que, respectivamente, representan las componentes real e imaginaria máximas y mínimas
 * 	- max_it: número máximo de iteraciones para calcular si un punto pertenece o no al conjunto de Mandelbrot
 * 	- n_procs: número de hilos que se utilizan para generar la imágen
 */
void draw_julia(complex punto, int exp, int w, int h, complex min, complex max, int max_it, 
	int n_procs, char *nombreImg) {

	double incw, inch; /*Incrementos para calcular el complex correspondiente a cada pixel*/
	tColorImage bmp_image, *ptrbmp_image;
	complex c; /*complex que va a ir correspondiendo a cada uno de los pixeles*/
	int it = 0; /*Iteraccion en la que el punto deja de considerarse en el conjuto de julia*/
	int pixelInicial, pixelFinal; /*Pixel inicial y final de cada procesador */
	int carga; /*Numero de pixeles que le corresponde a cada procesador*/
	void * params; /*Buffer para pasar los parámetros a cada procesador*/
	pthread_t threads[n_procs]; /*Procesos ligeros que generan la bmp_image*/
	int i = 0;

	/*Se obtiene el incremento para la parte real e imaginaria*/
	if(w < h) {
		incw = get_increment(w, min.real, max.real);
		inch = get_increment(h, min.img, max.img);
	}
	else {
		incw = get_increment(w, min.real, max.real);
		inch = get_increment(h, min.img, max.img);
	}

	/*Se crea la bmp_image*/
	BMP_Create_Color_Image(&bmp_image, h, w);

	/*Se calcula la carga que va a corresponder a cada procesador*/
	carga = w*h/n_procs;
	
	pthread_barrier_init(&barrera, NULL, n_procs);

	/*Calculamos los trozos a procesar*/
	lista_totals.i_totals = malloc (sizeof (inter)*n_procs);
	lista_totals.comp = malloc (sizeof (complex)*n_procs);
	for(i = 0; i < n_procs; i++) {
                /*Se calculan los píxeles inicial y final*/
                lista_totals.i_totals[i].coord[0] = i*carga;
		lista_totals.i_totals[i].coord[1] = (i == n_procs -1) ? w*h : (i+1)*carga;
		lista_totals.comp[i].real = min.real + incw * (lista_totals.i_totals[i].coord[0] % w);
		lista_totals.comp[i].img = min.img + inch * (lista_totals.i_totals[i].coord[0] / w);
	}
	lista_totals.totals = n_procs;
	lista_totals.used = 0;
	
	/*Cada procesador genera su parte de la bmp_image*/
	for(i = 0; i < n_procs; i++) {
		/*Se calculan los píxeles inicial y final*/
		pixelInicial = i*carga;
		pixelFinal = (i == n_procs -1) ? w*h : (i+1)*carga;
		//Se calcula el primer número complex que le va a tocar dibujar
		c.real = min.real + incw * (pixelInicial % w);
		c.img = min.img + inch * (pixelInicial / w);
		/*Se obtiene el puntero a la bmp_image*/
		ptrbmp_image = &bmp_image;
		/*Se copian los parámetros al hilo*/
		params = (void *)malloc(4*sizeof(complex) + 4*sizeof(int) + 2*sizeof(double) + sizeof(tColorImage *));
		memcpy(params,
			&c,sizeof(complex));
		memcpy(params+sizeof(complex),
			&pixelInicial,sizeof(int));
		memcpy(params+sizeof(complex)+sizeof(int),
			&pixelFinal,sizeof(int));
		memcpy(params+sizeof(complex)+2*sizeof(int),
			&incw,sizeof(double));
		memcpy(params+sizeof(complex)+2*sizeof(int)+sizeof(double),
			&inch,sizeof(double));
		memcpy(params+sizeof(complex)+2*sizeof(int)+2*sizeof(double),
			&ptrbmp_image,sizeof(tColorImage *));
		memcpy(params+sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
			&min,sizeof(complex));
		memcpy(params+2*sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
			&max,sizeof(complex));
		memcpy(params+3*sizeof(complex)+2*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
			&max_it,sizeof(int));
		memcpy(params+3*sizeof(complex)+3*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
			&punto,sizeof(complex));
		memcpy(params+4*sizeof(complex)+3*sizeof(int)+2*sizeof(double)+sizeof(tColorImage*),
			&exp,sizeof(int));		

		/*Se crea y comienza a ejecutar el hilo*/
		if (pthread_create(&threads[i], NULL, draw_julia_thread, params) != 0) {
			printf("Error: No se pudo crear el hilo %d.\n",(i));
			exit(2);
		}

		/*Se libera la memoria de los parámetros*/
		free(params);
	}
	/*Esperamos a que los procesos ligeros terminen de generar la bmp_image*/
	for(i = 0; i < n_procs; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_barrier_destroy(&barrera);

	free(lista_totals.i_totals);
	free(lista_totals.comp);

	/*Escribimos la bmp_image y liberamos la memoria*/
	BMP_Write_Color_Image(nombreImg, &bmp_image);
	BMP_Free_Color_Image(&bmp_image);
}

/*Dibuja un triángulo inverted a partir del punto p indicado, utilizando el color definido por las componentes r, g, b*/
void paint_triangle_inverted(tColorImage *bmp_image, int p, int altura, 
	unsigned char r, unsigned char g, unsigned char b) {
	
	/*Variables para recorrer la bmp_image*/
	int alturaCont, pixels;
	/*Se pinta el triángulo inverted coloreando los píxeles correspondientes*/
	for (alturaCont = 0; alturaCont < altura; alturaCont++){
		if (!((p < 0) || (p >= (bmp_image->cols * bmp_image->rows)))){
			bmp_image->image[p][0] = r;
			bmp_image->image[p][1] = g;
			bmp_image->image[p][2] = b;
			/*Se pintan los píxeles correspondientes a la fila*/
			for (pixels = 0; pixels <= alturaCont; pixels++){
				if (!(((p+pixels) < 0) || ((p+pixels) >= (bmp_image->cols * bmp_image->rows)))){
					bmp_image->image[p+pixels][0] = r;
					bmp_image->image[p+pixels][1] = g;
					bmp_image->image[p+pixels][2] = b;
				}
				if (!(((p-pixels) < 0) || ((p-pixels) >= (bmp_image->cols * bmp_image->rows)))){
					bmp_image->image[p-pixels][0] = r;
					bmp_image->image[p-pixels][1] = g;
					bmp_image->image[p-pixels][2] = b;	
				}
			}
			p += bmp_image->cols;
		}
	}
}

/*Pinta un triángulo del mismo modo que el triángulo inverted pero a la inversa*/
void paint_triangle(tColorImage *bmp_image, int p, int altura,
	unsigned char r, unsigned char g, unsigned char b) {

	/*Variables para recorrer la bmp_image*/
	int alturaCont, pixels;
	/*Se pinta el triángulo inverted coloreando los píxeles correspondientes*/
	for (alturaCont = 0; alturaCont < altura; alturaCont++){
		if (!((p < 0) || (p >= (bmp_image->cols * bmp_image->rows)))){
			bmp_image->image[p][0] = r;
			bmp_image->image[p][1] = g;
			bmp_image->image[p][2] = b;
			/*Se pintan los píxeles correspondientes a la fila*/
			for (pixels = 0; pixels < (altura-alturaCont-1); pixels++){
				if (!(((p+pixels) < 0) || ((p+pixels) >= (bmp_image->cols * bmp_image->rows)))){
					bmp_image->image[p+pixels][0] = r;
					bmp_image->image[p+pixels][1] = g;
					bmp_image->image[p+pixels][2] = b;
				}
				if (!(((p-pixels) < 0) || ((p-pixels) >= (bmp_image->cols * bmp_image->rows)))){
					bmp_image->image[p-pixels][0] = r;
					bmp_image->image[p-pixels][1] = g;
					bmp_image->image[p-pixels][2] = b;	
				}
			}
			p += bmp_image->cols;
		}
	}
}

/*
 * Se realizan las llamadas recurivas correspondientes para pintar el triángulo de Sierpinski, para ello se
 * calcula el punto en el que se debe calcular el triángulo inverted.
 */
void draw_sierpinski_recursivo(tColorImage *bmp_image, int punto, int altura, int iteracion, int maxIter, 
	unsigned char r, unsigned char g, unsigned char b) {
	
	/*Condición de parada*/
	if (iteracion < maxIter){
		paint_triangle_inverted(bmp_image,punto,altura,r,g,b);
		iteracion++;
		draw_sierpinski_recursivo(bmp_image, punto+(bmp_image->cols*altura), altura/2, iteracion, maxIter, r, g, b);
		draw_sierpinski_recursivo(bmp_image, punto+altura, altura/2, iteracion, maxIter, r, g, b);
		draw_sierpinski_recursivo(bmp_image, punto-altura, altura/2, iteracion, maxIter, r, g, b);
	}
}

/*Se genera un triángulo de sierpinski de base l, de it iteracciones, de color fractal y con fondo como color de fondo*/
void create_sierpinski(tColorImage* bmp_image, int l, int it, tColorPixel fractal, tColorPixel fondo) {
	
	/*Contador*/
	int i;

	/*Garantiza que la base sea par*/
	if (l%2 != 0) l--;
	/*Se crea la bmp_image*/
	BMP_Create_Color_Image(bmp_image, l/2, l);

	/*Pintamos la base del color de fondo*/
	for(i = 0; i < bmp_image->cols*bmp_image->rows; i++) {
		bmp_image->image[i][0] = fondo[0];
		bmp_image->image[i][1] = fondo[1];
		bmp_image->image[i][2] = fondo[2];
	}
	/*Pintamos el triángulo inicial*/
	paint_triangle(bmp_image,(l/2-1),(l/2+1), fractal[0], fractal[1], fractal[2]);

	/* Generamos el fractal */
	draw_sierpinski_recursivo(bmp_image, l/2-1, l/4, 1, it, fondo[0], fondo[1], fondo[2]);
}

/*
 * Dibuja un Sierpinski en la bmp_image de tamaño (wxh), de it iteraciones, de color fractal y fondo como color de fondo,
 * escribiéndolo en un File de nombre nombreImg.
 */
void draw_sierpinski(int w, int h, int it, tColorPixel fractal, tColorPixel fondo, char *nombreImg) {

	/*Índices para el bucle*/
	int l,i,j,f,c,k;
	/*Postprocesado para ajustar la bmp_image al tamaño*/
	int postprocesar;
	int copiar_desde_ancho;
	int copiar_desde_alto;
	/*Estructura para generar la bmp_image*/
	tColorImage img, bmp_image;
	
	k=0;
	/* Si el ancho es mayor que (2 * alto) entonces bmp_image final más ancha que bmp_image fractal */
	if (w > (2*h)) {
		l = 2*h;
		postprocesar = 1;
		copiar_desde_alto = 0;
		copiar_desde_ancho = (w - l) / 2;
	/* Si el ancho es menor que (2 * alto) entonces bmp_image final más baja que bmp_image fractal */
	} else if (w < (2*h)) {
		l = w;
		postprocesar = 2;
		copiar_desde_alto = (h - (l / 2)) / 2;
		copiar_desde_ancho = 0;
	/* Si el ancho es igual que (2 * alto) entonces bmp_image final igual que bmp_image fractal */	
	} else {
		l = w;
		postprocesar = 0;
	}

	/* Generamos la bmp_image */
	create_sierpinski(&img, l, it, fractal, fondo);

	/* Copiamos la bmp_image generada en la nueva */
	if (postprocesar != 0) {
		/*bmp_image generada del tamaño correspondiente*/
		BMP_Create_Color_Image(&bmp_image, h, w);
		for(i = 0, f = 0; f < bmp_image.rows; i+=bmp_image.cols, f++) {
			for(j = i, c = 0; c < bmp_image.cols; j++, c++) {
				/* Si el píxel está en la posición correcta se copia el de la bmp_image dibujada, si no
				 * se pinta del color del fondo */
				if((c < copiar_desde_ancho || c >= img.cols+copiar_desde_ancho) ||
					(f < copiar_desde_alto || f >= img.rows+copiar_desde_alto)) {
					bmp_image.image[j][0] = fondo[0];
					bmp_image.image[j][1] = fondo[1];
					bmp_image.image[j][2] = fondo[2];
				}
				else {
					bmp_image.image[j][0] = img.image[k][0];
					bmp_image.image[j][1] = img.image[k][1];
					bmp_image.image[j][2] = img.image[k][2];
					k++;
				}
			}
		}
		/*Se genera la bmp_image correspondiente*/
		BMP_Write_Color_Image(nombreImg, &bmp_image);
		BMP_Free_Color_Image(&bmp_image);
	} else {
		/*Se genera la bmp_image */
		BMP_Write_Color_Image(nombreImg, &img);
		BMP_Free_Color_Image(&img);
	}
}

/*
 * Genera una bmp_image de t-square, de forma recursiva, pintando un square_comp de lado l en el centro de la bmp_image,
 * a partir de pixel p, del fractal. La llamada recursiva viene limitada por it_max, siendo it la iteracion actual.
 */
void paint_square_comp(tColorImage *bmp_image, int p, int l, int it, int it_max, tColorPixel fractal) {
	int c, f, i, j;
	int lado;
	int siguiente[4];
	int eit; //2^(it-1)
	
	//Se calculan índices y direcciones
	eit = two_to_the(it-1);
	lado = (l/eit)/2;

	/*Se pinta el square_comp en el centro de la bmp_image a partir del punto p*/
	for(i = p, f = 0; f < l/eit; i+=l, f++)
		for(j = i, c = 0; c < l/eit; j++, c++) {
			if( ((f >= (lado / 2)) && (f <= (lado + (lado / 2)))) &&
				((c >= (lado / 2)) && (c <= (lado + (lado / 2))))) {
				bmp_image->image[j][0] = fractal[0];
				bmp_image->image[j][1] = fractal[1];
				bmp_image->image[j][2] = fractal[2];
			}
			/*Se calculan los puntos para pintar los square_comps en el siguiente nivel*/
			if(f == 0 && c == 0) siguiente[0] = j;
			if(f == lado && c == 0) siguiente[1] = j;
			if(f == 0 && c == lado) siguiente[2] = j;
			if(f == lado && c == lado) siguiente[3] = j;
		}
	/*Se hace la llamada recursiva*/
	if(it < it_max) { /*Condición de parada*/
		it++;
		for(i = 0; i < 4; i++) {
			paint_square_comp(bmp_image, siguiente[i], l, it, it_max, fractal);
		}
	}
}

/*Genera una representación gráfica del t-square en la bmp_image bmp_image, el lado del square_comp más grande es l, it es el
 * número de iteraciones para generarlo, fractal el color del fractal y fondo el color de fondo*/
void create_tsquare(tColorImage* bmp_image, int l, int it, tColorPixel fractal, tColorPixel fondo) {
	int i;

	/*Se crea la bmp_image*/
	BMP_Create_Color_Image(bmp_image, l, l);

	/*Pintamos la base de negro*/
	for(i = 0; i < l*l; i++) {
		bmp_image->image[i][0] = fondo[0];
		bmp_image->image[i][1] = fondo[1];
		bmp_image->image[i][2] = fondo[2];
	}

	/*Pintamos el square_comp en el centro*/
	paint_square_comp(bmp_image,0,l,1,it,fractal);
}

/*Genera la bmp_image con la representación del t-square en una bmp_image de tamaño (wxh), sien el fractal de color fractal
 * y el fondo de color fondo, la bmp_image se escribe en un File de nombreImg*/
void draw_tsquare(int w, int h, int it, tColorPixel fractal, tColorPixel fondo, char *nombreImg) {

	/*Índices para los bucles*/
	int l,i,j,f,c,k;
	/*Postprocesado para ajustar la bmp_image al tamaño*/
	int postprocesar;
	int copiar_desde_ancho;
	int copiar_desde_alto;
	/*bmp_image en la que se genera el fractal*/
	tColorImage img, bmp_image;
	
	k=0;
	/* Si el ancho es mayor que (2 * alto) entonces bmp_image final más ancha que bmp_image fractal */
	if (w > h) {
		l = h;
		postprocesar = 1;
		copiar_desde_alto = 0;
		copiar_desde_ancho = (w - l) / 2;
	/* Si el ancho es menor que (2 * alto) entonces bmp_image final más baja que bmp_image fractal */
	} else if (w < h) {
		l = w;
		postprocesar = 2;
		copiar_desde_alto = (h - l) / 2;
		copiar_desde_ancho = 0;
	/* Si el ancho es igual que (2 * alto) entonces bmp_image final igual que bmp_image fractal */	
	} else {
		l = w;
		postprocesar = 0;
	}

	/* Generamos la bmp_image */
	create_tsquare(&img, l, it, fractal, fondo);

	/* Copiamos la bmp_image generada en la nueva */
	if (postprocesar != 0) {
		BMP_Create_Color_Image(&bmp_image, h, w);
		//for(i = 0; i < bmp_image.rows*bmp_image.cols;
		for(i = 0, f = 0; f < bmp_image.rows; i+=bmp_image.cols, f++) {
			for(j = i, c = 0; c < bmp_image.cols; j++, c++) {
				/* Si el píxel está en la posición correcta se copia el de la bmp_image dibujada, si no
				 * se pinta del color del fondo */
				if((c < copiar_desde_ancho || c >= img.cols+copiar_desde_ancho) ||
					(f < copiar_desde_alto || f >= img.rows+copiar_desde_alto)) {
					bmp_image.image[j][0] = fondo[0];
					bmp_image.image[j][1] = fondo[1];
					bmp_image.image[j][2] = fondo[2];
				}
				else {
					bmp_image.image[j][0] = img.image[k][0];
					bmp_image.image[j][1] = img.image[k][1];
					bmp_image.image[j][2] = img.image[k][2];
					k++;
				}
			}
		}
		/*Se genera la bmp_image correspondiente*/
		BMP_Write_Color_Image(nombreImg, &bmp_image);
		BMP_Free_Color_Image(&bmp_image);
	} else {
		/*Se genera la bmp_image*/
		BMP_Write_Color_Image(nombreImg, &img);
		BMP_Free_Color_Image(&img);
	}
}


