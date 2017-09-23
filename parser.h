/**
@file parser.h
Definição de valores globais para todo o jogo e das imagens a serem utilizadas. Contém ainda a definição de alguma funções do jogo.
*/

#ifndef ___PARSER_H___
#define ___PARSER_H___

/** \brief O tamanho do mapa */
#define SIZE 10
/** \brief O tamanho dos quadrados do mapa */
#define TAM 60
/** \brief O nº inicial de inimigos */
#define NUM_INIMIGOS 10
/** \brief O nº inicial de obstáculos */
#define NUM_WALLS 20

/** \brief A imagem do jogador no estado normal */
#define PLAYER "http://127.0.0.1/player.png"
/** \brief A imagem do jogador no modo ranged */
#define PLAYER1 "http://127.0.0.1/marioranged.png"
/** \brief A imagem do jogador no modo de destruir obstáculos */
#define PLAYER2 "http://127.0.0.1/mariodestruir.png"
/** \brief A imagem do jogador no modo de voar */
#define PLAYER3 "http://127.0.0.1/mariovoar.png"
/** \brief A imagem do jogador no modo bomba */
#define PLAYER4 "http://127.0.0.1/mariobomb.png"
/** \brief A imagem dos inimigos melee */
#define ENEMY1 "http://127.0.0.1/inimigo1.png"
/** \brief A imagem dos inimigos ranged */
#define ENEMY2 "http://127.0.0.1/inimigo2.png"
/** \brief A imagem do "boss" que só morre com a bomba */
#define ENEMY3 "http://127.0.0.1/inimigo3.png"
/** \brief A imagem do "boss" que só morre com o fogo */
#define ENEMY4 "http://127.0.0.1/inimigo4.png"
/** \brief A imagem do obstáculo 1 (tijolo) */
#define WALL1 "http://127.0.0.1/obstaculo1.png"
/** \brief A imagem do obstáculo 2 (tubo) */
#define WALL2 "http://127.0.0.1/obstaculo2.png"
/** \brief A imagem do obstáculo 3 (pedra) */
#define WALL3 "http://127.0.0.1/obstaculo3.png"
/** \brief A imagem da "porta" (princesa) */
#define DOOR "http://127.0.0.1/porta.png"
/** \brief A imagem da bomba de destuição dos obstáculos */
#define BOMBA "http://127.0.0.1/bomba.png"
/** \brief A imagem do chão */
#define FLOOR "http://127.0.0.1/chao.png"
/** \brief A imagem de fundo do game over */
#define GAMEOVER "http://127.0.0.1/gameover.jpg"
/** \brief A imagem de fundo do jogo em geral */
#define FUNDO "http://127.0.0.1/fundo.png"
/** \brief A imagem do cursor normal de jogo */
#define CURSOR1 "http://127.0.0.1/cursor1.cur"
/** \brief A imagem do cursor das hiperligações */
#define CURSOR2 "http://127.0.0.1/cursor2.cur"
/** \brief A imagem do Mario morto */
#define MORTO "http://127.0.0.1/mariomorto.png"
/** \brief A letra personalizada do jogo */
#define LETRA "http://127.0.0.1/mario.ttf"
/** \brief A imagem do coração vermelho das vidas */
#define VIDA1 "http://127.0.0.1/vida1.png"
/** \brief A imagem do coração cinzento das vidas */
#define VIDA0 "http://127.0.0.1/vida0.png"
/** \brief A imagem da caixa dos power-ups */
#define POWER "http://127.0.0.1/powerup.png"
/** \brief A imagem do estado normal do Mario */
#define NORMAL "http://127.0.0.1/normal.png"
/** \brief A imagem do estado ranged do Mario */
#define RANGED "http://127.0.0.1/ranged.png"
/** \brief A imagem do estado voador do Mario */
#define VOAR "http://127.0.0.1/voar.png"
/** \brief A imagem do estado destruidor do Mario */
#define DESTRUIR "http://127.0.0.1/destruir.png"
/** \brief A imagem do estado bombista do Mario */
#define BOMB "http://127.0.0.1/bomb.png"
/** \brief A imagem da ajuda geral ativada */
#define HELP1 "http://127.0.0.1/range.jpg"
/** \brief A imagem da ajuda geral desativada */
#define HELP1OFF "http://127.0.0.1/rangeoff.jpg"
/** \brief A imagem da ajuda específica ativada */
#define HELP2 "http://127.0.0.1/range1.jpg"
/** \brief A imagem da ajuda específica desativada */
#define HELP2OFF "http://127.0.0.1/range1off.jpg"
/** \brief A imagem do "ok" */
#define OK "http://127.0.0.1/ok.png"
/** \brief A imagem da cruz para fechar uma janela */
#define X "http://127.0.0.1/close.png"
/** \brief A imagem do coração que regenera uma vida do Mario */
#define PLUS "http://127.0.0.1/plus.png"
/** \brief A imagem do Mario da direita da página inicial */
#define MENU1 "http://127.0.0.1/menu1.png"
/** \brief A imagem do Mario da esquerda da página inicial */
#define MENU2 "http://127.0.0.1/menu2.png"
/** \brief A imagem do fogo do power-up ranged */
#define FOGO "http://127.0.0.1/fogo.png"
/** \brief A imagem da explosão resultante da utilização do power-up de destruir obstáculos */
#define BOOM "http://127.0.0.1/boom.png"
/** \brief A imagem da explosão do power-up bomba */
#define EXPL "http://127.0.0.1/explosao.png"
/** \brief A imagem que surge e desvanece quando se apanha uma vida */
#define ONEUP "http://127.0.0.1/1up.png"
/** \brief A imagem que surge e desvanece quando se passa de nível */
#define NEXT "http://127.0.0.1/level.png"

/** 
\brief Funçao principal, utiliza todas as funções acima. Gera o jogo.
*/
void parser ();

/**
\brief Função que imprime os quadrados do tabuleiro.
@param px Posição no eixo dos x.
@param py Posição no eixo dos y.
@param tam Tamanho da imagem
*/
void print_square (int px,int py,int tam) ;

/**
\brief Função que imprime um retângulo.
@param x Posição do retângulo em x.
@param y Posição do retângulo em y.
@param w Largura do retângulo.
@param h Comprimento do retângulo.
@param cor Cor do retângulo.
@param o Opacidade do retângulo.
*/
void print_rect (int x, int y, int w, int h, char *cor, float o);


#endif