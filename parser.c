/** @file parser.c 
Definição das funções que permitem que a parte gráfica do jogo seja visualizada e, portanto, que permitem a execução do jogo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parser.h"
#include "estado.h"

/**
\brief Função que verifica se existe um certo inimigo numa certa casa
@param e Estado atual do jogo.
@param x Coordenada do eixo dos x.
@param y Coordenada do eixo dos y.
@param i Nº do inimigo.
@returns Retorna 1 se existir e 0 se não existir.
*/

int elem_enemy (ESTADO e, int x, int y,int i) { 			
	int r=0;
	if (e.inimigo[i].vida==1 && x==e.inimigo[i].x && y==e.inimigo[i].y) r=1;
	return r;
}

/**
\brief Função que encontra o índice de um inimigo.
@param e Estado atual do jogo.
@param x Coordenada do eixo dos x.
@param y Coordenada do eixo dos y.
@returns Retorna o índice do inimigo.
*/

int enemy_index (ESTADO e, int x, int y) {
	int i,r=-1;
	for (i=0;i<NUM_INIMIGOS+e.nivel;i++) {
		if (elem_enemy(e,x,y,i)) r=i; 
	}
	return r;
}

/**
\brief Função que verifica se existe um inimigo qualquer nas coordenadas dadas.
@param e Estado atual do jogo.
@param x Coordenada do eixo dos x.
@param y Coordenada do eixo dos y.
@returns Retorna 1 se existir e 0 se não existir.
*/

int elem_enemy_geral (ESTADO e, int x, int y) { 			
	int i, r=0;
	for (i=0;i<NUM_INIMIGOS+e.nivel;i++)
		if (elem_enemy(e,x,y,i)) r=1;
	return r;
}

/**
\brief Função que verifica se existe um certo obstáculo numa certa casa
@param e Estado atual do jogo.
@param x Coordenada do eixo dos x.
@param y Coordenada do eixo dos y.
@param i Nº do inimigo.
@returns Retorna 1 se existir e 0 se não existir.
*/


int elem_wall (ESTADO e, int x, int y,int i) { 			
	int r=0;
	if (e.obstaculo[i].destruido==0 && x==e.obstaculo[i].x && y==e.obstaculo[i].y) r=1;
	return r;
}


/**
\brief Função que encontra o índice de um obstáculo.
@param e Estado atual do jogo.
@param x Coordenada do eixo dos x.
@param y Coordenada do eixo dos y.
@returns Retorna o índice do obstáculo em (x,y)

*/

int wall_index (ESTADO e, int x, int y) {
	int i,r=-1;
	for (i=0;i<NUM_WALLS+2*e.nivel;i++) {
		if (elem_wall(e,x,y,i)) r=i; 
	}
	return r;
}

/** 
\brief Função que verifica se existe um qualquer obstáculo num par (x,y).
@param e Estado atual do jogo.
@param x Coordenada do eixo dos x.
@param y Coordenada do eixo dos y.
@returns Retorna 1 se existir e 0 se não existir.
*/

int elem_walls (ESTADO e, int x, int y) { 			
	int i, r=0;
	for (i=0;i<NUM_WALLS+2*e.nivel;i++)
		if (elem_wall(e,x,y,i)) r=1;
	return r;
}

/** 
\brief Função que verifica se existe um powerup no par (x,y).
@param e Estado atual do jogo.
@param x Coordenada do eixo dos x.
@param y Coordenada do eixo dos y.
@returns Retorna 1 se existir e 0 se não existir.
*/

int elem_powerup (ESTADO e, int x, int y) {
	int r=0;
	if (x==e.powerup.x && y ==e.powerup.y && e.powerup.usado==0) r=1;
	return r;
}

/**
\brief Função que verifica se o par (x,y) é uma posição vazia.
@param e Estado atual do jogo.
@param x Coordenada do eixo dos x.
@param y Coordenada do eixo dos y.
@returns Returna 1 se for vazia e 0 se não for.
*/

int pos_vazia (ESTADO e,int x,int y) {				
	if (x >= 0 && y >= 0 && x < SIZE && y < SIZE);
	else return 0;
	if (elem_enemy_geral(e,x,y)) return 0;
	if (elem_walls (e,x,y)) return 0;
	if (x==e.door.x && y ==e.door.y) return 0;
	if (x==e.jog.x && y ==e.jog.y) return 0;
	if (elem_powerup(e,x,y)) return 0;
	if (e.swap.plus==1 && x==e.plus.x && y==e.plus.y && e.plus.usado==0) return 0;
	return 1;
}

/**
\brief Função que lê os scores de um ficheiro e preenche um array ordenado de highscores.
@param e Estado atual do jogo.
@returns Novo estado do jogo com o array preenchido.
*/

ESTADO array_highscores(ESTADO e) {
	char path[] = "/var/www/html/estado/highscores";
	FILE *f;
	f = fopen( path, "r" );
	int i,N,j;
	for (i=0; fscanf(f,"%[^,],%d\n", e.campeao[i].nomecampeao, &e.campeao[i].scorecampeao)==2;i++); 
	N=i;
	e.numero=N;
	CAMPEAO a;
	for (i = 0; i < N; ++i) {  
        for (j = i + 1; j < N; ++j)
        {
            if (e.campeao[i].scorecampeao < e.campeao[j].scorecampeao)
            {
                a = e.campeao[i];
                e.campeao[i] = e.campeao[j];
                e.campeao[j] = a;
            }
        }
    }
	return e;
}

/**
\brief Função que, recebendo os dados do jogo, inicializa o jogo.
@param nivel Nível do jogo.
@param xporta Coordenada x da porta.
@param yporta Coordenada y da porta.
@param score Score do jogador atual.
@param state Estado do jogador, i.e, se tem power-ups.
@param powerup Power-up que o jogador tem atualmente.
@param vida Vida do jogador.
*/

ESTADO inicializar (int nivel,int xporta,int yporta,int score,int state, int powerup,int vida) { 
	int x,y,t;
	srand(time(NULL)); 
	ESTADO e;
	memset(&e, 0, sizeof(ESTADO));
	e.nivel = nivel;
	e.swap.menu=0;
	e.swap.howto=0;
	e.swap.help=0;
	e.swap.chart=0;
	e.swap.acao = 0;
	e.swap.range=0;
	e.swap.range1=0;
	e.swap.life=0;
	e.swap.monsterinfo=0;
	e.swap.monsterinfo1=0;
	e.swap.plus=rand() % 2;
	e.score = score;
	e.jog.vida=vida;
	e.jog.state=state;
	e.jog.powerup=powerup;
	e.powerup.usado=0;
	if (e.nivel==0) {
		e.jog.x = 0;
		e.jog.y = 0;
	}
	else {
		e.jog.x = xporta;
		e.jog.y = yporta; }
	int i=0;
	while (i < 1) {   		
    	x = rand() % SIZE;
		y = rand() % SIZE;
		if (pos_vazia (e,x,y)) {
			e.door.x = x;
			e.door.y = y;
			e.num_inimigos++;
			i++;
		}
	}
	i=0;
	while (i < 1) { 
    	x = rand() % SIZE;
		y = rand() % SIZE;
		t = rand() % 4 +1;
		if (pos_vazia (e,x,y)) {
			e.powerup.x = x;
			e.powerup.y = y;
			e.powerup.tipo = t;
			i++;
		}
	}
	e.num_inimigos = 0;
	i=0;
    while (i< NUM_INIMIGOS + (e.nivel)) { 			
    	x = rand() % SIZE;
		y = rand() % SIZE;
		if (pos_vazia (e,x,y)) {
			e.inimigo[i].x = x;
			e.inimigo[i].y = y;
			e.inimigo[i].vida = 1;
			e.inimigo[i].range = 0;
			if (e.nivel>=0 && e.nivel<2) e.inimigo[i].tipo=1;
			else if (e.nivel>=2 && e.nivel<5) e.inimigo[i].tipo=rand () % 2 +1;
			else e.inimigo[i].tipo=rand () % 4 +1;
			e.num_inimigos++;
			i++;
		}
	}
	e.num_obstaculos = 0;
	i=0;
    while ( i< NUM_WALLS + 2 * (e.nivel)) 			
    {
    	x = rand() % SIZE;
		y = rand() % SIZE;
		if (pos_vazia (e,x,y)) {
			e.obstaculo[i].x = x;
			e.obstaculo[i].y = y;
			e.obstaculo[i].destruido = 0;
			e.num_obstaculos++;
			i++;
		}
    }
    i=0;
    if (e.swap.plus==1) {
    	while (i < 1) {   						
    		x = rand() % SIZE;
			y = rand() % SIZE;
			if (pos_vazia (e,x,y)) {
			e.plus.x = x;
			e.plus.y = y;
			e.plus.usado = 0;
			i++;
			}
		}
	}
	return e;
}

/**
\brief Função que imprime uma imagem.
@param px Posição da imagem em x.
@param py Posição da imagem em y.
@param tam Tamanho da imagem.
@param imagem Imagem a ser imprimida.
*/

void print_image (int px, int py, int tam, char *imagem) {  
	printf ("<image x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", px*tam,py*tam,tam,tam,imagem);
}


/**
\brief Função que imprime texto.
@param x Posição do texto em x.
@param y Posição do texto em y.
@param f Tamanho da fonte.
@param cor Cor da fonte.
@param texto Texto a ser imprimido.
*/


void print_text (int x, int y, int f, char *cor, char *texto) {
	printf("<text x=%d y=%d font-family=Mario font-size=%d fill=%s> %s </text> \n",x,y,f,cor,texto);
}

/**
\brief Função que imprime um retângulo.
@param x Posição do retângulo em x.
@param y Posição do retângulo em y.
@param w Largura do retângulo.
@param h Comprimento do retângulo.
@param cor Cor do retângulo.
@param o Opacidade do retângulo.
*/

void print_rect (int x, int y, int w, int h, char *cor, float o) {
	printf("<rect x=%d y=%d width=%d height=%d style=\"fill:%s;fill-opacity:%f\"/> \n",x,y,w,h,cor,o);
}

/**
\brief Função que imprime uma imagem.
@param x Posição da imagem em x.
@param y Posição da imagem em y.
@param w Largura da imagem.
@param h Comprimento do retângulo.
@param imagem Imagem a ser impressa.
*/

