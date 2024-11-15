#include <stdio.h>
#include <stdlib.h>
#include "craps.h"

//	NOM: MARTIN D'ESCRIENNE
//	PRENOM: Yann
//	N°etu: 21713752

// Fonction Menu proposant les différentes options possible du joueur.
void Menu(player_t **ptr_player, player_t **addr_first, player_t **ptr_lanceur, int *Continuer, int *Point, int* suspension, int* Highscore){
	int changed = 0; // Variable pour savoir si le joueur a modifié sa mise ou non.

	while(*Continuer){ // Tant qu'il y a au moins un joueur.
		
		player_t *player = *ptr_player;
		char choix[2];
		
		// Si on dépasse le maximum (le highscore sera quand meme enregistré au double si mise remportée).
		// C'est le score maximum que l'on peut atteindre (1 999 999 998).
		if(player->money > 999999999){
			player->money = 999999999;
		}
		
		fprintf(stdout,"\n%s c'est a vous. Que voulez-vous faire ? :\n",player->name);
		fprintf(stdout,"-Miser / Changer sa mise (M)\n-Partir de la table (P)\n-Attendre le lancer de des (D)\n-Ajouter un joueur (A)\n-Info des joueurs (I)\n>> ");
	
		fscanf(stdin,"%s",choix);
		
		switch (*choix){ // En fonction de la réponse du joueur.
			case 'm' :
			case 'M' :
				// Le joueur veux miser ou modifier sa mise.
				if(*suspension == -1){ // Si aucun point.
					if(player->player_line == NOTHING){ // Si il n'a pas encore misé quelque part.
						player_first_bet(player);
					}else{
						printf("Desole mais vous avez deja parie, attendez le point.\n");
					}
				}else{
					if(player->player_line == NOTHING){ // Le joueur n'as pas misé avant. Il doit attendre la fin du point pour miser.
						printf("Desole mais vous n'avez pas parie pour cette manche, attendez la fin de celle-ci.\n");
					}else{
						if(changed){ // Si il a déjà modifié sa mise.
							printf("Desole mais vous avez deja changer votre mise.\n");
						}else{
							player_change_bet(player,*suspension,&changed);
						}
					}
				}
				break;
			
			case 'p' :
			case 'P':
				// Si le joueur veut quitter la partie.
				player_leave(ptr_player,addr_first,ptr_lanceur, Continuer);
				break;
				
			case 'i' :
			case 'I':
				// Si le joueur veux avoir les informations du jeu.
				player_info(addr_first,*Point, Highscore);
				break;
			
			case 'd' :
			case 'D' :
				// Le joueur vote pour le lancer de dé. On le fait quitter la fonction Menu.
				if(player->player_line == NOTHING && *suspension == -1){
					char rep[2];
					printf("Vous n'avez pas mise, voulez-vous quand meme continuer ?: (o/n)\n>> ");
					fscanf(stdin,"%2s",rep);
					if(*rep == 'o'){
						return;
					}
				}
				else{
					return;
				}
				break;
			
			case 'a' :
			case 'A' :
				// On ajoute un nouveau joueur à la fin de la table.
				player_add(addr_first);
				break;
				
			default :
				// Faute de frappe.
				fprintf(stdout,"Veuillez entrer un choix valide\n");
				break;	
		}	
	}
}

