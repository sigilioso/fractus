# Fractus: Fractals Generator 
#
# Authors:
#	Christian Felipe Álvarez <c.felipealvarez@gmail.com>	
#	Lisardo Prieto González

# Compiler 
CC = gcc

# Flags (to show warnings, O3 optimization, threads)
CFLAGS = -lm -Wall -O3 -lpthread -ansi

# Nombre del programa
PROGNAME = fractus

# Cleaning binary code and objects
RM = rm -rf $(PATHDEST)

# Object(s) to compile 
SRC0 = conio_linux.c
SRC1 = bmp_image.c
SRC2 = complex.c
SRC3 = fractals.c
SRC4 = fractus.c
SRC5 = menu_opciones.c
HEA0 = conio_linux.h
HEA1 = bmp_image.h
HEA2 = complex.h
HEA3 = menu_opciones.h
OBJ0 = $(PATHOBJS)conio_linux.o
OBJ1 = $(PATHOBJS)bmp_image.o
OBJ2 = $(PATHOBJS)complex.o
OBJ3 = $(PATHOBJS)fractals.o
OBJ4 = $(PATHOBJS)fractus.o
OBJ5 = $(PATHOBJS)menu_opciones.o

# Source path 
PATHORIG = ./src/

# Binary code path 
PATHDEST = ./bin/

# Objects 
PATHOBJS = ./bin/.o/

# Path to install the program 
USRBIN = /usr/bin/

# Name and path of fractus 
PROG1 = $(PATHDEST)$(PROGNAME)

# Binary code to build 
PROGS = $(PROG1)

# Go! Compile everything 
all: $(PROGS)

# Rules to compile the source code 
FILESPROG = $(OBJ0) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(PATHORIG)$(HEA1) $(PATHORIG)$(HEA2) $(PATHORIG)$(HEA3)

$(PROG1): $(PATHDEST) $(PATHOBJS) $(OBJ0) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(PATHORIG)$(SRC0) $(PATHORIG)$(SRC1) $(PATHORIG)$(SRC2) $(PATHORIG)$(SRC3) $(PATHORIG)$(SRC4) $(PATHORIG)$(SRC5)
	@mv *.o $(PATHOBJS);
	$(CC) $(CFLAGS) -o $(PROG1) $(FILESPROG)

$(PATHDEST):
	@mkdir ./bin

$(PATHOBJS):
	@mkdir ./bin/.o

$(OBJ0): $(PATHORIG)$(SRC0) $(PATHORIG)$(HEA0)
	$(CC) -c $(FLAGS) $(PATHORIG)$(SRC0)
	
$(OBJ1): $(PATHORIG)$(SRC1) $(PATHORIG)$(HEA1)
	$(CC) -c $(FLAGS) $(PATHORIG)$(SRC1)

$(OBJ2): $(PATHORIG)$(SRC2) $(PATHORIG)$(HEA2)
	$(CC) -c $(FLAGS) $(PATHORIG)$(SRC2)

$(OBJ3): $(PATHORIG)$(SRC3) $(PATHORIG)$(HEA1) $(PATHORIG)$(HEA2)
	$(CC) -c $(FLAGS) $(PATHORIG)$(SRC3)

$(OBJ4): $(PATHORIG)$(SRC4) $(PATHORIG)$(HEA0) $(PATHORIG)$(HEA1) $(PATHORIG)$(HEA2)
	$(CC) -c $(FLAGS) $(PATHORIG)$(SRC4)

$(OBJ5): $(PATHORIG)$(SRC5) $(PATHORIG)$(HEA0) $(PATHORIG)$(HEA1) $(PATHORIG)$(HEA2) $(PATHORIG)$(HEA3)
	$(CC) -c $(FLAGS) $(PATHORIG)$(SRC5)

# Cleaning 
clean:
	$(RM)

# Installing
install:
	@cp $(PROG1) $(USRBIN)
	@echo "Instalación completa"

# Uninstalling
uninstall:
	@rm -f  $(USRBIN)$(PROGNAME)
	@echo "$(PROGNAME) desinstalado con éxito"