void print_imagem (int x, int y, int w, int h, char *imagem) {
	printf ("<image x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",x,y,w,h,imagem);
}

/**
\brief Função que imprime a porta.
@param e Estado atual do jogo.
*/

void print_door (ESTADO e) {  						
	print_image(e.door.x, e.door.y, TAM, DOOR);
}

/**
\brief Função que imprime um powerup.
@param e Estado atual do jogo.
*/


void print_powerup (ESTADO e) {  						
	if (e.powerup.usado==0) print_image(e.powerup.x, e.powerup.y, TAM, POWER);
}

/**
\brief Função que o bónus de apanhar uma vida.
@param e Estado atual do jogo.
*/


void print_plus (ESTADO e) {
	if (e.swap.plus==1 && e.plus.usado==0) print_image(e.plus.x, e.plus.y, TAM, PLUS);
}

/**
\brief Função que imprime os inimigos.
@param e Estado atual do jogo.
*/

void print_enemies(ESTADO e) {					
	int i;
	for(i = 0; i<NUM_INIMIGOS + e.nivel; i++) {
		if (e.inimigo[i].vida==1) {
			if (e.inimigo[i].tipo==1) printf ("<image id=inimigo%d x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",i, e.inimigo[i].x*TAM,e.inimigo[i].y*TAM,TAM,TAM,ENEMY1);
			else if (e.inimigo[i].tipo==2) printf ("<image id=inimigo%d x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",i, e.inimigo[i].x*TAM,e.inimigo[i].y*TAM,TAM,TAM,ENEMY2);
			else if (e.inimigo[i].tipo==3) printf ("<image id=inimigo%d x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",i, e.inimigo[i].x*TAM,e.inimigo[i].y*TAM,TAM,TAM,ENEMY3);
			else printf ("<image id=inimigo%d x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",i, e.inimigo[i].x*TAM,e.inimigo[i].y*TAM,TAM,TAM,ENEMY4);
		}
	}
}


/**
\brief Função que imprime um certo inimigo.
@param e Estado atual do jogo.
@param i Índice do inimigo.
*/


void print_enemy(ESTADO e,int i) {					
	if (e.inimigo[i].vida==1) {
		if (e.inimigo[i].tipo==1) printf ("<image id=1inimigo%d x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",i, e.inimigo[i].x*TAM,e.inimigo[i].y*TAM,TAM,TAM,ENEMY1);
		else if (e.inimigo[i].tipo==2) printf ("<image id=1inimigo%d x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",i, e.inimigo[i].x*TAM,e.inimigo[i].y*TAM,TAM,TAM,ENEMY2);
		else if (e.inimigo[i].tipo==3) printf ("<image id=1inimigo%d x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",i, e.inimigo[i].x*TAM,e.inimigo[i].y*TAM,TAM,TAM,ENEMY3);
		else printf ("<image id=1inimigo%d x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",i, e.inimigo[i].x*TAM,e.inimigo[i].y*TAM,TAM,TAM,ENEMY4);
	}
}

/**
\brief Função que imprime um certo obstáculo.
@param e Estado atual do jogo.
@param i Índice da porta.
*/


void print_wall (ESTADO e,int i) {					
	if (e.obstaculo[i].destruido==0) {
		if (i<12) print_image (e.obstaculo[i].x, e.obstaculo[i].y, TAM, WALL1);
		if (i>=12 && i<24) print_image (e.obstaculo[i].x, e.obstaculo[i].y, TAM, WALL2);
		if (i>=24) print_image (e.obstaculo[i].x, e.obstaculo[i].y, TAM, WALL3);
	}
}


/**
\brief Função que verifica se uma casa é vazia ou tem lá o jogador, ou seja, esta função é usada para os inimigos matarem o jogador.
@param e Estado atual do jogo.
@param x Coordenada no eixo dos x.
@param y Coordenada no eixo dos y.
@returns Retorna 1 se for vazia ou estiver lá um jogador e 0 caso contrário.
*/


int mataplayer (ESTADO e,int x,int y) {				
	if (x >= 0 && y >= 0 && x < SIZE && y < SIZE);
	else return 0;
	if (elem_enemy_geral(e,x,y)) return 0;
	if (elem_walls (e,x,y)) return 0;
	if (x==e.door.x && y ==e.door.y) return 0;
	if (x==e.jog.x && y ==e.jog.y) return 1;
	if (elem_powerup(e,x,y)) return 0;
	if (x==e.plus.x && y==e.plus.y && e.plus.usado==0) return 0;
	return 1;
}

/**
\brief Função que verifica se um inimigo está em range para matar um jogador.
@param e Estado atual do jogo.
@param x Coordenada no eixo dos x do jogador.
@param y Coordenada no eixo dos y do jogador.
@param i Índice do inimigo em questão.
@returns Retorna 1, verdadeiro, ou 0,falso.
*/

int rangejogador (ESTADO e, int x, int y, int i) {
	if ( (e.inimigo[i].x == x && e.inimigo[i].y == (y+1)) ||
		 (e.inimigo[i].x == (x-1) && e.inimigo[i].y == (y+1) ) ||
		(e.inimigo[i].x == (x-1) && e.inimigo[i].y == y) ||
		(e.inimigo[i].x == (x-1) && e.inimigo[i].y == (y-1)) ||
		(e.inimigo[i].x == x && e.inimigo[i].y == (y-1)) ||
		(e.inimigo[i].x == (x+1) && e.inimigo[i].y == (y-1))||
		(e.inimigo[i].x == (x+1) && e.inimigo[i].y == y)) return 1;
	else return 0;
}

/**
\brief Função que verifica se um inimigo do tipo feiticeiro está em range para matar um jogador.
@param e Estado atual do jogo.
@param x Coordenada no eixo dos x do jogador.
@param y Coordenada no eixo dos y do jogador.
@param i Índice do inimigo em questão.
@returns Retorna 1, verdadeiro, ou 0,falso.
*/

int rangefeiticeiro (ESTADO e, int x, int y, int i) {
	if ( (e.inimigo[i].x == x && e.inimigo[i].y == y+2)   ||
		(e.inimigo[i].x == x-2 && e.inimigo[i].y == y+2)  ||
		(e.inimigo[i].x == x-2 && e.inimigo[i].y == y)    ||
		(e.inimigo[i].x == x-2 && e.inimigo[i].y == y-2)  ||
		(e.inimigo[i].x == x && e.inimigo[i].y == y-2) 	  ||
		(e.inimigo[i].x == x+2 && e.inimigo[i].y == y-2)  ||
		(e.inimigo[i].x == x+2 && e.inimigo[i].y == y)    ||
		(e.inimigo[i].x == x+2 && e.inimigo[i].y == y+2)) return 1;
	else return 0;
}

/**
\brief Função que gera as coordenadas para onde os inimigos se devem mover.
@param e Estado atual do jogo.
@param x Coordenada do jogador no eixo dos x.
@param y Coordenada do jogador no eixo dos y.
@returns Novo estado de jogo com as novas coordenadas dos inimigos.
*/

