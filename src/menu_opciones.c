/*
 * File: menu_opciones.c
 *
 * Authors:
 * 	Christian Felipe Álvarez
 * 	Lisardo Prieto González
 *
 */

#include "menu_opciones.h"

/* Función para mostrar el uso en caso de error por parámetros incorrectos */
void mostrar_uso(char *nombreProg){
	set_font_props_color(FONT_RED,FONT_BOLD);
	printf("\nError en la invocación: ");
	set_font_props_color(FONT_RED,FONT_NORMAL_INTENSISTY);
	printf("Parámetros incorrectos.\n\n");
	set_font_props_color(FONT_GREEN,FONT_BOLD);
	printf("   Ejecución:");
	set_font_props_color(FONT_BLUE,FONT_NORMAL_INTENSISTY);
	printf("  %s",nombreProg);
	set_font_props_color(FONT_GREEN,FONT_NORMAL_INTENSISTY);
	printf(" -f <fractal> [-w <ancho> -h <alto> [<otras_opc>]]\n\n");
	reset2defaults();
	printf("\tDonde:");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\t\t<fractal>");
	set_font_props_color(FONT_BLUE,FONT_BOLD);
	printf(" 1: Mandelbrot");
	reset2defaults();
	printf(", ");
	set_font_props_color(FONT_BROWN,FONT_BOLD);
	printf("2: Julia");
	reset2defaults();
	printf(", ");
	set_font_props_color(FONT_GREEN,FONT_BOLD);
	printf("3: Sierpinski");
	reset2defaults();
	printf(", ");
	set_font_props_color(FONT_RED,FONT_BOLD);
	printf("4: T-Square");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\t\t<ancho>");
	reset2defaults();
	printf(" Anchura en pixels de la bmp_image a generar");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\t\t<alto>");
	reset2defaults();
	printf(" Altura en pixels de la bmp_image a generar");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\n\t\t<otras_opc> OPCIONES ADICIONALES (mostradas a continuación):");
	printf("\n\t\t\t-p <num_proc>");
	reset2defaults();
	printf(" Número de procesos ligeros a emplear");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\t\t\t-i <num_iter>");
	reset2defaults();
	printf(" Número de iteraciones a generar");
	set_font_props_color(FONT_CYAN,FONT_BOLD);
	printf("\n\n\t\t\tOpciones para fractales tipo Mandelbrot y Julia:");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\t\t\t-dMax <complex>");
	reset2defaults();
	printf(" Delimitador máximo del fractal");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\t\t\t-dMin <complex>");
	reset2defaults();
	printf(" Delimitador mínimo del fractal");
	set_font_props_color(FONT_CYAN,FONT_BOLD);
	printf("\n\n\t\t\tOpciones exclusivas para Julia:");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\t\t\t-pj <complex>");
	reset2defaults();
	printf(" Generador del conjunto de Julia");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\n\t\t\t    <complex>");
	reset2defaults();
	printf(" Valor complex en la forma '");
	set_font_props_color(FONT_GREEN,FONT_NORMAL_INTENSISTY);
	printf("real,img");
	reset2defaults();
	printf("'.\n\t\t\t\t");
	set_font_props_color(FONT_GREEN,FONT_BOLD);
	printf("Ej: ");
	set_font_props_color(FONT_GREEN,FONT_NORMAL_INTENSISTY);
	printf("5,-3");
	set_font_props_color(FONT_CYAN,FONT_BOLD);
	printf("\n\n\t\t\tOpciones para fractales tipo Sierpinski y T-Square:");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\t\t\t-bg <color>");
	reset2defaults();
	printf(" Color del fondo de la bmp_image");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\t\t\t-fg <color>");
	reset2defaults();
	printf(" Color del fractal");
	set_font_props_color(FONT_MAGENTA,FONT_BOLD);
	printf("\n\n\t\t\t    <color>");
	reset2defaults();
	printf(" Es un color en la forma '");
	set_font_props_color(FONT_GREEN,FONT_NORMAL_INTENSISTY);
	printf("%cr,g,b%c",DELIMITADOR_COLOR_ABRIR,DELIMITADOR_COLOR_CERRAR);
	reset2defaults();
	printf("'.\n\t\t\t\t");
	set_font_props_color(FONT_GREEN,FONT_BOLD);
	printf("Ej: ");
	set_font_props_color(FONT_GREEN,FONT_NORMAL_INTENSISTY);
	printf("%c0,0,255%c\n\n",DELIMITADOR_COLOR_ABRIR,DELIMITADOR_COLOR_CERRAR);
	reset2defaults();
}

