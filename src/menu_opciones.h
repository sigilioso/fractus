/*
 * File: menu_opciones.h
 *
 * Se definen las funciones necesarias para el tratamiento de datos de entrada y los valores por defecto para la
 * generación de imágenes.
 *
 * Authors:
 * 	Christian Felipe Álvarez
 * 	Lisardo Prieto González
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fractals.h"
#include "conio_linux.h"

#ifndef _MENU_OPCIONES_H
#define _MENU_OPCIONES_H

/* Valores por defecto */
#define DEF_PROCS 1
#define DEF_ITERS_MJ 150
#define DEF_ITERS_ST 5
#define DEF_WIDTH 800
#define DEF_HEIGHT 600
#define DEF_DMAX_REAL_MANDELBROT 0.7
#define DEF_DMAX_IMAG_MANDELBROT 1.5
#define DEF_DMIN_REAL_MANDELBROT -2
#define DEF_DMIN_IMAG_MANDELBROT -1.5
#define DEF_DMAX_REAL_JULIA 2
#define DEF_DMAX_IMAG_JULIA 1
#define DEF_DMIN_REAL_JULIA -2
#define DEF_DMIN_IMAG_JULIA -1
#define DEF_PJ_REAL_JULIA -0.70176
#define DEF_PJ_IMAG_JULIA -0.3842
#define DEF_BG_RED 0
#define DEF_BG_GREEN 0
#define DEF_BG_BLUE 0
#define DEF_FG_RED 255
#define DEF_FG_GREEN 255
#define DEF_FG_BLUE 255

#define DELIMITADOR_COLOR_ABRIR '['
#define DELIMITADOR_COLOR_CERRAR ']'

/*Alias para definir más cómodamente los unsigned char*/
typedef unsigned char uchar;

/*Estructura para el color, equivalente a tColorPixel*/
typedef uchar color[3];

/*Definición de la configuración*/
typedef struct {
	uchar tipoFractal;
	int ancho;
	int alto;
	int numProcesadores;
	int numIter;
	complex dMax;
	complex dMin;
	complex pj;
	color bg;
	color fg;
} config;

/* Función para mostrar el uso en caso de error por parámetros incorrectos */
void mostrar_uso(char *nombreProg);

/*Obtiene un número a partir de una cadena*/
int numero(char *numIn, int *error);

/*Obtiene un complex a partir de una cadena*/
int obtener_numero(char *numIn, int *error);

/*Obtiene un color a partir de una cadena*/
complex obtener_complex(char *complexIn, int *error);

/*Obtiene un color a partir de una cadena de caracteres*/
int obtener_color(char *colorIn, color col);

/*Parsea las opciones que ha introducido el usuario*/
int parsear_opciones(int argc, char **argv, config *conf);

/*Muestra un complex por pantalla*/
void mostrar_complex(complex comp);

/*Muestra un color por pantalla*/
void mostrar_color(color col);

/*Muestra la configuración por pantalla*/
void mostrar_config(config conf);

/*Genera un log a partir de los datos especificados (configuración, fechas de inicio y fin, y tiempo) y lo escribe en
 * File_log*/
void create_log(config conf, char* fecha_ini, char* fecha_fin, double tiempo, char* File_log);

/*Genera los nombres de los Files a partir de una configuración*/
void genera_nombres_Files(config conf, char *nombreFract, char *nombreLog);

/*Ejecuta con las operaciones especificadas por el usuario*/
int ejecuta_fractus(int argc, char **argv);

/*Función para test*/
/*void probar_parsers();*/
#endif