ESTADO mexemonstros (ESTADO e, int x, int y) {
	int a,b;
	int i =0;
	int k = e.jog.vida;
	int xdoor,ydoor;
	xdoor= e.door.x;
	ydoor= e.door.y;
	while (i<NUM_INIMIGOS + e.nivel) {
		if (e.inimigo[i].tipo==1) {
				a=e.inimigo[i].x;
				b=e.inimigo[i].y;   
				if 		(a > x  && b > y && mataplayer(e,a-1,b-1)  && elem_enemy_geral(e,a-1,b-1)==0)  { e.inimigo[i].x = a - 1; e.inimigo[i].y = b - 1; }
				else if (a > x  && b == y && mataplayer(e,a-1,b)   && elem_enemy_geral(e,a-1,b)==0)    { e.inimigo[i].x = a - 1; e.inimigo[i].y = b;     }
				else if (a > x  && b < y && mataplayer(e,a-1,b+1)  && elem_enemy_geral(e,a-1,b+1)==0)  { e.inimigo[i].x = a - 1; e.inimigo[i].y = b + 1; }
				else if (a < x  && b > y && mataplayer(e,a+1,b-1)   && elem_enemy_geral(e,a+1,b-1)==0)  { e.inimigo[i].x = a + 1; e.inimigo[i].y = b - 1; }
				else if (a < x  && b == y && mataplayer(e,a+1,b)   && elem_enemy_geral(e,a+1,b)==0)    { e.inimigo[i].x = a + 1; e.inimigo[i].y = b;     }
				else if (a < x  && b < y  && mataplayer(e,a+1,b+1) && elem_enemy_geral(e,a+1,b+1)==0)  { e.inimigo[i].x = a + 1; e.inimigo[i].y = b + 1; }
				else if (a == x && b > y && mataplayer(e,a,b-1)   && elem_enemy_geral(e,a,b-1)==0)    { e.inimigo[i].x = a; 	   e.inimigo[i].y = b - 1; }
				else if (a == x && b < y && mataplayer(e,a,b+1)   && elem_enemy_geral(e,a,b+1)==0)    { e.inimigo[i].x = a;	   e.inimigo[i].y = b + 1; }
				

				if (e.inimigo[i].x == x && e.inimigo[i].y == y && e.inimigo[i].vida==1) {
					e.jog.vida=k-1;
					e.swap.life=1;
					e.inimigo[i].x=a;
					e.inimigo[i].y=b;
				} 	

				printf ("<animateMotion xlink:href=#inimigo%d dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",i,(a-e.inimigo[i].x)*TAM,(b-e.inimigo[i].y)*TAM,0,0);
				printf ("<animateMotion xlink:href=#1inimigo%d dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",i,(a-e.inimigo[i].x)*TAM,(b-e.inimigo[i].y)*TAM,0,0);
		}
		else if (e.inimigo[i].tipo == 2) {
				a=e.inimigo[i].x;
				b=e.inimigo[i].y;   
				if (!rangefeiticeiro(e,x,y,i)) { 
					if 		(a > x  && b > y  && pos_vazia(e,a-1,b-1) && elem_enemy_geral(e,a-1,b-1)==0)  { e.inimigo[i].x = a - 1; e.inimigo[i].y = b - 1; }
					else if (a > x && b == y  && pos_vazia(e,a-1,b)   && elem_enemy_geral(e,a-1,b)==0)    { e.inimigo[i].x = a - 1; e.inimigo[i].y = b;     }
					else if (a > x  && b < y  && pos_vazia(e,a-1,b+1) && elem_enemy_geral(e,a-1,b+1)==0)  { e.inimigo[i].x = a - 1; e.inimigo[i].y = b + 1; }
					else if (a < x  && b > y  && pos_vazia(e,a+1,b-1) && elem_enemy_geral(e,a+1,b-1)==0)  { e.inimigo[i].x = a + 1; e.inimigo[i].y = b - 1; }
					else if (a < x  && b == y && pos_vazia(e,a+1,b)   && elem_enemy_geral(e,a+1,b)==0)    { e.inimigo[i].x = a + 1; e.inimigo[i].y = b;     }
					else if (a < x  && b < y  && pos_vazia(e,a+1,b+1) && elem_enemy_geral(e,a+1,b+1)==0)  { e.inimigo[i].x = a + 1; e.inimigo[i].y = b + 1; }
					else if (a == x && b > y  && pos_vazia(e,a,b-1)   && elem_enemy_geral(e,a,b-1)==0)    { e.inimigo[i].x = a; 	e.inimigo[i].y = b - 1; }
					else if (a == x && b < y  && pos_vazia(e,a,b+1)   && elem_enemy_geral(e,a,b+1)==0)    { e.inimigo[i].x = a;	    e.inimigo[i].y = b + 1; }
				}
				else { 
				
					if 		(a > x  && b > y  && mataplayer(e,a-2,b-2)  && elem_enemy_geral(e,a-2,b-2)==0)  { e.inimigo[i].x = a - 2; e.inimigo[i].y = b - 2; }					
					else if (a > x  && b == y && mataplayer(e,a-2,b)    && elem_enemy_geral(e,a-2,b)==0)    { e.inimigo[i].x = a - 2; e.inimigo[i].y = b;     }
					
					else if (a > x  && b < y  && mataplayer(e,a-2,b+2)  && elem_enemy_geral(e,a-2,b+2)==0)  { e.inimigo[i].x = a - 2; e.inimigo[i].y = b + 2; }
					else if (a < x  && b > y  && mataplayer(e,a+2,b-2)  && elem_enemy_geral(e,a+2,b-2)==0)  { e.inimigo[i].x = a + 2; e.inimigo[i].y = b - 2; }
					
					else if (a < x  && b == y && mataplayer(e,a+2,b)    && elem_enemy_geral(e,a+2,b)==0)    { e.inimigo[i].x = a + 2; e.inimigo[i].y = b;     }
					
					else if (a < x  && b < y  && mataplayer(e,a+2,b+2)  && elem_enemy_geral(e,a+2,b+2)==0)  { e.inimigo[i].x = a + 2; e.inimigo[i].y = b + 2; }
					
					else if (a == x && b > y  && mataplayer(e,a,b-2)    && elem_enemy_geral(e,a,b-2)==0)    { e.inimigo[i].x = a; 	e.inimigo[i].y = b - 2; }
					else if (a == x && b < y  && mataplayer(e,a,b+2)    && elem_enemy_geral(e,a,b+2)==0)    { e.inimigo[i].x = a;	e.inimigo[i].y = b + 2; }
					
				

					if (e.inimigo[i].x == x && e.inimigo[i].y == y && e.inimigo[i].vida==1) {
						e.jog.vida=k-1;
						e.swap.life=1;
						e.inimigo[i].x=a;
						e.inimigo[i].y=b;
					} 	
				}
				
				printf ("<animateMotion xlink:href=#inimigo%d dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",i,(a-e.inimigo[i].x)*TAM,(b-e.inimigo[i].y)*TAM,0,0);
				printf ("<animateMotion xlink:href=#1inimigo%d dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",i,(a-e.inimigo[i].x)*TAM,(b-e.inimigo[i].y)*TAM,0,0);
		}

		else if (e.inimigo[i].tipo == 3 || e.inimigo[i].tipo == 4) {
				a=e.inimigo[i].x;
				b=e.inimigo[i].y;   
				if (!rangejogador(e,x,y,i)) {
					if 		(a > xdoor  && b > ydoor  && mataplayer(e,a-1,b-1) && elem_enemy_geral(e,a-1,b-1)==0)  { e.inimigo[i].x = a - 1; e.inimigo[i].y = b - 1; }
					else if (a > xdoor && b == ydoor  && mataplayer(e,a-1,b)   && elem_enemy_geral(e,a-1,b)==0)    { e.inimigo[i].x = a - 1; e.inimigo[i].y = b;     }
					else if (a > xdoor  && b < ydoor  && mataplayer(e,a-1,b+1) && elem_enemy_geral(e,a-1,b+1)==0)  { e.inimigo[i].x = a - 1; e.inimigo[i].y = b + 1; }
					else if (a < xdoor  && b > ydoor  && mataplayer(e,a+1,b-1) && elem_enemy_geral(e,a+1,b-1)==0)  { e.inimigo[i].x = a + 1; e.inimigo[i].y = b - 1; }
					else if (a < xdoor  && b == ydoor && mataplayer(e,a+1,b)   && elem_enemy_geral(e,a+1,b)==0)    { e.inimigo[i].x = a + 1; e.inimigo[i].y = b;     }
					else if (a < xdoor  && b < ydoor  && mataplayer(e,a+1,b+1) && elem_enemy_geral(e,a+1,b+1)==0)  { e.inimigo[i].x = a + 1; e.inimigo[i].y = b + 1; }
					else if (a == xdoor && b > ydoor  && mataplayer(e,a,b-1)   && elem_enemy_geral(e,a,b-1)==0)    { e.inimigo[i].x = a; 	   e.inimigo[i].y = b - 1; }
					else if (a == xdoor && b < ydoor  && mataplayer(e,a,b+1)   && elem_enemy_geral(e,a,b+1)==0)    { e.inimigo[i].x = a;	   e.inimigo[i].y = b + 1; }
				}
				else { 
					if 		(a > x  && b > y  && mataplayer(e,a-1,b-1) && elem_enemy_geral(e,a-1,b-1)==0)  { e.inimigo[i].x = a - 1; e.inimigo[i].y = b - 1; }
					else if (a > x  && b == y && mataplayer(e,a-1,b)   && elem_enemy_geral(e,a-1,b)==0)    { e.inimigo[i].x = a - 1; e.inimigo[i].y = b;     }
					else if (a > x  && b < y  && mataplayer(e,a-1,b+1) && elem_enemy_geral(e,a-1,b+1)==0)  { e.inimigo[i].x = a - 1; e.inimigo[i].y = b + 1; }
					else if (a < x  && b > y  && mataplayer(e,a+1,b-1) && elem_enemy_geral(e,a+1,b-1)==0)  { e.inimigo[i].x = a + 1; e.inimigo[i].y = b - 1; }
					else if (a < x  && b == y && mataplayer(e,a+1,b)   && elem_enemy_geral(e,a+1,b)==0)    { e.inimigo[i].x = a + 1; e.inimigo[i].y = b;     }
					else if (a < x  && b < y  && mataplayer(e,a+1,b+1) && elem_enemy_geral(e,a+1,b+1)==0)  { e.inimigo[i].x = a + 1; e.inimigo[i].y = b + 1; }
					else if (a == x && b > y  && mataplayer(e,a,b-1)   && elem_enemy_geral(e,a,b-1)==0)    { e.inimigo[i].x = a; 	   e.inimigo[i].y = b - 1; }
					else if (a == x && b < y  && mataplayer(e,a,b+1)   && elem_enemy_geral(e,a,b+1)==0)    { e.inimigo[i].x = a;	   e.inimigo[i].y = b + 1; }
				}
				if (e.inimigo[i].x == x && e.inimigo[i].y == y && e.inimigo[i].vida==1) {
					e.jog.vida=k-1;
					e.swap.life=1;
					e.inimigo[i].x=a;
					e.inimigo[i].y=b;
				} 	

				printf ("<animateMotion xlink:href=#inimigo%d dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",i,(a-e.inimigo[i].x)*TAM,(b-e.inimigo[i].y)*TAM,0,0);
				printf ("<animateMotion xlink:href=#1inimigo%d dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",i,(a-e.inimigo[i].x)*TAM,(b-e.inimigo[i].y)*TAM,0,0);
		}
				i++;
	}
	return e;
}


/**
\brief Função que imprime os movimentos e ações.
@param e Estado atual do jogo.
@param dX Distância entre a posição do jogador e a nova posição no eixo dos x.
@param dY Distância entre a posição do jogador e a nova posição no eixo dos y.
@param nome_f Nome do ficheiro/jogador.
*/