/*Obtiene un número a partir de una cadena*/
int obtener_numero(char *numIn, int *error) {
	char *finalPtr;
	int num = (int)strtol(numIn, &finalPtr, 10);

	/* Mal, no es número */
	if ((*finalPtr != '\0')||(strlen(numIn) < 1))
		*error = -1;
	/* Bien, es número */
	else
		*error = 0;
	
	return num;
}

/*Obtiene un complex a partir de una cadena*/
complex obtener_complex(char *complexIn, int *error) {
	complex comp;
	double real, img;
	char *finalPtr1, *finalPtr2;

	real = 0;
	img = 0;

	/* Si cadena de entrada vacÃ­a, error */
	if (strlen(complexIn) < 1){
		*error = -1;
	/* Si no... */
	} else {
		real = strtod(complexIn, &finalPtr1);
		/* Si no hay mÃ¡s caracteres o lo que hay no es un separador */
		if ((*finalPtr1 == '\0') || (*finalPtr1 != ',')) {
			*error = -2;
		/* Si no... */
		} else if (*finalPtr1 == ',') {
			finalPtr1++;
			img = strtod(finalPtr1, &finalPtr2);
			/* Si quedan mÃ¡s caracteres en la cadena de entrdada, error */
			if (*finalPtr2 != '\0')
				*error = -3;
			/* Si no, correcto */
			else
				*error = 0;
		}
	}
	comp.real = real;
	comp.img = img;
	return comp;
}

/*Obtiene un color a partir de una cadena*/
int obtener_color(char *colorIn, color col) {
	int r,g,b;	
	char *finalPtr1, *finalPtr2, *finalPtr3;

	/* Si el primer caracter no es la apertura */
	if (colorIn[0] != DELIMITADOR_COLOR_ABRIR) {
		return -1;
	/* Si empieza por '<' buscamos la primera componente (red, r) */
	} else {
		colorIn++;
		r = (int)strtol(colorIn, &finalPtr1, 10);

		/* Si lo que hay a continuación no es una coma */
		if (*finalPtr1 != ',') {
			return -2;
		} else {
			finalPtr1++;
			g = (int)strtol(finalPtr1, &finalPtr2, 10);
			
			/* Si lo que hay a continuación no es otra coma */
			if (*finalPtr2 != ',') {
				return -3;
			} else {
				finalPtr2++;
				b = (int)strtol(finalPtr2, &finalPtr3, 10);

				/* Si lo que hay a continuación no es un ']' */
				if (*finalPtr3 != DELIMITADOR_COLOR_CERRAR) {
					return -4;
				/* Si no, todo fue bien */				
				} else {

					/* Si colores obtenidos fuera de rango, error */
					if ((r < 0) || (r > 255) || (g < 0) || (g > 255) || (b < 0) || (b > 255)) {
						return -5;						

					/* Si no, ok */					
					} else {
						col[0] = (uchar)r;
						col[1] = (uchar)g;
						col[2] = (uchar)b;
					}
					return 0;
				}
			}
		}
	}
}