// Fonction du jeu de Nicecraps.
void Game(player_t **ptr_player, player_t **addr_first, player_t **ptr_lanceur, int *Point, int *suspension, int* Highscore){
	
	int point_tmp = dice(*ptr_lanceur);// Le score obtenu aux dés.
	player_t *curr_player = *addr_first; // On initialise le joueur actuel au premier de la liste chainée.
	
	if(*Point == 0){ //si pas de point avant.
		while(curr_player != NULL){// met a jour l'argent de tout les joueurs un par un en fonction du score obtenu.
			first_phase(curr_player,point_tmp, Highscore);
			curr_player = curr_player->suivant;
		}
		if(point_tmp != 2 && point_tmp != 7 && point_tmp != 12){ //Si le score fait un nouveau point.
			*Point = point_tmp;
			*suspension = 0;
			printf("\n\n*** Le point est determine a %d. ***\n*** Vous pouvez changer votre mise selon votre ligne. ***\n",*Point);
		}
	}
	else{// Il y a déjà un point.
		while(curr_player != NULL){// met a jour l'argent de tout les joueurs un par un en fonction du score obtenu.
			second_phase(curr_player, point_tmp, *Point, Highscore);
			curr_player = curr_player->suivant;
		}
		if(point_tmp == 7){// Si le point termine la manche et change le lanceur.
			if((*ptr_lanceur)->suivant == NULL){// Si le lanceur est à la fin de la liste chainée.
				*ptr_lanceur = *addr_first;// retour au début de la liste. (même si seul)
				if((*ptr_lanceur)->suivant != NULL){// Si le joueur n'est pas seul on affiche le changement de lanceur.
					printf("\n\n*** Le lanceur est a present %s. ***",(*ptr_lanceur)->name);
				}
			}
			else{// Sinon on passe au lanceur suivant.
				*ptr_lanceur = (*ptr_lanceur)->suivant;
				printf("\n\n*** Le lanceur est a present %s. ***",(*ptr_lanceur)->name);
			}
			// Nouvelle manche
			*Point = 0; 
			printf("\n\n*** Le point est reinitialise. ***\n");
			*suspension = -1;
			
		}else if(point_tmp == *Point){// Si le point termine la manche.
			// Nouvelle manche
			*Point = 0;
			printf("\n\n*** Le point est reinitialise. ***\n");
			*suspension = -1;
			
		}else{//Sinon on incrémente suspension. (Le point est toujours là).
			*suspension += 1;
			if(*suspension == 1){
				printf("\n\n*** Vous pouvez doubler votre mise. ***\n");
			}
			else{
				printf("\n\n*** Vous pouvez diminuer votre mise. ***\n");
			}
		}
	}
	printf("\n");		
}

		
// Fonction MAIN.
int main(int argc, char *argv[]) {
	printf(">>> Bienvenue au jeu de Nicecraps <<<\n\n");
	
	// Le premier joueur de la liste chainée. 
	// (également celui qui est présent depuis le plus longtemps à la table).
	player_t *first_player = newplayer();  // Initialisation
	player_t **addr_first = &first_player; // Le pointeur sur le premier joueur.
	
	// Le joueur actuel dans la boucle.
	player_t *player = first_player; // Le premier joueur est le joueur actuel.
	player_t **tmp = &player; // Le pointeur sur le joueur actuel.
	
	// Le lanceur actuel.
	player_t *lanceur = first_player; // Le premier joueur est le lanceur.
	player_t **ptr_lanceur = &lanceur; // Le pointeur sur le lanceur actuel.
	
	int Continuer = 1; // Tant que l'entier vaut 1 la partie continue.
	int Point = 0;
	int suspension = -1; // Variable pour situer l'état du jeu.
	// -1 pour aucun point; 0 pour un point; 1 pour la 1ere suspension du jeu; ect....
	
	// Tableau d'highscore pouvant contenir 5 records classés.
	int Highscore[5];
	init_highscore(Highscore,&suspension); // On charge le highscore.
	
		
	// Boucle principale.
	while(Continuer){
		Menu(tmp, addr_first ,ptr_lanceur ,&Continuer, &Point, &suspension, Highscore);
		
		//Si il y a un joueur et que tout les joueurs précédent on votés pour le lancer de dé.
		if((*tmp)->suivant == NULL && Continuer){ 
			Game(tmp, addr_first, ptr_lanceur, &Point, &suspension, Highscore);
		}
		
		//Si ce n'est pas le dernier joueur.
		if((*tmp)->suivant != NULL){
			*tmp = (*tmp)->suivant;
		
		//Sinon on repart au debut de la liste chainée (si le joueur n'est pas seul). 
		}else if (*tmp != *addr_first){
			*tmp = *addr_first;
		}
	}
	
	// Fin de partie quand il n'y a plus de joueurs a la table.
	Save_highscore(Highscore); // On sauvegarde le Highscore.
	printf("\nIl n'y a plus de joueur a la table. La partie est terminee.\n>>> A bientot. <<<\n");
	system("pause");
	return 0;
	
}