void print_move (ESTADO e,int dX, int dY,char *nome_f) {        
	int x = e.jog.x + dX;									  
	int y = e.jog.y + dY;
	int x0,y0,x1,y1,x2,y2;
	x0=x1=x2=y0=y1=y2=-1;
	if (e.swap.life==0 && e.swap.range==0 && e.swap.range1==0 && e.swap.chart==0 && e.swap.help==0 && e.swap.monsterinfo1==0){

		
		if (x==e.door.x && y==e.door.y) {
			printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\">\n",nome_f,8,x,y);
			print_rect(x*TAM,y*TAM,TAM,TAM,"green",0.3);
			print_door (e);
			printf("</a>\n");
		}

		
		if (x==e.powerup.x && y==e.powerup.y && e.powerup.usado==0) {
			printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\">\n",nome_f,7,x,y);
			print_powerup (e);
			printf("</a>\n");
		}

		if (e.swap.plus==1 && x==e.plus.x && y==e.plus.y && e.plus.usado==0) {
			printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\">\n",nome_f,6,x,y);
			print_rect(x*TAM,y*TAM,TAM,TAM,"green",0.3);
			print_plus (e);
			printf("</a>\n");
		}

		if (elem_enemy_geral(e,x,y) && e.inimigo[enemy_index(e,x,y)].tipo!=3 && e.inimigo[enemy_index(e,x,y)].tipo!=4) {
				printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\"> \n",nome_f,5,x,y);
				print_rect(x*TAM,y*TAM,TAM,TAM,"red",0.3);
				print_enemy(e,(enemy_index(e,x,y)));
				printf ("</a>\n");
		}

		if (pos_vazia (e,x,y)) {
			printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\"> \n",nome_f,4,x,y);
			print_rect(x*TAM,y*TAM,TAM,TAM,"green",0.3);
			printf ("</a>\n"); 
		}

		if (e.jog.state==1 && e.jog.powerup>0) {
			x0 = e.jog.x + 2*dX;					  
			y0 = e.jog.y + 2*dY;
			if (2*dX-dY>-3 && 2*dY-dX>-3) {
			x1 = e.jog.x + 2*dX-dY;
			y1 = e.jog.y + 2*dY-dX; }
			if (2*dX+dY>-3 && 2*dY+dX<3) {
			x2 = e.jog.x + 2*dX+dY;
			y2 = e.jog.y + 2*dY+dX; }
			if (elem_enemy_geral(e,x0,y0) && e.inimigo[enemy_index(e,x0,y0)].tipo!=3) {
				printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\">\n",nome_f,11,x0,y0);
				print_rect(x0*TAM,y0*TAM,TAM,TAM,"red",0.3);
				print_enemy(e,(enemy_index(e,x0,y0)));
				printf ("</a>\n");
			}
			if (elem_enemy_geral(e,x1,y1) && dX!=dY && dX!=-dY && e.inimigo[enemy_index(e,x1,y1)].tipo!=3) {
				printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\">\n",nome_f,11,x1,y1);
				print_rect(x1*TAM,y1*TAM,TAM,TAM,"red",0.3);
				print_enemy(e,(enemy_index(e,x1,y1)));
				printf ("</a>\n");
			}
			if (elem_enemy_geral(e,x2,y2) && dX!=dY && dX!=-dY && e.inimigo[enemy_index(e,x2,y2)].tipo!=3) {
				printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\">\n",nome_f,11,x2,y2);
				print_rect(x2*TAM,y2*TAM,TAM,TAM,"red",0.3);
				print_enemy(e,(enemy_index(e,x2,y2)));
				printf ("</a>\n");
			}
		}

		if (e.jog.state==2 && e.jog.powerup>0) {
			if (elem_walls(e,x,y)) {
					printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\">\n",nome_f,10,x,y);
					print_image(x,y,TAM,BOMBA);
					printf ("</a>\n");
			}
		}

		if (e.jog.state==3 && e.jog.powerup>0) {
			x0 = e.jog.x + 2*dX;					  
			y0 = e.jog.y + 2*dY;
			if (2*dX-dY>-3 && 2*dY-dX>-3) {
			x1 = e.jog.x + 2*dX-dY;
			y1 = e.jog.y + 2*dY-dX; 
			}
			if (2*dX+dY>-3 && 2*dY+dX<3) {
			x2 = e.jog.x + 2*dX+dY;
			y2 = e.jog.y + 2*dY+dX; 
			}
			if (pos_vazia(e,x0,y0)) {
				printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\">\n",nome_f,9,x0,y0);
				print_rect(x0*TAM,y0*TAM,TAM,TAM,"green",0.3);
				printf ("</a>\n");
			}
			if (pos_vazia(e,x1,y1) && dX!=dY && dX!=-dY) {
				printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\">\n",nome_f,9,x1,y1);
				print_rect(x1*TAM,y1*TAM,TAM,TAM,"green",0.3);
				printf ("</a>\n");
			}
			if (pos_vazia(e,x2,y2) && dX!=dY && dX!=-dY) {
				printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\">\n",nome_f,9,x2,y2);
				print_rect(x2*TAM,y2*TAM,TAM,TAM,"green",0.3);
				printf ("</a>\n");
			}
		}

		if (e.jog.state==4 && e.jog.powerup>0) {
			if (elem_enemy_geral(e,x,y) && e.inimigo[enemy_index(e,x,y)].tipo!=4) {
				print_text (615,210,18,"black","click on the player");
				print_text (615,230,18,"black","to use the bomb!");
				printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\"> \n",nome_f,17,e.jog.x,e.jog.y);
				print_rect(e.jog.x*TAM,e.jog.y*TAM,TAM,TAM,"orange",0.3);
				printf ("<image id=1jog%d%d x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",dX,dY,e.jog.x*TAM,e.jog.y*TAM,TAM,TAM,PLAYER4);
				printf ("</a>\n"); 
			}
		}

		if (e.jog.state==1 && e.jog.powerup>0 && elem_enemy_geral(e,x,y) && e.inimigo[enemy_index(e,x,y)].tipo==4){
			printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d,%d\"> \n",nome_f,21,x,y);
			print_rect(x*TAM,y*TAM,TAM,TAM,"red",0.3);
			print_enemy(e,(enemy_index(e,x,y)));
			printf ("</a>\n");
		}

		if (pos_vazia(e,x,y) == 0) return;
	}
}


/**
\brief Função que imprime o jogador.
@param e Estado atual do jogo.
@param nome_f Nome do ficheiro/jogador.
*/


void print_player (ESTADO e,char *nome_f) {					
	if (e.jog.state==1 && e.jog.powerup>0) printf ("<image id=jog x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", e.jog.x*TAM,e.jog.y*TAM,TAM,TAM,PLAYER1);
		 else if (e.jog.state==2 && e.jog.powerup>0) printf ("<image id=jog x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", e.jog.x*TAM,e.jog.y*TAM,TAM,TAM,PLAYER2);
		 	  else if (e.jog.state==3 && e.jog.powerup>0) printf ("<image id=jog x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", e.jog.x*TAM,e.jog.y*TAM,TAM,TAM,PLAYER3);
		 	  	   else if (e.jog.state==4 && e.jog.powerup>0) printf ("<image id=jog x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", e.jog.x*TAM,e.jog.y*TAM,TAM,TAM,PLAYER4);
		 	  	   		else printf ("<image id=jog x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", e.jog.x*TAM,e.jog.y*TAM,TAM,TAM,PLAYER);
	print_move (e,+1,+0,nome_f);
	print_move (e,-1,+0,nome_f);
	print_move (e,+0,+1,nome_f);
	print_move (e,+0,-1,nome_f);
	print_move (e,+1,+1,nome_f); 
	print_move (e,-1,-1,nome_f); 
	print_move (e,+1,-1,nome_f);
	print_move (e,-1,+1,nome_f);
}

/** 
\brief Funnção que imprime os obstáculos.
@param e Estado atual do jogo.
*/

void print_walls (ESTADO e) {				
	int i;
	for(i=0; i < e.num_obstaculos; i++) {
		if (e.obstaculo[i].destruido==0) {
			if (i<12) print_image (e.obstaculo[i].x, e.obstaculo[i].y, TAM, WALL1);
			if (i>=12 && i<24) print_image (e.obstaculo[i].x, e.obstaculo[i].y, TAM, WALL2);
			if (i>=24) print_image (e.obstaculo[i].x, e.obstaculo[i].y, TAM, WALL3);
		}
	}
}

/**
\brief Função que imprime os quadrados do tabuleiro.
@param px Posição no eixo dos x.
@param py Posição no eixo dos y.
@param tam Tamanho da imagem
*/

void print_square(int px, int py, int tam) {   
	print_image (px,py,tam,FLOOR);
}


/** 
\brief Função que imprime o tabuleiro.
*/

void print_board () {						  
	int x,y;
	for (y=0;y<SIZE;y++)
		for (x=0;x<SIZE;x++)
			print_square(x,y,TAM);
}

/** 
\brief Função que imprime o nível do jogo atual.
@param e Estado atual do jogo.
*/


void print_level (ESTADO e) {
	printf("<svg> <g> \n");
	print_rect(615,0,125,75,"green",1);
    print_text (618,25,28,"white","LEVEL:");
    printf("<text x=618 y=60 font-family=Mario font-size=28 fill=white> %d </text> \n",e.nivel+1);
	printf("</g> </svg> \n");
}


/** 
\brief Função que imprime o score atual.
@param e Estado atual do jogo.
*/

void print_score (ESTADO e) {
	printf("<svg> <g> \n");
	print_rect(615,75,125,75,"red",1);
    print_text (618,100,28,"white","SCORE:");
    printf("<text x=618 y=135 font-family=Mario font-size=28 fill=white> %d </text> \n",e.score);
	printf("</g> </svg> \n");
}

/** 
\brief Função que imprime o background do jogo.
@param url Url da imagem.
*/

void print_background (char *url) {
	printf("<style> body {background-image:url(%s); background-position:center top; background-repeat:no-repeat; background-size:cover} </style> \n",url);
}

/**
\brief Função que passa a fonte de texto para "Mario".
*/

void print_letra () {
	printf ("<style> @font-face {font-family: Mario; src: url(\"%s\")} </style> \n",LETRA);
}

/**
\brief Função que imprime as vidas do jogador.
@param e Estado atual do jogo.
*/

void print_vida (ESTADO e) {
	print_text (850,150,40,"black","Lives:");
	if (e.jog.vida==3) {
		print_imagem (1000,110,50,50,VIDA1);
		print_imagem (1050,110,50,50,VIDA1);
		print_imagem (1100,110,50,50,VIDA1);
	}
	else if (e.jog.vida==2) {
		print_imagem (1000,110,50,50,VIDA1);
		print_imagem (1050,110,50,50,VIDA1);
		print_imagem (1100,110,50,50,VIDA0);
	}
	else {
		print_imagem (1000,110,50,50,VIDA1);
		print_imagem (1050,110,50,50,VIDA0);
		print_imagem (1100,110,50,50,VIDA0);
	}
}

/**
\brief Função que imprime o estado do jogador, i.e, se estiver com powerups.
@param e Estado atual do jogo.
*/


void print_state (ESTADO e) {
	print_text (850,225,40,"black","State:");
	if (e.jog.state==0 || e.jog.powerup==0) print_imagem (1025,185,50,50,NORMAL);
	else if (e.jog.state==1) {
		print_imagem (1025,185,50,50,RANGED);
		printf("<text x=1080 y=230 font-family=Mario font-size=25 fill=black> %dx </text> \n",e.jog.powerup);
		}
		else if (e.jog.state==2) {
				print_imagem (1025,185,50,50,DESTRUIR);
				printf("<text x=1080 y=230 font-family=Mario font-size=25 fill=black> %dx </text> \n",e.jog.powerup);
			} 
			else if (e.jog.state==3) {
				print_imagem (1025,185,50,50,VOAR);
				printf("<text x=1080 y=230 font-family=Mario font-size=25 fill=black> %dx </text> \n",e.jog.powerup);
			} 
				else {
					print_imagem (1025,185,50,50,BOMB);
					printf("<text x=1080 y=230 font-family=Mario font-size=25 fill=black> %dx </text> \n",e.jog.powerup);
				}
}