/*Parsea las opciones que ha introducido el usuario*/
int parsear_opciones(int argc, char **argv, config *conf){
	/* variable para almacenar posibles errores de parseo */
	int error_parser;

	/* Contador de parámetros, a partir del tipo de fractal */
	uchar params = 3;
	/* Flags para detectar si hay parámetros repetidos */
	uchar w_flag = 0;
	uchar h_flag = 0;
	uchar p_flag = 0;
	uchar i_flag = 0;
	uchar dmax_flag = 0;
	uchar dmin_flag = 0;
	uchar bg_flag = 0;
	uchar fg_flag = 0;
	uchar ptcj_flag = 0;

	/* Si el número de parámetros no está en rango */
	if ((argc < 2) || (argc > 16)) {
		/* Se marca la configuración como inválida y se devuelve error */
		conf->tipoFractal = 250;
		return -1;	
	} else {
		/* En caso de haber opciones, la primera debe ser -f */
		if ((argc >= 3) && (strcmp(argv[1],"-f") == 0)) {
			conf->tipoFractal = obtener_numero(argv[2], &error_parser);
			/* Si no hubo error al parsear, se eliminan las opciones correspondientes */
			if ((error_parser == 0) && (conf->tipoFractal > 0) && (conf->tipoFractal < 5)) {
				/* Si se eligió Mandelbrot o Julia, bg y fg no tienen sentido */
				if (conf->tipoFractal == 1) {
					bg_flag = 1;
					fg_flag = 1;
					/* El punto generador del conjunto de julia no debe buscarse en mandelbrot */					
					ptcj_flag = 1;
				} else if (conf->tipoFractal  == 2) {
					bg_flag = 1;
					fg_flag = 1;
				/* Si por el contrario se eligió Sierpinski o T-Square, no pueden utilizarse
				 * dmax ni dmin */
				} else {
					dmax_flag = 1;
					dmin_flag = 1;
					ptcj_flag = 1;
				}
			/* Si ocurrió error al obtener el tipo de fractal, o no es un fractal válido, error */
			} else {
				/* Se marca la configuración como inválida y se devuelve error */
				conf->tipoFractal = 250;
				return -1;
			}
		/* Si no se especifica al principio el tipo de fractal, error */		
		} else {
			/* Se marca la configuración como inválida y se devuelve error */
			conf->tipoFractal = 250;
			return -1;
		}

		while (params < argc) {
			/* Si se detecta la anchura (width) y no se ha especificado previamente */
			if ((strcmp(argv[params],"-w") == 0) && (w_flag == 0)) {
				/* Si la opción viene acompañada de un número, ok */				
				if (params+1 < argc) {
					conf->ancho = obtener_numero(argv[params+1], &error_parser);
					/* Si no hubo error al parsear, ok */
					if (error_parser == 0) {
						/*Si la dimensión de la bmp_image es menor o igual a 0*/
						if (conf->ancho <= 0) {
							/* Se marca la configuración como inválida y se devuelve error */
							conf->tipoFractal = 250;
							return -1;
						/* Si la anchura especificada no es par, se hace par para 
						 * evitar los problemas con la librería de bmp_image */
						} else if (conf->ancho % 2 != 0) {
							conf->ancho++;
							w_flag = 1;
						} else {
							w_flag = 1;
						}
					/* Si hubo error al parsear */					
					} else {
						/* Se marca la configuración como inválida y se devuelve error */
						conf->tipoFractal = 250;
						return -1;
					}

				/* Si después del -w no hay número, error */
				} else {
					/* Se marca la configuración como inválida y se devuelve error */
					conf->tipoFractal = 250;
					return -1;
				}

			/* Si se detecta la altura (height) y no se ha especificado previamente */
			} else if ((strcmp(argv[params],"-h") == 0) && (h_flag == 0)) {
				/* Si la opción viene acompañada de un número, ok */				
				if (params+1 < argc) {
					conf->alto = obtener_numero(argv[params+1], &error_parser);
					/* Si no hubo error al parsear, ok */
					if (error_parser == 0) {
						/*Si la dimensión de la bmp_image es menor o igual a 0*/
						if (conf->alto <= 0) {
							/* Se marca la configuración como inválida y se devuelve error */
							conf->tipoFractal = 250;
							return -1;
						/* Si la altura especificada no es par, se hace par para 
						 * evitar los problemas con la librería de bmp_image */
						} else if (conf->alto % 2 != 0) {
							conf->alto++;
							h_flag = 1;
						} else {
							h_flag = 1;
						}
					/* Si hubo error al parsear */					
					} else {
						/* Se marca la configuración como inválida y se devuelve error */
						conf->tipoFractal = 250;
						return -1;
					}

				/* Si después del -h no hay número, error */
				} else {
					/* Se marca la configuración como inválida y se devuelve error */
					conf->tipoFractal = 250;
					return -1;
				}

			/* Si se detecta el número de procesadores y no se ha especificado previamente */
			} else if ((strcmp(argv[params],"-p") == 0) && (p_flag == 0)) {
				/* Si la opción viene acompañada de un número, ok */				
				if (params+1 < argc) {
					conf->numProcesadores = obtener_numero(argv[params+1], &error_parser);
					/* Si no hubo error al parsear, ok */
					if (error_parser == 0) {
						p_flag = 1;
					/* Si hubo error al parsear */					
					} else {
						/* Se marca la configuración como inválida y se devuelve error */
						conf->tipoFractal = 250;
						return -1;
					}

				/* Si después del -p no hay número, error */
				} else {
					/* Se marca la configuración como inválida y se devuelve error */
					conf->tipoFractal = 250;
					return -1;
				}

			/* Si se detecta el núm de iteraciones y no se ha especificado previamente */
			} else if ((strcmp(argv[params],"-i") == 0) && (i_flag == 0)) {
				/* Si la opción viene acompañada de un número, ok */				
				if (params+1 < argc) {
					conf->numIter = obtener_numero(argv[params+1], &error_parser);
					/* Si no hubo error al parsear, ok */
					if (error_parser == 0) {
						i_flag = 1;
					/* Si hubo error al parsear */					
					} else {
						/* Se marca la configuración como inválida y se devuelve error */
						conf->tipoFractal = 250;
						return -1;
					}

				/* Si después del -i no hay número, error */
				} else {
					/* Se marca la configuración como inválida y se devuelve error */
					conf->tipoFractal = 250;
					return -1;
				}

			/* Si se detecta el delimitador máximo (julia o mandelbrot) y no se ha especificado 
			 * previamente*/
			} else if (((strcmp(argv[params],"-dMax") == 0) || (strcmp(argv[params],"-dmax") == 0)) && 
				(dmax_flag == 0)) {
				/* Si la opción viene acompañada de un complex, ok */				
				if (params+1 < argc) {
					conf->dMax = obtener_complex(argv[params+1], &error_parser);
					/* Si no hubo error al parsear, ok */
					if (error_parser == 0) {
						dmax_flag = 1;
					/* Si hubo error al parsear */					
					} else {
						/* Se marca la configuración como inválida y se devuelve error */
						conf->tipoFractal = 250;
						return -1;
					}

				/* Si después del -dmax no hay complex, error */
				} else {
					/* Se marca la configuración como inválida y se devuelve error */
					conf->tipoFractal = 250;
					return -1;
				}

			/* Si se detecta el delimitador mÃ­nimo (julia o mandelbrot) y no se ha especificado 
			 * previamente */
			} else if (((strcmp(argv[params],"-dMin") == 0) || (strcmp(argv[params],"-dmin") == 0)) && (dmin_flag == 0)) {
				/* Si la opción viene acompañada de un complex, ok */				
				if (params+1 < argc) {
					conf->dMin = obtener_complex(argv[params+1], &error_parser);
					/* Si no hubo error al parsear, ok */
					if (error_parser == 0) {
						dmin_flag = 1;
					/* Si hubo error al parsear */					
					} else {
						/* Se marca la configuración como inválida y se devuelve error */
						conf->tipoFractal = 250;
						return -1;
					}

				/* Si después del -dmin no hay complex, error */
				} else {
					/* Se marca la configuración como inválida y se devuelve error */
					conf->tipoFractal = 250;
					return -1;
				}

			/* Si se detecta el punto de generación del conjunto de julia y no se ha especificado 
			 * previamente */
			} else if ((strcmp(argv[params],"-pj") == 0)  && (ptcj_flag == 0) && (conf->tipoFractal == 2)) {
				/* Si la opción viene acompañada de un complex, ok */				
				if (params+1 < argc) {
					conf->pj = obtener_complex(argv[params+1], &error_parser);
					/* Si no hubo error al parsear, ok */
					if (error_parser == 0) {
						dmin_flag = 1;
					/* Si hubo error al parsear */					
					} else {
						/* Se marca la configuración como inválida y se devuelve error */
						conf->tipoFractal = 250;
						return -1;
					}

				/* Si después del -pj no hay complex, error */
				} else {
					/* Se marca la configuración como inválida y se devuelve error */
					conf->tipoFractal = 250;
					return -1;
				}

			/* Si se detecta el color de fondo (sierpinski o t-square) y no se ha especificado previamente */
			} else if ((strcmp(argv[params],"-bg") == 0) && (bg_flag == 0)) {
				/* Si la opción viene acompañada de un color, ok */				
				if (params+1 < argc) {
					error_parser = obtener_color(argv[params+1], conf->bg);
					/* Si no hubo error al parsear, ok */
					if (error_parser == 0) {
						bg_flag = 1;
					/* Si hubo error al parsear */					
					} else {
						/* Se marca la configuración como inválida y se devuelve error */
						conf->tipoFractal = 250;
						return -1;
					}

				/* Si después del -bg no hay un color, error */
				} else {
					/* Se marca la configuración como inválida y se devuelve error */
					conf->tipoFractal = 250;
					return -1;
				}

			/* Si se detecta el color de fractal (sierpinski o t-square) y no se ha 
			 * especificado previamente */
			} else if ((strcmp(argv[params],"-fg") == 0) && (fg_flag == 0)) {
				/* Si la opción viene acompañada de un color, ok */				
				if (params+1 < argc) {
					error_parser = obtener_color(argv[params+1], conf->fg);
					/* Si no hubo error al parsear, ok */
					if (error_parser == 0) {
						fg_flag = 1;
					/* Si hubo error al parsear */					
					} else {
						/* Se marca la configuración como inválida y se devuelve error */
						conf->tipoFractal = 250;
						return -1;
					}

				/* Si después del -bg no hay un color, error */
				} else {
					/* Se marca la configuración como inválida y se devuelve error */
					conf->tipoFractal = 250;
					return -1;
				}

			/* Si estaban usadas todas las opciones, o es incorrecta, error */
			} else {
				/* Se marca la configuración como inválida y se devuelve error */
				conf->tipoFractal = 250;
				return -1;
			}

			/* Incrementamos en dos, por parámetro y valor procesado */
			params += 2;

		} /* Fin del bucle */

		/* Vamos a poner los valores por defecto para los parámetros genéricos no especificados */
		/* Anchura */
		if (w_flag == 0) {
			conf->ancho = DEF_WIDTH;
		}
		/* Altura */
		if (h_flag == 0) {
			conf->alto = DEF_HEIGHT;
		}
		/* Procesadores */
		if (p_flag == 0) {
			conf->numProcesadores = DEF_PROCS;
		}
		/* Iteraciones */
		if (i_flag == 0) {
			if ((conf->tipoFractal == 1) || (conf->tipoFractal == 2))
				conf->numIter = DEF_ITERS_MJ;
			else
				conf->numIter = DEF_ITERS_ST;
		}

		/* Si el fractal elegido es mandelbrot */
		if (conf->tipoFractal == 1) { 
			if (dmax_flag == 0) {
				conf->dMax.real = DEF_DMAX_REAL_MANDELBROT;
				conf->dMax.img = DEF_DMAX_IMAG_MANDELBROT;
			}
			if (dmin_flag == 0) {
				conf->dMin.real = DEF_DMIN_REAL_MANDELBROT;
				conf->dMin.img = DEF_DMIN_IMAG_MANDELBROT;
			}

		/* Si el fractal elegido es julia */
		} else if (conf->tipoFractal == 2) {
			if (dmax_flag == 0) {
				conf->dMax.real = DEF_DMAX_REAL_JULIA;
				conf->dMax.img = DEF_DMAX_IMAG_JULIA;
			}
			if (dmin_flag == 0) {
				conf->dMin.real = DEF_DMIN_REAL_JULIA;
				conf->dMin.img = DEF_DMIN_IMAG_JULIA;
			}
			if (ptcj_flag == 0) {
				conf->pj.real = DEF_PJ_REAL_JULIA;
				conf->pj.img = DEF_PJ_IMAG_JULIA;
			}

		/* Si el fractal elegido es sierpinski o t-square */
		} else if ((conf->tipoFractal == 3) || (conf->tipoFractal  == 4)) {
			if (bg_flag == 0){
				conf->bg[0] = DEF_BG_RED;
				conf->bg[1] = DEF_BG_GREEN;
				conf->bg[2] = DEF_BG_BLUE;
			}
			if (fg_flag == 0){
				conf->fg[0] = DEF_FG_RED;
				conf->fg[1] = DEF_FG_GREEN;
				conf->fg[2] = DEF_FG_BLUE;
			}
		}

		return 0;

	} /* Fin si argumentos en rango */
}

