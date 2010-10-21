/*
 * CONIO_LINUX
 *
 * Utilities to manipulate text properties on Linux Console
 *
 * Authors:
 * 	Christian Felipe Álvarez
 * 	Lisardo Prieto González
 */

#include "conio_linux.h"

/*Set specified text color*/
void set_font_color(int color){
	if (!((color < 30) || (color > 37)))
		printf("\033[%im",color);
}

/*Set specified background color*/
void set_bg_color(int color){
	if (!((color < 40) || (color > 47)))
		printf("\033[%im",color);
}

/*Set specified text and background color*/
void set_font_bg_color(int f_color, int bg_color){
	if ( !(((bg_color < 40) || (bg_color > 47)) || ((f_color < 30) || (f_color > 37))) )
		printf("\033[%i;%im",bg_color, f_color);
}

/*Set specified font property*/
void set_font_props(int props){
	if (((props ==1) || (props ==2) || (props == 4) || (props ==5) || (props ==22)))
		printf("\033[%im",props);
}

/*Set specified color and prop to text*/
void set_font_props_color(int f_color, int props){
	if (!((f_color < 30) || (f_color > 37)))
		if (((props ==1) || (props ==2) || (props == 4) || (props ==5) || (props ==22)))
			printf("\033[%i;%im", props, f_color);
}

/*Reset text properties to default*/
void reset2defaults(){
	printf("\033[0m");
}

/*Do this system call: "reset"*/
void reset_console(){
	system("reset");
}

/* TEST */
/*
int main(){
	reset_console();
	int i,j;
	for (i=30;i<38;i++){
		for (j=1;j<23;j++){
			set_font_props_color(i,j);
			if (((j ==1) || (j ==2) || (j == 4) || (j ==5) || (j ==22)))
				printf("Hola en color\n");
			reset2defaults();
		}
	}
	sleep(2);
}
*/