/**
\brief Função que imprime as informações do jogador (estado e vida).
@param e Estado atual do jogo.
*/

void print_info (ESTADO e){
	print_text (825,75,65,"black","PLAYER INFO:");
	print_vida(e);
	print_state (e);
}

/**
\brief Função que imprime o butão "Give Up".
@param nome_f Nome do ficheiro/jogador.
*/


void print_giveup (char *nome_f) {
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,12);
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",1100,520,210,75);
	print_text(1112,570,40,"black","give up!");
	printf ("</a>\n");
}

/**
\brief Função que imprime o "How To Play".
@param e Estado atual do jogo.
@param nome_f Nome do ficheiro/jogador.
*/


void print_help (ESTADO e, char *nome_f) {
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,19);
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",1100,430,210,75);
	print_text(1112,480,32,"black","Need help?");
	printf ("</a>\n");
	if (e.swap.help==1) {		
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:black;stroke-width:6;stroke:white\" />\n",70,50,1000,450);
	print_text(80,75,15,"white","this is a simplified version of a Coffeebreak Roguelike game, in the world of Super Mario.");
	print_text(80,95,15,"white","So, the rules are pretty simple:");
	print_text(80,115,15,"white","1. The objective is to reach the princess, so that you can advance to the next level.");
	print_text(80,135,15,"white","2. You can move to the green areas and you can attack enemies with a red area.");
	print_text(80,155,15,"white","3. If you move an area inside an enemy's attack range, you will lose one life.");
	print_text(80,175,15,"white","4. But don't worry, you will get the oportunity to get extra lives! and you can pick up Special Powers!");
	print_text(80,195,15,"white","Well, these Special Powers are inside the yellow boxes. But what can you do with them?");
	print_imagem(80,205,40,40,NORMAL);
	print_text(125,230,15,"white",": Normal state");
	print_imagem(80,250,40,40,RANGED);
	print_text(125,275,15,"white",": This lets you attack enemies that are not in your normal attack range (+1)");
	print_imagem(80,295,40,40,DESTRUIR);
	print_text(125,320,15,"white",": This lets you destroy obstacles in your radius");
	print_imagem(80,340,40,40,VOAR);
	print_text(125,365,15,"white",": This lets you move to areas that are not in your normal radius (+1)");
	print_imagem(80,385,40,40,BOMB);
	print_text(125,410,15,"white",": This explodes a bomb around you, killing all the enemies in your radius");
	print_text(80,455,15,"white","And that's it. As you can see, it's pretty easy to play.");
	print_text(80,485,15,"white","Now the last, but not the least, rule:");
	print_text(450,485,15,"red","DO NOT REFRESH THE PAGE");
	print_text(680,485,15,"white",", OR THE GAME WILL CRASH!");
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:black;stroke-width:6;stroke:white\" />\n",1070,50,210,350);
	print_text(1090,75,20,"white","Score Points:");
	print_text(1080,110,15,"white","Saving the princess:");
	print_text(1080,130,20,"green","50 pts");
	print_text(1080,155,15,"white","Killing monsters:");
	print_text(1080,175,18,"green","See monster info");
	print_text(1080,210,15,"white","Showing range of");
	print_text(1080,225,15,"white","all enemies:");
	print_text(1080,245,20,"red","-20 pts");
	print_text(1080,270,15,"white","Showing range of");
	print_text(1080,290,15,"white","specific enemy:");
	print_text(1080,310,20,"red","-10 pts");
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,19);
	print_imagem(990,420,75,75,X);
	printf ("</a>\n");
	}
}

/**
\brief Função que imprime o botão "Monster Info" com as info dos monstros.
@param e Estado atual de jogo.
@param nome_f Nome do ficheiro/jogador.
*/

void print_monsterinfo (ESTADO e, char *nome_f) {
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,23);
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",1100,250,210,75);
	print_text(1120,280,30,"black","monster");
	print_text(1170,310,30,"black","info");
	printf ("</a>\n");
	if (e.swap.monsterinfo1==1) {
		printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:black;stroke-width:6;stroke:white\" />\n",70,50,1000,450);
		print_text(80,75,15,"white","In this game, we have 4 types of enemies, and each type gives you a different amount of points.");
		print_text(80,115,15,"white","1. The melee monsters follow you and try to attack you when you move to their attack range.");
		print_text(80,145,15,"white","They give you 5 points, since they are the easiest to deal with.");
		print_text(80,175,15,"white","2. The ranged monsters have double the range of the melee ones, only attacking interspersed positions.");
		print_text(80,195,15,"white","They also follow you, but they don't attack the positions directly near to them.");
		print_text(80,215,15,"white","They give you 10 points.");
		print_text(80,245,15,"white","Well, there are two types of boss-like monsters. Both of them go to the princess,");
		print_text(80,265,15,"white","but they attack you if you interfere.");
		print_text(80,295,15,"white","3. The first one can only die with the Bomb Power-up, because he has a hard shell.");
		print_text(80,325,15,"white","4. The second one can only die with the Ranged Power-up, since he is only afraid of fire.");
		print_text(80,355,15,"white","The both of them give you 20 points. But take care...");
		print_text(80,385,15,"white","1:");
		print_imagem(120,375,100,100,ENEMY1);
		print_text(280,385,15,"white","2:");
		print_imagem(320,375,100,100,ENEMY2);
		print_text(480,385,15,"white","3:");
		print_imagem(520,375,100,100,ENEMY3);
		print_text(680,385,15,"white","4:");
		print_imagem(720,375,100,100,ENEMY4);
		print_text(830,380,15,"red","to see their range, check");
		print_text(830,400,15,"red","the hint area in-game.");
		printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,23);
		print_imagem(990,420,75,75,X);
		printf ("</a>\n");
	}
}

/** 
\brief Função que imprime a tabela dos Highscores.
@param e Estado atual do jogo.
@param nome_f Nome do ficheiro/jogador.
*/


void print_chart (ESTADO e, char *nome_f) {
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,20);
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",1100,340,210,75);
	print_text(1109,390,30,"black","Highscores");
	printf ("</a>\n");
	if (e.swap.chart==1) {
		int i;
		e=array_highscores(e);
		printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:black;stroke-width:6;stroke:white\" />\n",375,125,565,440);
		for (i=0;i<10 && i<e.numero;i++) printf("<text x=500 y=%d font-family=Mario font-size=35 fill=white> %d. %s - %d </text> \n",175+i*40,i+1,e.campeao[i].nomecampeao,e.campeao[i].scorecampeao);
		printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,20);
		print_imagem(375,125,75,75,X);
		printf ("</a>\n");
	}
}

/**
\brief Função que, dada uma posição relativa, mostra se o inimigo pode atacar essa casa.
@param e Estado atual do jogo.
@param i Índice do inimigo.
@param dx Diferença no x.
@param dy Diferença no y.
*/

void print_range (ESTADO e, int i, int dx, int dy) {
	if (e.inimigo[i].tipo!=2 && e.inimigo[i].vida==1) {
		int x=e.inimigo[i].x+dx;
		int y=e.inimigo[i].y+dy;
		if (pos_vazia(e,x,y)) print_rect(x*TAM,y*TAM,TAM,TAM,"yellow",0.4);
		if (x==e.jog.x && y==e.jog.y) print_rect(x*TAM,y*TAM,TAM,TAM,"yellow",0.4);
	}
	if (e.inimigo[i].tipo==2 && e.inimigo[i].vida==1) {
		int x0 = e.inimigo[i].x + 2*dx;					  
		int y0 = e.inimigo[i].y + 2*dy;
		if (pos_vazia(e,x0,y0)) print_rect(x0*TAM,y0*TAM,TAM,TAM,"yellow",0.4);
		if (x0==e.jog.x && y0==e.jog.y) print_rect(x0*TAM,y0*TAM,TAM,TAM,"yellow",0.4);
	}
}

/**
\brief  Função que mostra o alcance do ataque de todos os inimigos.
@param e Estado atual do jogo.
*/
void print_enemy_range (ESTADO e) {
	int i;
	for (i=0;i<NUM_INIMIGOS+e.nivel;i++) {
		if (e.inimigo[i].range==1) {
			print_range(e,i,+1,0);
			print_range(e,i,0,+1);
			print_range(e,i,-1,0);
			print_range(e,i,0,-1);
			print_range(e,i,-1,-1);
			print_range(e,i,+1,+1);
			print_range(e,i,+1,-1);
			print_range(e,i,-1,+1);
		}
	}
}

/**
\brief  Função que permite selecionar um inimigo específico para ver o ser alcance de ataque.
@param e Estado atual do jogo.
@param nome_f Nome do ficheiro/jogador.
*/

void print_select_specific (ESTADO e, char *nome_f) {
	int i;
	print_text(790,525,16,"red","select an enemy to");
	print_text(790,545,16,"red","view his attack range");
	for (i=0;i<NUM_INIMIGOS+e.nivel;i++) {
		if (e.inimigo[i].vida==1) {
			printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d,%d\" >\n",nome_f,13,i);
			print_rect(e.inimigo[i].x*TAM,e.inimigo[i].y*TAM,TAM,TAM,"blue",0.3);
			print_enemy(e,i);
			printf ("</a>\n");
		}
	}
	print_enemy_range (e);
}

/**
\brief  Função que permite ativar/desativar a ajuda de mostrar o range de todos os inimigos.
@param e Estado atual do jogo.
@param nome_f Nome do ficheiro/jogador.
*/