/*Muestra un complex por pantalla*/
void mostrar_complex(complex comp) {
	printf("'%.3lf,%.3lf'", comp.real, comp.img);
}

/*Muestra un color por pantalla*/
void mostrar_color(color col){
	printf("<%d,%d,%d>", col[0], col[1], col[2]);
}

/*Muestra la configuración por pantalla*/
void mostrar_config(config conf){
	printf("\n----- Configuración -----\n");
	printf("- Tipo fractal: %d\n", conf.tipoFractal);
	printf("- Ancho: %d\n", conf.ancho);
	printf("- Alto: %d\n", conf.alto);
	printf("- Procesadores: %d\n", conf.numProcesadores);
	printf("- Iteraciones: %d\n", conf.numIter);

	if ((conf.tipoFractal == 1) || (conf.tipoFractal == 2)) {
		printf("- Delimitador max: ");
		mostrar_complex(conf.dMax);
		printf("\n- Delimitador min: ");
		mostrar_complex(conf.dMin);
		if (conf.tipoFractal == 2) {
			printf("\n- Pto. generador del cjto. Julia: ");
			mostrar_complex(conf.pj);
		}
	} else {	
		printf("- Color fractal: ");
		mostrar_color(conf.fg);
		printf("\n- Color fondo: ");
		mostrar_color(conf.bg);
	}
	printf("\n-------------------------\n\n");
}

