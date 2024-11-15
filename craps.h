#ifndef _CRAPS_H
#define _CRAPS_H

//	NOM: MARTIN D'ESCRIENNE
//	PRENOM: Yann
//	N°etu: 21713752

// include
#include <stdio.h>
#include <stdlib.h>

// Line_t représente la ligne ou mise le joueur (Pass, Don't Pass ou rien).
typedef enum {NOTHING, PASS , DONTPASS} Line_t;

//Structure player 
struct player {
	char *name; //son prénom
	int money; //son argent
	int bet; //sa mise
	Line_t player_line; //l'endroit où il a misé
	struct player *suivant; //le joueur suivant (liste chainée)
};

typedef struct player player_t;


// #### Highscore ####

// Initialise le tableau d'highscore avec des 0 puis le charge depuis le fichier highscore.txt
void init_highscore(int *Highscore, int *suspension);

// Sauvegarde le tableau d'highscore dans le fichier highscore.txt.
void Save_highscore(int *Highscore);

// Permet de changer le tableau d'highscore lors d'un nouveau record.
void change_highscore(int score,int *Highscore);


// #### Player ####

// Initialise tous les champs de la structure player.
void init_player(player_t *player);

// Retourne un nouveau joueur avec les champs initialisés.
player_t *newplayer(void);

// Permet d'avoir le nom et l'argent du nouveau joueur. 
void identification(player_t *player);

// Charge le joueur si il existe dans le fichier save.txt. Retourne 1 si le joueur est chargé, 0 sinon.
int read_save(FILE* save, player_t *player);

// Ecrit les données du joueur lorsqu'il quitte la partie.
void write_save(player_t *player);

// Affiche le tableau d'highscore, le point actuel et l'argent ainsi que la mise de tout les joueurs.
void player_info(player_t **addr_first, int Point, int* Highscore);

// Permet d'ajouter un joueur à la partie (ajouter à la fin de la table)
void player_add(player_t **addr_first);

// Permet au joueur de quitter la table. Peut modifier le lanceur actuel et arreter le jeu. 
void player_leave(player_t **player, player_t **addr_first, player_t **ptr_lanceur, int *Continuer);

// Sous-fonction de player_leave : permet de retirer le joueur de la liste chainée.
void delete_player(player_t *player, player_t **addr_first);


// #### Mise ####

// La mise du joueur sur Pass ou Don't Pass quand il n'y a pas encore de point. 
void player_first_bet(player_t *player);

// Sous-fonction de player_first_bet : modifie les champs de player (bet et money) en fonction de ses choix.
void bet_function(player_t *player);

// Gère les modifications possibles des mises au cours de la partie. 
void player_change_bet(player_t *player, int suspension, int *changed);


// #### Jeu ####

// Premiere partie du jeu : établissement du point.
void first_phase(player_t *player, int point_tmp, int *Highscore);

// Deuxieme partie du jeu : 7 ou point.
void second_phase(player_t *player, int point_tmp, int Point, int *Highscore);

// Le lancer de dés par le lanceur actuel.
int dice(player_t *player);

#endif