void print_show_range (ESTADO e, char *nome_f) {
	int i;
	if (e.swap.range==1) print_imagem (675,335,100,90,HELP1);
	else print_imagem (675,335,100,90,HELP1OFF);
	print_text(790,380,20,"black","show all enemies");
	print_text(790,400,20,"black","attack range");
	if (e.swap.range1==0 && e.swap.help==0 && e.swap.chart==0 && e.swap.monsterinfo1==0){
		printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,14);
		if (e.swap.range==1) print_imagem (675,335,100,90,HELP1);
		else print_imagem (675,335,100,90,HELP1OFF);
		printf ("</a>\n");
		if (e.swap.range==1) {
			for (i=0;i<NUM_INIMIGOS+e.nivel;i++) {
				print_range(e,i,+1,0);
				print_range(e,i,0,+1);
				print_range(e,i,-1,0);
				print_range(e,i,0,-1);
				print_range(e,i,-1,-1);
				print_range(e,i,+1,+1);
				print_range(e,i,+1,-1);
				print_range(e,i,-1,+1);
			}
			print_text(790,335,16,"red","yellow means an");
			print_text(790,355,16,"red","enemy can attack there");
		}
	}
}


/**
\brief  Função que permite ativar/desativar a ajuda de mostrar o range de um inimigo em específico.
@param e Estado atual do jogo.
@param nome_f Nome do ficheiro/jogador.
*/

void print_show_range_specific (ESTADO e,char *nome_f) {
	if (e.swap.range1==1) print_imagem (675,435,100,90,HELP2);
	else print_imagem (675,435,100,90,HELP2OFF);
	print_text(790,480,20,"black","show specific enemy");
	print_text(790,500,20,"black","attack range");
	if (e.swap.range==0 && e.swap.help==0 && e.swap.chart==0 && e.swap.monsterinfo1==0){
		printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,15);
		if (e.swap.range1==1) print_imagem (675,435,100,90,HELP2);
		else print_imagem (675,435,100,90,HELP2OFF);
		printf ("</a>\n");
		if (e.swap.range1==1) print_select_specific(e,nome_f);
	}
}

/**
\brief Função que imprime o quadrado com as ajudas.
@param e Estado atual do jogo.
@param nome_f Nome do ficheiro/jogador.
*/

void print_hints (ESTADO e,char *nome_f) {
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",650,275,400,300);
	print_text (665,315,40,"black","Hints:");
	print_show_range(e,nome_f);
	print_show_range_specific (e,nome_f);
}


/**
\brief Função que imprime o aviso "You have lost 1 life.".
@param e Estado atual do jogo.
@param nome_f Nome do ficheiro/jogador.
*/

void print_lostlife (ESTADO e, char *nome_f) {
	if (e.swap.life==1) {
		printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",90,180,420,200);
		print_text(105,220,30,"red","you have lost 1 life.");
		print_text(105,250,30,"red","take care!");
		if (e.jog.vida==2) printf("<text x=105 y=340 font-family=Mario font-size=30 fill=black> %d lives left </text> \n",e.jog.vida);
		else printf("<text x=105 y=340 font-family=Mario font-size=30 fill=black> %d life left </text> \n",e.jog.vida);
		printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,16);
		print_imagem (400,280,80,80,OK);
		printf ("</a>\n");
	}
}

/**
\brief Função que imprime a animação do fogo de explosão.
@param coordx Coordenada no eixo dos x.
@param coordy Coordenada no eixo dos y.
*/

void print_fogo (int coordx, int coordy) {
	printf ("<image id=fogo x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", coordx*TAM,coordy*TAM,TAM,TAM,FOGO);
	printf("<animate xlink:href=#fogo attributeType='auto' attributeName='opacity' to='0' dur='0.8s' begin='0s' fill='freeze' /> \n");
}


/**
\brief Função que imprime a animação "Boom!".
@param coordx Coordenada no eixo dos x.
@param coordy Coordenada no eixo dos y.
*/


void print_boom (int coordx,int coordy) {
	printf ("<image id=boom x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", coordx*TAM,coordy*TAM,TAM,TAM,BOOM);
	printf("<animate xlink:href=#boom attributeType='auto' attributeName='opacity' to='0' dur='0.8s' begin='0s' fill='freeze' /> \n");
}

/**
\brief Função que imprime a animação da explosão da bomba.
@param e Estado atual do jogo.
@param coordx Coordenada no eixo dos x.
@param coordy Coordenada no eixo dos y.
*/


void print_bomb (ESTADO e,int coordx,int coordy) {
	int x=coordx+1;
	int x1=coordx-1;
	int y=coordy+1;
	int y1=coordy-1;
	if (pos_vazia(e,x,coordy) || (elem_enemy_geral(e,x,coordy) && e.inimigo[enemy_index(e,x,coordy)].tipo!=4)) {
		printf ("<image id=expl x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", x*TAM,coordy*TAM,TAM,TAM,EXPL);
		printf("<animate xlink:href=#expl attributeType='auto' attributeName='opacity' to='0' dur='0.8s' begin='0s' fill='freeze' /> \n");
	}
	if (pos_vazia(e,x1,coordy) || (elem_enemy_geral(e,x1,coordy) && e.inimigo[enemy_index(e,x1,coordy)].tipo!=4)) {
		printf ("<image id=expl1 x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", x1*TAM,coordy*TAM,TAM,TAM,EXPL);
		printf("<animate xlink:href=#expl1 attributeType='auto' attributeName='opacity' to='0' dur='0.8s' begin='0s' fill='freeze' /> \n");
	}
	if (pos_vazia(e,coordx,y) || (elem_enemy_geral(e,coordx,y) && e.inimigo[enemy_index(e,coordx,y)].tipo!=4)) {
		printf ("<image id=expl2 x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", coordx*TAM,y*TAM,TAM,TAM,EXPL);
		printf("<animate xlink:href=#expl2 attributeType='auto' attributeName='opacity' to='0' dur='0.8s' begin='0s' fill='freeze' /> \n");
	}
	if (pos_vazia(e,coordx,y1) || (elem_enemy_geral(e,coordx,y1) && e.inimigo[enemy_index(e,coordx,y1)].tipo!=4)) {
		printf ("<image id=expl3 x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", coordx*TAM,y1*TAM,TAM,TAM,EXPL);
		printf("<animate xlink:href=#expl3 attributeType='auto' attributeName='opacity' to='0' dur='0.8s' begin='0s' fill='freeze' /> \n");
	}
	if (pos_vazia(e,x,y) || (elem_enemy_geral(e,x,y) && e.inimigo[enemy_index(e,x,y)].tipo!=4)) {
		printf ("<image id=expl4 x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", x*TAM,y*TAM,TAM,TAM,EXPL);
		printf("<animate xlink:href=#expl4 attributeType='auto' attributeName='opacity' to='0' dur='0.8s' begin='0s' fill='freeze' /> \n");
	}
	if (pos_vazia(e,x1,y1) || (elem_enemy_geral(e,x1,y1) && e.inimigo[enemy_index(e,x1,y1)].tipo!=4)) {
		printf ("<image id=expl5 x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", x1*TAM,y1*TAM,TAM,TAM,EXPL);
		printf("<animate xlink:href=#expl5 attributeType='auto' attributeName='opacity' to='0' dur='0.8s' begin='0s' fill='freeze' /> \n");
	}
	if (pos_vazia(e,x,y1) || (elem_enemy_geral(e,x,y1) && e.inimigo[enemy_index(e,x,y1)].tipo!=4)) {
		printf ("<image id=expl6 x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", x*TAM,y1*TAM,TAM,TAM,EXPL);
		printf("<animate xlink:href=#expl6 attributeType='auto' attributeName='opacity' to='0' dur='0.8s' begin='0s' fill='freeze' /> \n");
	}
	if (pos_vazia(e,x1,y) || (elem_enemy_geral(e,x1,y) && e.inimigo[enemy_index(e,x1,y)].tipo!=4)) {
		printf ("<image id=expl7 x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n", x1*TAM,y*TAM,TAM,TAM,EXPL);
		printf("<animate xlink:href=#expl7 attributeType='auto' attributeName='opacity' to='0' dur='0.8s' begin='0s' fill='freeze' /> \n");
	}
}

/**
\brief Função que imprime a animação de ganhar uma vida.
*/

void print_1up () {
	printf ("<image id=1up x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",1125,75,180,180,ONEUP);
	printf("<animate xlink:href=#1up attributeType='auto' attributeName='opacity' to='0' dur='1s' begin='0s' fill='freeze' /> \n");
}

/**
\brief Função que imprime a animação de passar de nível.
*/

void print_nextlevel () {
	printf ("<image id=next x=%d y=%d width=%d height=%d xlink:href=\"%s\"/>\n",615,145,125,125,NEXT);
	printf("<animate xlink:href=#next attributeType='auto' attributeName='opacity' to='0' dur='1s' begin='0s' fill='freeze' /> \n");
}

/**
\brief Função que utiliza todas as funções constituintes e imprime o jogo.
@param e Estado atual do jogo.
@param nome_f Nome do ficheiro/jogo.
@param acao Ação de jogo, i.e, o que vai fazer o jogador.
@param coordx Coordenada no eixo dos x.
@param coordy Coordenada no eixo dos y.
*/

void print_game (ESTADO e,char *nome_f,int acao,int coordx,int coordy) {
	print_background(FUNDO);
	print_level (e);
	print_score (e);
	print_board();
	print_powerup(e);
	print_plus(e);
	print_door(e);
	print_enemies(e);
	print_walls(e);
	print_hints(e,nome_f);
	if (acao==11 || acao==21) print_fogo(coordx,coordy);
	if (acao==10) print_boom(coordx,coordy);
	if (acao==17) print_bomb(e,coordx,coordy);
	if (acao==6) print_1up();
	print_player(e,nome_f);
	print_lostlife(e,nome_f);
	print_info (e);
	print_giveup(nome_f);
	if (acao==8) print_nextlevel();
	print_chart(e,nome_f);
	print_monsterinfo(e,nome_f);
	print_help(e,nome_f);
}

/**
\brief Função que imprime o "How To Play".
@param e Estado de jogo atual.
@param nome_f Nome do ficheiro/jogador.
*/