/*----------- Funciones para medir tiempo de ejecución ------------*/

/*Hace que el tiempo empiece a correr, devuelve la variable a utilizar en obtener_tiempo y para_cronometro  */
struct timeval* iniciar_cronometro() {

	struct timeval* h;

	h = (struct timeval*)malloc(sizeof(struct timeval));
	gettimeofday(h,NULL);
	return h;
}

/*Devuelve el tiempo transcurrido en segundos*/
double obtener_tiempo(struct timeval* inicio) {

	struct timeval* f;
	double t;

	f = (struct timeval*)malloc(sizeof(struct timeval));
	gettimeofday(f,NULL);
	t = ((double)((double)f->tv_sec - (double)inicio->tv_sec))*1000000;
	t += ((double)((double)f->tv_usec - (double)inicio->tv_usec));

	return t/1000000.0;
}

/*Libera la memoria del cronometro y lo pone*/
void liberar_cronometro(struct timeval* inicio) {
	free(inicio);

}
/*----------- Fin de funciones para medir tiempo de ejecución ------------*/

/*Genera un log a partir de los datos especificados (configuración, fechas de inicio y fin, y tiempo) y lo escribe en
 * File_log*/
void create_log(config conf, char* fecha_ini, char* fecha_fin, double tiempo, char* File_log) {
	FILE *fd;

	if((fd = fopen(File_log, "w")) == NULL)
		perror("ERROR en la generación del File de logs");
	else  {
		switch(conf.tipoFractal) {
			case 1 :
				fprintf(fd,"------------------- Conjunto de Mandelbrot -------------------\n");
				fprintf(fd,"- Dimensiones: %d x %d\n",conf.ancho,conf.alto);
				fprintf(fd,"- Número de procesos ligeros: %d\n",conf.numProcesadores);
				fprintf(fd,"- Número de iteraciones: %d\n",conf.numIter);
				fprintf(fd,"- Delimitador máximo: (%.3lf + i · %.3lf)\n",conf.dMax.real, conf.dMax.img);
				fprintf(fd,"- Delimitador mínimo: (%.3lf + i · %.3lf)\n",conf.dMin.real, conf.dMin.img);
			break;
			case 2 :
				fprintf(fd,"------------------- Conjunto de Julia -------------------\n");
				fprintf(fd,"- Dimensiones: %d x %d\n",conf.ancho,conf.alto);
				fprintf(fd,"- Número de procesadores: %d\n",conf.numProcesadores);
				fprintf(fd,"- Número de iteraciones: %d\n",conf.numIter);
				fprintf(fd,"- Delimitador máximo: (%.3lf + i · %.3lf)\n",conf.dMax.real, conf.dMax.img);
				fprintf(fd,"- Delimitador mínimo: (%.3lf + i · %.3lf)\n",conf.dMin.real, conf.dMin.img);
				fprintf(fd,"- Punto generador del conjunto: (%.3lf + i · %.3lf)\n",conf.pj.real, conf.pj.img);
			break;
			case 3 :
				fprintf(fd,"------------------- Triángulo de Sierpinski -------------------\n");
				fprintf(fd,"- Dimensiones: %d x %d\n",conf.ancho,conf.alto);
				fprintf(fd,"- Número de procesadores: %d\n",conf.numProcesadores);
				fprintf(fd,"- Número de iteraciones: %d\n",conf.numIter);
				fprintf(fd,"- Color del fractal: <%d,%d,%d>\n",conf.fg[0], conf.fg[1], conf.fg[2]);
				fprintf(fd,"- Color del fondo: <%d,%d,%d>\n",conf.bg[0], conf.bg[1], conf.bg[2]);
				
			break;
			case 4 :
				fprintf(fd,"------------------- T-Square -------------------\n");
				fprintf(fd,"- Dimensiones: %d x %d\n",conf.ancho,conf.alto);
				fprintf(fd,"- Número de procesadores: %d\n",conf.numProcesadores);
				fprintf(fd,"- Número de iteraciones: %d\n",conf.numIter);
				fprintf(fd,"- Color del fractal: <%d,%d,%d>\n",conf.fg[0], conf.fg[1], conf.fg[2]);
				fprintf(fd,"- Color del fondo: <%d,%d,%d>\n",conf.bg[0], conf.bg[1], conf.bg[2]);
				
			break;
		}
		fprintf(fd,"- Fecha de inicio de la generación del fractal: %s\n",fecha_ini);
		fprintf(fd,"- Fecha de fin de generación del fractal: %s\n",fecha_fin);
		fprintf(fd,"- Tiempo de ejecución: %f segundos\n\n",tiempo);
		fclose(fd);
	}
}

