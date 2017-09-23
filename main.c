/** @file main.c 
Definição das funções que permitem que imprimem o jogo numa página em html do browser.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

/**
\brief Função que imprime o header da página do jogo.
*/
void print_header () {
	printf("Content-Type: text/html; charset=utf-8\n\n");
	printf("<header><title> Rogue </title></header> \n");
	printf("<style> body {cursor:url(%s), default;} a {cursor:url(%s), auto;} </style> \n",CURSOR1,CURSOR2);
	printf("<body> \n");
	printf("<svg width=1336 height=600> \n");
}

/**
\brief Função que imprime o corpo da página do jogo
*/
void print_footer () {
	printf("</svg> \n");
	printf("</body> \n");
}

/**
\brief Função que imprime o jogo na sua totalidade.
@returns Retorna 0 se funcionar.
*/
int main () {
	print_header ();
	parser ();
	print_footer ();
	return 0;
}