void print_howtoplay (ESTADO e, char *nome_f) {
	print_background (FUNDO);
	if (e.swap.monsterinfo==0) {
		print_text (365,100,90,"black","how to play");
		printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,0);
		printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",1100,500,210,75);
		print_text(1120,550,40,"black","return");
		printf ("</a>\n");
		printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",50,125,1000,450);
		print_text(60,150,15,"black","this is a simplified version of a Coffeebreak Roguelike game, in the world of Super Mario.");
		print_text(60,170,15,"black","So, the rules are pretty simple:");
		print_text(60,190,15,"black","1. The objective is to reach the princess, so that you can advance to the next level.");
		print_text(60,210,15,"black","2. You can move to the green areas and you can attack enemies with a red area.");
		print_text(60,230,15,"black","3. If you move an area inside an enemy's attack range, you will lose one life.");
		print_text(60,250,15,"black","4. But don't worry, you will get the oportunity to get extra lives! and you can pick up Special Powers!");
		print_text(60,270,15,"black","Well, these Special Powers are inside the yellow boxes. But what can you do with them?");
		print_imagem(60,280,40,40,NORMAL);
		print_text(105,305,15,"black",": Normal state");
		print_imagem(60,325,40,40,RANGED);
		print_text(105,350,15,"black",": This lets you attack enemies that are not in your normal attack range (+1)");
		print_imagem(60,370,40,40,DESTRUIR);
		print_text(105,395,15,"black",": This lets you destroy obstacles in your radius");
		print_imagem(60,415,40,40,VOAR);
		print_text(105,440,15,"black",": This lets you move to areas that are not in your normal radius (+1)");
		print_imagem(60,460,40,40,BOMB);
		print_text(105,485,15,"black",": This explodes a bomb around you, killing all the enemies in your radius");
		print_text(60,530,15,"black","And that's it. As you can see, it's pretty easy to play.");
		print_text(60,560,15,"black","Now the last, but not the least, rule:");
		print_text(430,560,15,"red","DO NOT REFRESH THE PAGE");
		print_text(660,560,15,"black",", OR THE GAME WILL CRASH!");
		printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",1100,125,210,350);
		print_text(1120,150,20,"black","Score Points:");
		print_text(1110,180,15,"black","Saving the princess:");
		print_text(1110,200,20,"green","50 pts");
		print_text(1110,230,15,"black","Killing monsters:");
		print_text(1110,250,18,"green","see monster info");
		print_text(1110,280,15,"black","Showing range of");
		print_text(1110,295,15,"black","all enemies:");
		print_text(1110,315,20,"red","-20 pts");
		print_text(1110,345,15,"black","Showing range of");
		print_text(1110,360,15,"black","specific enemy:");
		print_text(1110,380,20,"red","-10 pts");
		printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,22);
		printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",1100,20,210,75);
		print_text(1120,50,30,"black","monster");
		print_text(1170,80,30,"black","info");
		printf ("</a>\n");
	}
	else {
		print_text (300,100,90,"black","Monster Info");
		printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,3);
		printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",1100,500,210,75);
		print_text(1120,550,40,"black","return");
		printf ("</a>\n");
		printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",50,125,1000,450);
		print_text(60,150,15,"black","In this game, we have 4 types of enemies, and each type gives you a different amount of points.");
		print_text(60,190,15,"black","1. The melee monsters follow you and try to attack you when you move to their attack range.");
		print_text(60,210,15,"black","They give you 5 points, since they are the easiest to deal with.");
		print_text(60,240,15,"black","2. The ranged monsters have double the range of the melee ones, only attacking interspersed positions.");
		print_text(60,260,15,"black","They also follow you, but they don't attack the positions directly near to them.");
		print_text(60,280,15,"black","They give you 10 points.");
		print_text(60,310,15,"black","Well, there are two types of boss-like monsters. Both of them go to the princess,");
		print_text(60,330,15,"black","but they attack you if you interfere.");
		print_text(60,360,15,"black","3. The first one can only die with the Bomb Power-up, because he has a hard shell.");
		print_text(60,390,15,"black","4. The second one can only die with the Ranged Power-up, since he is only afraid of fire.");
		print_text(60,420,15,"black","The both of them give you 20 points. But take care...");
		print_text(60,450,15,"black","1:");
		print_imagem(100,450,100,100,ENEMY1);
		print_text(260,450,15,"black","2:");
		print_imagem(300,450,100,100,ENEMY2);
		print_text(460,450,15,"black","3:");
		print_imagem(500,450,100,100,ENEMY3);
		print_text(660,450,15,"black","4:");
		print_imagem(700,450,100,100,ENEMY4);
		print_text(810,490,15,"red","to see their range, check");
		print_text(810,510,15,"red","the hint area in-game.");

	}
}

/** 
\brief Função que imprime o menu do jogo.
@param nome_f Nome do ficheiro/jogador.
*/


void print_menu (char *nome_f) {
	print_background (FUNDO);
	print_text (490,100,90,"red","mario:");
	print_text (310,200,90,"black","the roguelike");
	print_imagem(1010,0,300,300,MENU1);
	print_imagem(50,0,300,300,MENU2);
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,2);
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",550,300,210,75);
	print_text(580,360,60,"black","play");
	printf ("</a>\n");
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,3);
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",550,400,210,75);
	print_text(585,435,30,"black","how to");
	print_text(615,465,30,"black","play");
	printf ("</a>\n");
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,18);
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",550,500,210,75);
	print_text(558,550,30,"black","highscores");
	printf ("</a>\n");
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:black;stroke-width:3;stroke:orange\" />\n",930,430,400,150);
	print_text(950,460,15,"orange","to change the username (max 12 chars!)");
	print_text(950,490,15,"orange","just write in the url:");
	print_text(950,520,15,"orange","http://127.0.0.1/cgi-bin/Rogue?username");
	print_text(950,550,15,"orange","replacing username with your name");
}

/** 
\brief Função que coloca a cor diferentes os highscores do jogador atual.
@param e Estado do jogo atual.
@param i Índice no array de highscores.
@param nome_f Nome do ficheiro/jogador.
*/

char *cor (ESTADO e,int i, char *nome_f) {
	char *color=malloc(10*(sizeof (char)));
	if (strcmp(e.campeao[i].nomecampeao,nome_f)==0) strcpy(color,"orange");
	else strcpy(color,"white");
	return color;
}

/**
\brief Função que imprime o ecrã de "gameover".
@param e Estado de jogo atual.
@param nome_f Nome do ficheiro/jogador.
*/

void print_gameover (ESTADO e, char *nome_f) {
	int i;
	print_background (GAMEOVER);
	print_imagem(420,-35,468,291,MORTO);
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,2);
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:black;stroke-width:3;stroke:red\" />\n",510,480,300,109);
	print_text (555,560,75,"red","PLAY!");
	printf ("</a>\n");
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,0);
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:black;stroke-width:3;stroke:red\" />\n",1000,480,300,109);
	print_text (1030,560,75,"red","menu");
	printf ("</a>\n");
	print_text (50,100,50,"white","YOU REACHED");
	printf("<text x=50 y=150 font-family=Mario font-size=50 fill=white> LEVEL %d,</text> \n",e.nivel+1);
	printf("<text x=50 y=200 font-family=Mario font-size=50 fill=orange> %s!</text> \n",nome_f);
	print_text (50,500,60,"red","SCORE:");
	print_text (1000,100,40,"red","HIGHSCORES:");
	for (i=0;i<10 && i<e.numero;i++) printf("<text x=975 y=%d font-family=Mario font-size=25 fill=%s> %d. %s - %d </text> \n",150+i*30,cor(e,i,nome_f),i+1,e.campeao[i].nomecampeao,e.campeao[i].scorecampeao);
    printf("<text x=50 y=550 font-family=Mario font-size=50 fill=white> %d </text> \n",e.score);
}


/**
\brief Função que grava o estado de jogo.
@param e Estado de jogo atual.
@param nome_f Nome do ficheiro/jogador.
*/

void grava_estado(ESTADO e, char *nome_f) {
	FILE *fp;
	char path[100];
	strcpy(path,"/var/www/html/estado/");
	strcat(path,nome_f);
	if ((fp=fopen(path,"w"))==NULL) printf("erro");
	else {
	fprintf (fp,"%s",estado2str(e));
	fclose (fp);
	}
}

/**
\brief Função que lê um estado de jogo.
@param nome_f Nome do ficheiro/jogador.
@returns Estado lido.
*/

ESTADO ler_estado(char *nome_f) {
	FILE *fp;
	char st[10000];
	ESTADO e;
	char path[100];
	strcpy(path,"/var/www/html/estado/");
	strcat(path,nome_f);		
	fp=fopen(path,"r");	
	if (fscanf (fp,"%s",st)==0) printf("erro");
	else {
		fclose(fp);				
		e=str2estado(st);
	}	
	return e;				
}

/**
\brief Função que guarda um score no ficheiro de highscores.
@param e Estado de jogo atual.
@param nome_f Nome do ficheiro/jogo.
*/


void highscore (ESTADO e, char *nome_f) {
	FILE *fp;
	char path [100] = "/var/www/html/estado/highscores";
	fp=fopen(path,"a");
	char st [100];
	char score [10];
	strcpy (st,nome_f);
	sprintf (score,"%d",e.score);
	strcat (st,",");
	strcat (st,score);
	fprintf(fp,"%s \n",st);
	fclose(fp);
} 

/**
\brief Função que imprime o array dos highscores.
@param e Estado de jogo atual.
@param nome_f Nome do ficheiro/jogador.
*/

void print_highscores(ESTADO e,char *nome_f) {
	int i;
	e=array_highscores(e);
	print_background (FUNDO);
	print_text (365,100,90,"black","highscores");
	printf ("<a xlink:href=\"http://127.0.0.1/cgi-bin/Rogue?%s,%d\" >\n",nome_f,0);
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",1100,500,210,75);
	print_text(1120,550,40,"black","return");
	printf ("</a>\n");
	printf ("<rect x=%d y=%d width=%d height=%d style=\"fill:orange;stroke-width:6;stroke:black\" />\n",375,125,565,440);
	for (i=0;i<10 && i<e.numero;i++) printf("<text x=500 y=%d font-family=Mario font-size=35 fill=black> %d. %s - %d </text> \n",175+i*40,i+1,e.campeao[i].nomecampeao,e.campeao[i].scorecampeao);

}

/**
\brief Função que processa ação recebida.
@param e Estado de jogo atual.
@param nome_f Nome do ficheiro/jogador.
@param acao Ação.
@param coordx Coordenada no eixo dos x.
@param coordy Coordenada no eixo dos y.
*/