/*Genera los nombres de los Files a partir de una configuración*/
void genera_nombres_Files(config conf, char *nombreFract, char *nombreLog) {

	/*Parámetros para obtener la fecha*/
	char fecha[255];
	time_t t;
	struct tm *tmp;
	t = time(NULL);
	tmp = localtime(&t);
	strftime(fecha, sizeof(char)*255, "%Y.%m.%d-%H.%M.%S", tmp);

	switch (conf.tipoFractal){
		case 1:
			sprintf(nombreFract, "[%s]_mandelbrot_i%d_(%dx%d).bmp"
				,fecha, conf.numIter, conf.alto, conf.ancho);
			sprintf(nombreLog, "[%s]_mandelbrot_i%d_(%dx%d).log"
				,fecha, conf.numIter, conf.alto, conf.ancho);
			break;
		case 2:
			sprintf(nombreFract, "[%s]_julia_i%d_(%dx%d).bmp"
				,fecha, conf.numIter, conf.alto, conf.ancho);
			sprintf(nombreLog, "[%s]_julia_i%d_(%dx%d).log"
				,fecha, conf.numIter, conf.alto, conf.ancho);
			break;
		case 3:
			sprintf(nombreFract, "[%s]_sierpinski_i%d_(%dx%d).bmp"
				,fecha, conf.numIter, conf.alto, conf.ancho);
			sprintf(nombreLog, "[%s]_sierpinski_i%d_(%dx%d).log"
				,fecha, conf.numIter, conf.alto, conf.ancho);
			break;
		case 4:
			sprintf(nombreFract, "[%s]_t-square_i%d_(%dx%d).bmp"
				,fecha, conf.numIter, conf.alto, conf.ancho);
			sprintf(nombreLog, "[%s]_t-square_i%d_(%dx%d).log"
				,fecha, conf.numIter, conf.alto, conf.ancho);
	}

}

