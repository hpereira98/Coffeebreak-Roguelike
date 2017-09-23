/**
@file estado.h
Definição do estado e das funções que convertem estados em strings e vice-versa
*/

/** \brief O nº máximo de inimigos */
#define MAX_INIMIGOS		100

/** \brief O nº máximo de obstáculos */
#define MAX_OBSTACULOS		100

/**
\brief Estrutura que armazena uma posição
*/
typedef struct posicao {
	/** Posição no eixo dos x */
	char x;
	/** Posição no eixo dos y */
	char y;
} POSICAO;

/**
\brief Estrutura que armazena as informações relativas ao jogador
*/
typedef struct infoJog {
	/** Posição no eixo dos x do jogador */
	char x;
	/** Posição no eixo dos y do jogador */
	char y;
	/** Vida do jogador */
	char vida;
	/** Estado do jogador */
	char state;   
	/** Quantas utilizações do power-up restam ao jogador */
	char powerup; 
} INFOJOG;

/**
\brief Estrutura que armazena as informações relativas a um inimigo
*/
typedef struct infoEnemy {
	/** Posição no eixo dos x do inimigo */
	char x;
	/** Posição no eixo dos y do inimigo */
	char y;
	/** Vida do inimigo */
	char vida;
	/** Se tem a opção de mostrar o range do inimigo ativada */
	char range;
	/** Tipo do inimigo */
	char tipo;
} INFOENEMY;

/**
\brief Estrutura que armazena as informações relativas a um obstáculo
*/
typedef struct infoWall {
	/** Posição no eixo dos x do obstáculo */
	char x;
	/** Posição no eixo dos y do obstáculo */
	char y;
	/** Estado do obstáculo */
	char destruido;
} INFOWALL;

/**
\brief Estrutura que armazena o estado dos "botões" do jogo
*/
typedef struct button {
	/** Menu ativo/inativo */
	char menu;
	/** Subir de nível */
	char acao;
	/** Ativar/desativar hint geral */
	char range;
	/** Ativar/desativar hint especifica */
	char range1;
	/** Ativar/desativar aviso de -1 vida */
	char life;
	/** Ativar/desativar objeto que regenera vida no mapa */
	char plus;
	/** Página de ajuda ativa/inativa */
	char howto;
	/** Página dos highscores ativa/inativa */
	char highscore;
	/** Ativar/desativar ajuda dentro do jogo */
	char help;
	/** Ativar/desativar tabela de highscores dentro do jogo */
	char chart;
	/** Página das informações dos monstros ativa/inativa */
	char monsterinfo;
	/** Ativar/desativar informações dos monstros dentro do jogo */
	char monsterinfo1;
} BUTTON;

/**
\brief Estrutura que armazena as informações do objeto que recupera uma vida do jogador
*/
typedef struct maisvida {
	/** Posição do objeto no eixo dos x */
	char x;
	/** Posição do objeto no eixo dos y */
	char y;
	/** Estado do objeto */
	char usado;   
} MAISVIDA;

/**
\brief Estrutura que armazena as informações da caixa dos power-ups.
*/
typedef struct powerup {
	/** Posição da caixa no eixo dos x */
	char x;
	/** Posição da caixa no eixo dos y */
	char y;
	/** Tipo do power-up dentro da caixa */
	char tipo;
	/** Estado da caixa */
	char usado; 
} POWERUP;

/**
\brief Estrutura que armazena o nome e o score do jogador
*/
typedef struct campeao { 
	/** Nome do jogador */
	char nomecampeao [12];
	/** Score do jogador */
	int scorecampeao;
} CAMPEAO;


/**
\brief Estrutura que armazena o estado do jogo
*/
typedef struct estado {
	/** Porta anterior */
	POSICAO portanivel;
	/** Nível do jogo */
	char nivel;
	/** Os botões do jogo */
	BUTTON swap;
	/** As infos do jogador */
	INFOJOG jog;
	/** A posição da porta */
	POSICAO door;
	/** Power-ups */
	POWERUP powerup;
	/** Mais vida */
	MAISVIDA plus;
	/** O nº de inimigos */
	char num_inimigos;
	/** O nº de obstáculos */
	char num_obstaculos;
	/** Array com os highscores **/
	CAMPEAO campeao[100];
	/** Numero de registos de jogadores */
	int numero;
	/** Array com a posição dos inimigos */
	INFOENEMY inimigo[MAX_INIMIGOS];
	/** Array com as infos dos obstáculos */
	INFOWALL obstaculo[MAX_OBSTACULOS];
	/** O score do jogo */
	int score;
} ESTADO;

/**
\brief Função que converte um estado numa string
@param e O estado
@returns A string correspondente ao estado e
*/
char *estado2str(ESTADO e);

/**
\brief Função que converte uma string num estado 
@param argumentos Uma string contendo os argumentos passados à CGI
@returns O estado correspondente à string dos argumentos
*/
ESTADO str2estado(char *argumentos);