ESTADO processar_acao (ESTADO e, char *nome_f, int acao, int coordx, int coordy) {
	if (acao==0) {  // abre o menu
		e.swap.menu=1;
		e.swap.howto=0;
		e.swap.highscore=0;
	}
	else if (acao==1) e=ler_estado(nome_f);  						  
	else if (acao==2) e = inicializar(0,e.door.x,e.door.y,0,0,0,3);   
	else if (acao==3) {               								  
		e.swap.menu=0;
		e.swap.howto=1;
		e.swap.monsterinfo=0;
		e.swap.highscore=0;
	}
	else if (acao==4) {												  
		printf ("<animateMotion xlink:href=#jog dur=0.3s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",(e.jog.x-coordx)*TAM,(e.jog.y-coordy)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog10 dur=0.3s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",(e.jog.x-coordx)*TAM,(e.jog.y-coordy)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-10 dur=0.3s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",(e.jog.x-coordx)*TAM,(e.jog.y-coordy)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog11 dur=0.3s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",(e.jog.x-coordx)*TAM,(e.jog.y-coordy)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog1-1 dur=0.3s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",(e.jog.x-coordx)*TAM,(e.jog.y-coordy)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-11 dur=0.3s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",(e.jog.x-coordx)*TAM,(e.jog.y-coordy)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog01 dur=0.3s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",(e.jog.x-coordx)*TAM,(e.jog.y-coordy)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog0-1 dur=0.3s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",(e.jog.x-coordx)*TAM,(e.jog.y-coordy)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-1-1 dur=0.3s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",(e.jog.x-coordx)*TAM,(e.jog.y-coordy)*TAM,0,0);
		e.jog.x=coordx;
		e.jog.y=coordy;
		e= mexemonstros(e,coordx,coordy);
	}
	else if (acao==5) {                                               
		int i=enemy_index(e,coordx,coordy);
		printf ("<animateMotion xlink:href=#jog dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog10 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-10 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog11 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog1-1 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-11 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog01 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog0-1 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-1-1 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		e.inimigo[i].vida=0;
		if (e.inimigo[i].tipo==1) e.score+=5;
		else e.score+=10;
	}
	else if (acao==6) {                                               
		printf ("<animateMotion xlink:href=#jog dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog10 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-10 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog11 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog1-1 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-11 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog01 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog0-1 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-1-1 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		if (e.jog.vida<3) e.jog.vida++;
		e.plus.usado=1;
	}
	else if (acao==7) {                                               
		printf ("<animateMotion xlink:href=#jog dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog10 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-10 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog11 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog1-1 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-11 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog01 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog0-1 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		printf ("<animateMotion xlink:href=#1jog-1-1 dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d %d,%d' /> \n",0,0,(coordx-e.jog.x)*TAM,(coordy-e.jog.y)*TAM,0,0);
		e.jog.state=e.powerup.tipo;
		e.jog.powerup=3;
		e.powerup.usado=1;
	}
	else if (acao==8) {                                               
		e = inicializar(e.nivel + 1,e.door.x,e.door.y,e.score+25,e.jog.state,e.jog.powerup,e.jog.vida);
	}
	else if (acao==9) {												   
		printf ("<animateMotion xlink:href=#jog dur=0.3s begin=0s fill=freeze path='M%d,%d %d,%d' /> \n",(e.jog.x-coordx)*TAM,(e.jog.y-coordy)*TAM,0,0);
		e.jog.x = coordx;
		e.jog.y = coordy;
		e.jog.powerup--;
		e=mexemonstros(e,coordx,coordy);
	}
	else if (acao==10) {											  
		int i = wall_index(e,coordx,coordy);
		e.obstaculo[i].destruido=1;
		e.jog.powerup--;
	}
	else if (acao==11) {											  
		int i=enemy_index(e,coordx,coordy);
		e.inimigo[i].vida=0;
		if (e.inimigo[i].tipo==1) e.score+=5;
		else if (e.inimigo[i].tipo==2) e.score+=10;
		else e.score+=20;
		e.jog.powerup--;
	}
	else if (acao==12) e.jog.vida=0;								  
	else if (acao==13) {                                              
		int k;
		for (k=0;k<NUM_INIMIGOS+e.nivel;k++) e.inimigo[k].range=0;
		e.inimigo[coordx].range=!e.inimigo[coordx].range;
	}
	else if (acao==14) {											  
		e.swap.range=!e.swap.range;
		if (e.swap.range==1) e.score-=20;
	}							 
	else if (acao==15) {											  
		int i;
		for (i=0;i<NUM_INIMIGOS+e.nivel;i++) e.inimigo[i].range=0;
		e.swap.range1=!e.swap.range1;
		if (e.swap.range1==1) e.score-=10;
	}
	else if (acao==16) e.swap.life=0;								  
	else if (acao==17) {											 
		int i;
		int x1 = coordx + 1;
		int x2 = coordx - 1;
		int y1 = coordy + 1;
		int y2 = coordy - 1;
		if (elem_enemy_geral(e,x1,coordy) && e.inimigo[enemy_index(e,x1,coordy)].tipo!=4) {
			i=enemy_index(e,x1,coordy);
			e.inimigo[enemy_index(e,x1,coordy)].vida=0;
		 	if (e.inimigo[i].tipo==1) e.score+=5;
		 	else if (e.inimigo[i].tipo==2) e.score+=10;
		 	else e.score+=20;
		}
		if (elem_enemy_geral(e,x2,coordy) && e.inimigo[enemy_index(e,x2,coordy)].tipo!=4) {
			i=enemy_index(e,x2,coordy);
			e.inimigo[enemy_index(e,x2,coordy)].vida=0;
			if (e.inimigo[i].tipo==1) e.score+=5;
			else if (e.inimigo[i].tipo==2) e.score+=10;
			else e.score+=20;
		}
		if (elem_enemy_geral(e,coordx,y1) && e.inimigo[enemy_index(e,coordx,y1)].tipo!=4) {
			i=enemy_index(e,coordx,y1);
			e.inimigo[enemy_index(e,coordx,y1)].vida=0;
			if (e.inimigo[i].tipo==1) e.score+=5;
			else if (e.inimigo[i].tipo==2) e.score+=10;
			else e.score+=20;
		}
		if (elem_enemy_geral(e,coordx,y2) && e.inimigo[enemy_index(e,coordx,y2)].tipo!=4) {
			i=enemy_index(e,coordx,y2);
			e.inimigo[enemy_index(e,coordx,y2)].vida=0;
			if (e.inimigo[i].tipo==1) e.score+=5;
			else if (e.inimigo[i].tipo==2) e.score+=10;
			else e.score+=20;
		}
		if (elem_enemy_geral(e,x1,y1) && e.inimigo[enemy_index(e,x1,y1)].tipo!=4) {
			i=enemy_index(e,x1,y1);
			e.inimigo[enemy_index(e,x1,y1)].vida=0;
			if (e.inimigo[i].tipo==1) e.score+=5;
			else if (e.inimigo[i].tipo==2) e.score+=10;
			else e.score+=20;
		}
		if (elem_enemy_geral(e,x2,y2) && e.inimigo[enemy_index(e,x2,y2)].tipo!=4) {
			i=enemy_index(e,x2,y2);
			e.inimigo[enemy_index(e,x2,y2)].vida=0;
			if (e.inimigo[i].tipo==1) e.score+=5;
			else if (e.inimigo[i].tipo==2) e.score+=10;
			else e.score+=20;
		}
		if (elem_enemy_geral(e,x1,y2) && e.inimigo[enemy_index(e,x1,y2)].tipo!=4) {
			i=enemy_index(e,x1,y2);
			e.inimigo[enemy_index(e,x1,y2)].vida=0;
			if (e.inimigo[i].tipo==1) e.score+=5;
			else if (e.inimigo[i].tipo==2) e.score+=10;
			else e.score+=20;
		}
		if (elem_enemy_geral(e,x2,y1) && e.inimigo[enemy_index(e,x2,y1)].tipo!=4) {
			i=enemy_index(e,x2,y1);
			e.inimigo[enemy_index(e,x2,y1)].vida=0;
			if (e.inimigo[i].tipo==1) e.score+=5;
			else if (e.inimigo[i].tipo==2) e.score+=10;
			else e.score+=20;
		}
		e.jog.powerup--;
	}
	else if (acao==18) {
		e.swap.menu=0;
		e.swap.howto=0;
		e.swap.highscore=1;
	}
	else if (acao==19) e.swap.help=!e.swap.help; 
	else if (acao==20) e.swap.chart=!e.swap.chart;
	else if (acao==21) {
		int i=enemy_index(e,coordx,coordy);
		e.inimigo[i].vida=0;
		e.score+=20;
		e.jog.powerup--;
	}
	else if (acao==22) e.swap.monsterinfo=1;
	else if (acao==23) e.swap.monsterinfo1=!e.swap.monsterinfo1;
	return e;
}

/**
\brief Função que imprime o jogo.
@param e Estado atual do jogo.
@param nome_f Nome do ficheiro/jogador.
@param acao Ação.
@param coordx Coordenada dos x.
@param coordy Coordenada dos y.
*/

void print_jogo (ESTADO e,char *nome_f,int acao,int coordx,int coordy) {

	if (e.swap.menu==1) print_menu(nome_f);
	else {
		if (e.swap.howto==1) print_howtoplay(e,nome_f);
			else if (e.swap.highscore==1) print_highscores(e,nome_f);
				else {	
					if (e.jog.vida!=0) print_game (e,nome_f,acao,coordx,coordy);
					else {
						highscore (e,nome_f);
						e=array_highscores(e);
						print_gameover (e,nome_f);
					}
				}
	}
}

/** 
\brief Funçao principal, utiliza todas as funções acima. Gera o jogo.
*/

void parser () {
	ESTADO e; 

	print_letra();

	char *args = getenv ("QUERY_STRING");
	char nome_f [12];
	char path [100];
	int acao,coordx,coordy;
	int num;

	if (strlen(args)==0) {
		strcpy(nome_f,"Player");
		acao=0;
	}

	else {
		num=sscanf(args,"%[^,],%d,%d,%d",nome_f,&acao,&coordx,&coordy);
		strcpy(path,"/var/www/html/estado/");
		strcat(path,nome_f);
		if (num==1) {
			if (fopen(path,"r")==NULL) acao=2;
			else acao=1;
		}
		else e=ler_estado(nome_f);
	}

	e=processar_acao(e,nome_f,acao,coordx,coordy);

	print_jogo (e,nome_f,acao,coordx,coordy);

	grava_estado(e, nome_f);
}