/*Ejecuta con las operaciones especificadas por el usuario*/
int ejecuta_fractus(int argc, char **argv) {
	
	/*Configuración, posible error y nombres de Files*/
	config confFractus;
	int error;
	char *nombreFileFractal, *nombreFileLog;
	/*Variables para medir tiempos*/
	double tiempoEjecucion;
	char fechaIni[255], fechaFin[255];
	struct timeval *crono;
	/*Variables para obtener la fecha*/
	char fecha[255];
	time_t t;
	struct tm *tmp;
	
	/*Se obtiene las opciones que ha especificado*/
	error =	parsear_opciones(argc, argv, &confFractus);
	
	/*Si no hay errores en los datos de entrada*/
	if (error == 0) {
		/*Se especifica el formato de la fecha*/
		t = time(NULL);
		tmp = localtime(&t);
		strftime(fechaIni, sizeof(char)*255, "%Y/%m/%d - %H:%M:%S", tmp);
		/*Nombres de los Files*/
		nombreFileFractal = (char *)malloc(4*200);
		nombreFileLog = (char *)malloc(4*200);
		genera_nombres_Files(confFractus, nombreFileFractal, nombreFileLog);
		/*Inicio de la toma de tiempo de ejecución*/
		crono = iniciar_cronometro();
		switch (confFractus.tipoFractal) {
			case 1: /*Conjunto de Mandelbrot*/
				draw_mandelbrot(confFractus.ancho, 
					confFractus.alto, 
					confFractus.dMin, 
					confFractus.dMax, 
					confFractus.numIter, 
					confFractus.numProcesadores, 
					nombreFileFractal);
				break;
			case 2: /*Conjundo de Julia*/
				draw_julia(confFractus.pj, 
					2, 
					confFractus.ancho, 
					confFractus.alto, 
					confFractus.dMin, 
					confFractus.dMax, 
					confFractus.numIter, 
					confFractus.numProcesadores, 
					nombreFileFractal);
				break;
			case 3: /*Triángulo de Sierpinski*/
				draw_sierpinski(confFractus.ancho, 
				confFractus.alto, 
				confFractus.numIter, 
				confFractus.fg, 
				confFractus.bg, 
				nombreFileFractal);
				break;
			case 4: /*T-square*/
				draw_tsquare(confFractus.ancho, 
				confFractus.alto, 
				confFractus.numIter, 
				confFractus.fg, 
				confFractus.bg, 
				nombreFileFractal);
		}
		tiempoEjecucion = obtener_tiempo(crono);
		strftime(fechaFin, sizeof(char)*255, "%Y/%m/%d - %H:%M:%S", tmp);
		/*Se genera y escribe el File de log*/
		create_log(confFractus, fechaIni, fechaFin, tiempoEjecucion, nombreFileLog);
		/*Se libera memoria*/
		liberar_cronometro(crono);
		free(nombreFileFractal);
		free(nombreFileLog);
		return 0;
	/*Si hay algún error se muestra la ayuda*/
	} else {
		mostrar_uso(argv[0]);
		return -1;
	}
}

/*void probar_parsers() {
	int error;

	complex comp;
	color col;
	int ent;
	
	char *compSt, *colSt, *entSt;

	compSt = " 57.7,    -29.17";
	colSt = "<  -15,  2, 3>";
	entSt = "  25";

	comp = obtener_complex(compSt, &error);
	printf("\n- complex (%lf,%lf). Error: %d\n", comp.real, comp.img, error);
	error = obtener_color(colSt, col);
	printf("- Color <%d,%d,%d>. Error: %d\n", col[0], col[1], col[2] , error);
	ent = obtener_numero(entSt, &error);
	printf("- Entero %d. Error: %d\n", ent , error);
}*/
