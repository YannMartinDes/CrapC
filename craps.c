#include "craps.h"

//	NOM: MARTIN D'ESCRIENNE
//	PRENOM: Yann
//	N°etu: 21713752


//########### Highscore ############

void init_highscore(int *Highscore, int *suspension){
	int i,j;
	for(i=0;i<5;i++){
		Highscore[i] = 0; //Tableau rempli de scores nuls.
	}
	FILE* save_highscore = NULL;
	char temp[11];
	save_highscore = fopen("highscore.txt","r");
	
	if(save_highscore != NULL){// Si l'ouverture rיussie.
		int h;
		j = 0;
		char ligne[50];
		while(j<5 && fgets(ligne,50,save_highscore) != NULL){ 
		// j<5 יgalement car la derniטre ligne contient du vide et pas NULL, א cause du '\n'.
			
			for(h=0;h<11;h++){ // On vide temp.
				temp[h] = '\0';
			}
			i = 0;
			while(ligne[i] != ';'){// On lit le iטme highscore.
				temp[i] = ligne[i];
				i += 1;
			}
			Highscore[j] = atoi(temp); // On ecrit le iטme score convertit dans le tableau.
			j += 1;// On passe au score suivant.
		}
		fclose(save_highscore);
	}
}

void Save_highscore(int *Highscore){
	FILE *save_highscore = NULL;
	save_highscore = fopen("highscore.txt","w");// Le fichier est crיי si inיxistant.
	
	int i =0,n,j;
	int i_score;
	char nb_coma[12];// Le tableau du nombre de ';' 

	while(i<5){// Tant qu'il reste des highscore.
		n = 0;
		i_score = Highscore[i]; // iטme score.
		
		if(i_score == 0){ // Si le iטme highscore est nul.
			n = 1;
		}
		
		for(j=0;j<11;j++){ // On vide le tableau rempli prיcיdemment.
			nb_coma[j] = '\0';
		}
		
		// le nombre de ';' qu'il faudra en fonction du nombre de caractטres de du iטme score.
		while(i_score%10 != 0 || i_score/10 != 0){
			n += 1;
			i_score = i_score / 10;
		}
		for(j=0;j < 11-n;j++){// On rempli le tableau de ';' nיcיssaire.
			nb_coma[j] = ';';
		}
		
		fprintf(save_highscore,"%d%s\n",Highscore[i],nb_coma);// On ecrit dans le fichier.
		// Pas besoin de ';' ici car highscore[i] ne peux qu'augmenter. 
		i += 1;// Score suivant.
	}
	fclose(save_highscore);
}

void change_highscore(int score,int *Highscore){
	int i,temp1,temp2;
	for(i=0;i<5;i++){
		if(score > Highscore[i]){// Si nouveau record.
			temp1 = Highscore[i];
			Highscore[i] = score;
			printf("\nNouveau record de gain a la %deme position : %d$\n",i+1,Highscore[i]);
			i +=1;
			for(i;i<5;i++){ // On dיplace les scores en fonction du nouveau record.
				temp2 = Highscore[i];
				Highscore[i] = temp1;
				temp1 = temp2;
			}
			break;
		}	
	}
}

//########### Player ############

void init_player(player_t *player) {
	player->name = NULL;
	player->money = -1;
	player->bet = 0;
	player->player_line = NOTHING;
	player->suivant = NULL; 
}

player_t *newplayer(void){
	// On alloue dynamiquement le nouveau joueur.
	player_t *newplayer =(player_t *)malloc(sizeof(struct player));
	init_player(newplayer);
	identification(newplayer);
	return newplayer;
}

void identification(player_t *player){
	FILE* save = NULL;
	int load; // Vaut 1 si le joueur a יtי chargי. 0 sinon.
	// Son nom n'est pas une variable locale. On l'alloue.
	char *name = (char *) malloc(sizeof(char)*30);
	
	fprintf(stdout,"Nouveau joueur veuillez donner votre prenom:\n>> ");
	fscanf(stdin,"%s",name);
	player->name = name;  // On prend le nom du nouveau joueur.
	
	save = fopen("save.txt","r");
	if(save != NULL){
		load = read_save(save,player); // On regarde dans la sauvegarde.
		fclose(save);
	}
	else{// Si il n'existe pas on crיer le fichier (vide).
		save = fopen("save.txt","w");
		fclose(save);
 	}
 	
 	if(load == 0){ // Si le joueur n'a pas יtי chargי.
		char temp[10];
		int numb;
		while(player->money < 0){
			fprintf(stdout,"Combien d'argent avez-vous ? :\n>> ");
			fscanf(stdin,"%9s",temp);
			numb = atoi(temp);
		
			while(numb <= 0){ // Si rיponse inconvenable.
				fprintf(stdout,"Entrez une somme valide.\n>> ");
				fscanf(stdin,"%9s",temp);
				numb = atoi(temp);
			}	
			player->money = numb;
		}	
	}
}

int read_save(FILE* save, player_t *player){
	char ligne[50];
	char *name = player->name;
	int i,not_idem = 1; // not_idem vaut 1 si le nom du joueur n'est pas trouvי, 0 sinon.
	
	while(not_idem && fgets(ligne,50,save) != NULL){
		// On lit chaque ligne du fichier si le joueur n'est pas trouvי.
		not_idem = 0;
		for(i=0;name[i]!='\0';i++){
			if(ligne[i] != name[i]){
				not_idem = 1; // Le joueur ne correspond pas au nom de la ligne actuelle.
				break;
			}
		}
		if(not_idem == 0){ // On vיrifie que le nom de la ligne est bien fini.
			if(ligne[i] != ','){
				not_idem = 1;
			}
		}
	}
	
	if(not_idem){// Le joueur n'as pas יtי trouvי.
		return 0;
	}
	else{
		char rep[2];
		printf("%s voulez-vous recharger votre partie ?: (o/n)\n>> ",name);
		fscanf(stdin,"%2s",rep);
		
		if(*rep == 'o'){
			char temp[10];
			int j=0;
			i++;
			for(i;ligne[i] != ';';i++){// On lit l'argent sauvegardי sur la ligne actuelle.
				temp[j] = ligne[i];
				j++;
			}
			player->money = atoi(temp);// On le convertit en entier. 
			return 1;// Le joueur a יtי trouvי.
		}
		else{
			return 0;// Le joueur ne veux pas recharger sa partie.
		}
	}
}

void write_save(player_t *player){
	FILE *save = NULL;
	save = fopen("save.txt","r+");
	
	char ligne[50];
	char *name = player->name;
	int money = player->money; // money va ךtre modifiי.
	int i,not_idem = 1;
	
	int old_curs = 0,currnt_curst, taille; // Variable pour calculer la taille de la ligne actuelle.
	
	while(not_idem && fgets(ligne,45,save) != NULL){
		// On lit chaque ligne du fichier si le joueur n'est pas trouvי.
		currnt_curst = ftell(save);// Position du curseur א la fin de la ligne actuelle.
		taille = currnt_curst - old_curs;// taille de la ligne actuelle.
		old_curs = currnt_curst;// Position du curseur א la fin de la ligne prיcיdente.
		
		not_idem = 0;
		for(i=0;name[i]!='\0';i++){
			if(ligne[i] != name[i]){
				not_idem = 1;// Le joueur ne correspond pas au nom de la ligne actuelle.
				break;
			}
		}
		if(not_idem == 0){
			if(ligne[i] != ','){
				not_idem = 1;// Le joueur ne correspond pas au nom de la ligne actuelle.
			}
		}
	}
	if(not_idem){//Le joueur n'existait pas, on l'ecrit dans save.txt.
		// avec 0 chiffre il y a 10 ";" pour amortir l'יcrasement des futurs write (le somme peut faire jusqu'א 9 caractטres).
		int n = 0;
		
		if(money == 0){// Pour ne pas avoir 10 ';' si le joueur a 0$.
			n = 1;
		}
		
		char nb_coma[11];// le nombre de ';' qu'il faudra en fonction du nombre de caractטres de money.
		while(money%10 != 0 || money/10 != 0){
			n += 1;
			money = money / 10;
		}
		
		for(i=0;i<11;i++){ // On rempli nb_coma de '\0'.
			nb_coma[i] = '\0';
		}
		for(i=0;i < 10-n;i++){ // On place le nombre de ';' nיcיssaire.
			nb_coma[i] = ';';
		}
		
		// On ecrit donc le nombre de ';' qu'il faut. (pour ne pas prendre de place avec des ';' inutiles)
		fprintf(save,"%s,%d%s\n",name,player->money,nb_coma);
		
	}else{
		fseek(save,-taille,SEEK_CUR); // On revient au dיbut de la ligne actuelle.
		fprintf(save,"%s,%d;",name,money);// On ecrit la nouvelle somme d'argent du joueur.
		//";" א la fin pour couper si on passe de 1000 א 50 par exemple. (sinon on aurait 5000$)
	}
	fclose(save);
}

void player_info(player_t **addr_first, int Point, int* Highscore){
	player_t *curr_player = *addr_first;
	int i;
	printf("#######################################\n"); 
	printf("Highscore :\n");// Affichage du tableau d'highscore.
	for(i=0;i<5;i++){
		printf("[%d]: %d$\n",i+1,Highscore[i]);
	}
	
	printf("\nPoint en cours : %d\n",Point);// Affichage du point actuel.
	
	while(curr_player != NULL){ // Affichage des infos des joueurs.
		if(curr_player->player_line == PASS){
			printf("Nom: %s , Argent: %d$ , Bet: %d$ sur PassLine\n",curr_player->name,curr_player->money,curr_player->bet);
		}else if(curr_player->player_line == DONTPASS){
			printf("Nom: %s , Argent: %d$ , Bet: %d$ sur Don\'t PassLine\n",curr_player->name,curr_player->money,curr_player->bet);
		}else{
			printf("Nom: %s , Argent: %d$\n",curr_player->name,curr_player->money);
		}
		curr_player = curr_player->suivant;
	}
	printf("#######################################\n");
}

void player_add(player_t **addr_first){
	player_t *add_player = newplayer();
	player_t *curr_player = *addr_first;
	
	while(curr_player->suivant != NULL){ // On se place sur le dernier joueur actuel.
		curr_player = curr_player->suivant;
	}
	add_player->suivant = NULL; // Insertion du nouveau joueur א la fin de la liste chainיe.
	curr_player->suivant = add_player;
}

void player_leave(player_t **player, player_t **addr_first, player_t **ptr_lanceur, int *Continuer){
	player_t *to_delete = *player; // Le joueur א effacer.
	char rep[2];
	printf("\nVoulez-vous vraiment partir, vous avez %d$",to_delete->money);
	if(to_delete->bet != 0){
		char* line; 
		if(to_delete->player_line == PASS){
			printf(" et parie %d$ sur PassLine. (o/n)\n*** La mise ne sera pas enregistree. ***\n>> ",to_delete->bet);
		}else {
			printf(" et parie %d$ sur Don\'t PassLine. (o/n)\n*** La mise ne sera pas enregistree. ***\n>> ",to_delete->bet);
		}
	}else{
		printf(" (o/n)\n>> ");
	}
	
	fscanf(stdin,"%2s",rep);
	if(*rep == 'o'){
		write_save(to_delete);// On ecrit dans save.txt les infos du joueur quittant la partie.
		printf("Au revoir %s.\n",to_delete->name);
		delete_player(to_delete, addr_first); // On le retire de la liste chainיe.
		
		//Si c'יtait le lanceur actuel.
		if(*player == *ptr_lanceur){
			if((*player)->suivant != NULL){ // Si ce n'יtait pas le dernier.
				*ptr_lanceur =(*ptr_lanceur)->suivant;
				printf("\n*** Le lanceur est a present %s ***\n",(*ptr_lanceur)->name);
			}
			else{// Si c'יtait le dernier.
				*ptr_lanceur = *addr_first; // On retourne au dיbut de la liste chainיe.
				if((*ptr_lanceur)->suivant != NULL){ // Si le joueur n'יtait pas seul.
					printf("\n*** Le lanceur est a present %s ***\n",(*ptr_lanceur)->name);
				}
			}
			
		}
		
		if(*player == *addr_first){ // Si le joueur יtait le premier de la liste.
			if((*player)->suivant != NULL){ // Si il n'יtait pas seul.
				*player = (*player)->suivant;
				*addr_first = (*addr_first)->suivant; // Le premier est maintenant le deuxiטme de la liste.
			}else{
				*Continuer = 0; // Il יtait seul. On arrete la boucle principale.
			}
		}else{// Sinon on passe au suivant.
			if((*player)->suivant != NULL){
				*player = (*player)->suivant;
			}else{// On retourne au dיbut de la liste chainיe.
				*player = *addr_first;
				
			}
		}
		
		free(to_delete); // On free le joueur qui a quittי la partie.
	}	
}

void delete_player(player_t *player, player_t **addr_first){
	player_t *current_player = *addr_first;
	player_t *old_current;
	
	if(player != *addr_first){
		while(current_player != player){
		// On parcours la liste jusqu'au joueur א supprimer en mיmorisant le prיcיdent.
			old_current = current_player; 
			current_player = current_player->suivant;
		}
		//On supprime le joueur de la liste chainיe.
		old_current->suivant = current_player->suivant;
	}
}

//########### Mise ############

void player_first_bet(player_t *player){
	printf("\nOu voulez-vous miser ? (P) PassLine / (D) Don't PassLine / (Q) Annuler\n>> ");
	char rep[2];
	fscanf(stdin,"%2s",rep);
	while(*rep !='p' && *rep != 'P' && *rep !='D' && *rep !='d' && *rep !='Q' && *rep !='q'){
		printf("Veuillez entrez un choix valide\n>> ");
		fscanf(stdin,"%2s",&rep);
	}
	
	switch (*rep){
		case 'p' :
		case 'P' :
			// Le joueur mise sur Passline.
			player->player_line = PASS;
			bet_function(player);
			break;
		
		case 'D' :
		case 'd':
			// le joueur mise sur Don't Passline.
			player->player_line = DONTPASS;
			bet_function(player);
			break;
		
		case 'Q' :
		case 'q' :
			break;
	}
	
}

void bet_function(player_t *player){
	char *line;
	int bet; 
	if(player->player_line == PASS){
		line = "PassLine";
	}
	else if (player->player_line == DONTPASS){
		line = "Don't PassLine";
	}
	
	char temp[10];
	printf("%s combien voulez-vous miser sur %s ? (vous avez %d$)\n>> ",player->name,line,player->money);
	fscanf(stdin,"%9s",temp);
	bet = atoi(temp);
	
	while(bet <= 0 || bet > player->money){ // Si la mise est correcte.
		printf("Veuillez saisir une valeur correcte et inferieure a votre argent actuel.\n>> ");
		fscanf(stdin,"%9s",temp);
		bet = atoi(temp);
	}
	// On modifie les champs du joueurs en fonction de son choix.
	player->money -= bet; 
	player->bet = bet; 
	printf("Vous avez parie %d $ sur %s, il vous reste %d $\n",player->bet,line,player->money);
}

void player_change_bet(player_t *player, int suspension, int *changed){
	char rep[2];
	char temp[10];
	int newmise; // La somme que le joueur veux ajouter ou bien retirer.
	
	if(suspension == 0){ // Si le point viens d'ךtre fait.
	
		if(player->player_line == PASS){ // Il peut augmenter sa mise.
			printf("Vous pouvez augmenter votre mise, voulez-vous le faire (o/n):\n>> ");
			fscanf(stdin,"%2s",rep);
			if(*rep == 'o' || *rep =='O'){
				printf("%s avez parie %d$ sur PassLine et vous avez %d$\n",player->name,player->bet,player->money);
				printf("De combien voulez-vous augmenter votre mise ? :\n>> ");
				fscanf(stdin,"%9s",temp);
				newmise = atoi(temp);
				
				while(newmise < 0 || newmise > player->money){ // Si la mise א ajouter est valide.
					printf("Veuillez saisir une valeur correcte et inferieure a votre argent actuel.\n>> ");
					fscanf(stdin,"%9s",temp);
					newmise = atoi(temp);
				}
				// On modifie les champs du joueurs en fonction de ses choix.
				player->money -= newmise;
				player->bet += newmise;
				*changed = 1; // Il ne peux plus changer sa mise.
				printf("Vous avez parie %d$ sur PassLine et il vous reste %d$\n",player->bet,player->money);
			}
		}else{// Don't Pass, Il peut diminuer sa mise.
			printf("Vous pouvez diminuer votre mise, voulez-vous le faire (o/n):\n>> ");
			fscanf(stdin,"%2s",rep);
			if(*rep == 'o' || *rep =='O'){
				printf("%s avez parie %d$ sur Don't PassLine et vous avez %d$\n",player->name,player->bet,player->money);
				printf("De combien voulez-vous diminuer votre mise ? :\n>> ");
				fscanf(stdin,"%9s",temp);
				newmise = atoi(temp);
				
				while(newmise < 0 || newmise > player->bet){// Si la mise א ajouter est valide.
					printf("Veuillez saisir une valeur correcte et inferieure ou egale a votre mise actuel.\n>> ");
					fscanf(stdin,"%9s",temp);
					newmise = atoi(temp);
				}
				// On modifie les champs du joueurs en fonction de ses choix.
				player->money += newmise;
				player->bet -= newmise;
				*changed = 1;// Il ne peux plus changer sa mise.
				printf("Vous avez parie %d$ sur Don't PassLine et il vous reste %d$\n",player->bet,player->money);
			}
		}
	}
	else if(suspension == 1){ // Si il s'agit de la premiטre suspension.
		if(player->player_line == PASS){
			printf("%s avez parie %d$ sur PassLine et vous avez %d$\n",player->name,player->bet,player->money);
		}else{
			printf("%s avez parie %d$ sur Don't PassLine et vous avez %d$\n",player->name,player->bet,player->money);
		}
		printf("Vous pouvez doubler votre mise, voulez-vous le faire (o/n):\n>> ");
		fscanf(stdin,"%2s",rep);
		if(*rep == 'o' || *rep =='O'){
			if(player->money >= player->bet){// Si il a l'argent pour doubler sa mise.
				player->money -= player->bet;
				player->bet *= 2;
				*changed = 1;// Il ne peux plus changer sa mise.
				if(player->player_line == PASS){
					printf("Vous avez maintenant %d$ sur PassLine et vous avez %d$\n",player->bet,player->money);
				}else{
					printf("Vous avez maintenant %d$ sur Don't PassLine et vous avez %d$\n",player->bet,player->money);
				}	
			}else{// Sinon message d'erreur. 
				printf("Vous n'avez pas assez d'argent pour doubler votre mise.\n");
			}
		}
	}
	else{// Ce n'est pas la premiטre suspension.
		if(player->player_line == PASS){
			printf("%s avez parie %d$ sur PassLine et vous avez %d$\n",player->name,player->bet,player->money);
		}else{
			printf("%s avez parie %d$ sur Don't PassLine et vous avez %d$\n",player->name,player->bet,player->money);
		}
		printf("Vous pouvez diminuer votre mise d'un facteur 2, voulez-vous le faire (o/n):\n>> ");
		fscanf(stdin,"%2s",rep);
		if(*rep == 'o' || *rep =='O'){// Il peut diviser sa mise par deux (et rיcuperer l'argent retirי).
			player->money += player->bet / 2;
			player->bet -= player->bet / 2;
			*changed = 1;
			if(player->player_line == PASS){
				printf("Vous avez maintenant %d$ sur PassLine et vous avez %d$\n",player->bet,player->money);
			}else{
				printf("Vous avez maintenant %d$ sur Don't PassLine et vous avez %d$\n",player->bet,player->money);
			}
		}
	}
}

//########### Jeu ############

void first_phase(player_t *player, int point_tmp, int *Highscore){
	if(player->player_line == PASS){ // Si le joueur a misי sur Passline.
		if(point_tmp == 7){// Il gagne. 
			printf("\n%s remporte %d$.",player->name,(player->bet * 2));
			change_highscore((player->bet * 2),Highscore); // Si son gain est un nouveau record.
			
			player->money += player->bet * 2;
			player->bet = 0;
			player->player_line = NOTHING;
		}
		else if(point_tmp == 2 || point_tmp == 12){// Il perd.
			printf("\n%s perd %d$.",player->name,player->bet);
			player->bet =0;
			player->player_line = NOTHING;
		}
	}
	
	else if(player->player_line == DONTPASS){// Si le joueur a misי sur Don't Passline.
		if(point_tmp == 7){// Il perd.
			printf("\n%s perd %d$.",player->name,player->bet);
			player->bet =0;
			player->player_line = NOTHING;
		}
		else if(point_tmp == 2){// Il gagne.
			printf("\n%s remporte %d$.",player->name,(player->bet * 2));
			change_highscore((player->bet * 2),Highscore);// Si son gain est un nouveau record.
			
			player->money += player->bet * 2;
			player->bet = 0;
			player->player_line = NOTHING;
		}
		else if(point_tmp == 12){// Coup nul, rien ne bouge.
			printf("\nCoup nul pour %s, sa mise reste en place.",player->name);
		}
	}
	else if(point_tmp == 12 || point_tmp == 7 || point_tmp == 2) {// Si il n'as miser sur aucune ligne.
		printf("\n%s n'a mise sur aucune ligne.",player->name);
	}	
}

void second_phase(player_t *player, int point_tmp, int Point, int *Highscore){
	if(player->player_line == PASS){// Si le joueur a misי sur Passline.
		if(point_tmp == Point){// Il gagne.
			printf("\n%s remporte %d$.",player->name,(player->bet * 2));
			change_highscore((player->bet * 2),Highscore);// Si son gain est un nouveau record.
			
			player->money += player->bet * 2;
			player->bet = 0;
			player->player_line = NOTHING;
		}
		else if(point_tmp == 7){// Il perd.
			printf("\n%s perd %d$.",player->name,player->bet);
			player->bet =0;
			player->player_line = NOTHING;
		}
	}
	
	else if(player->player_line == DONTPASS){// Si le joueur a misי sur Don't Passline.
		if(point_tmp == 7){// Il gagne.
			printf("\n%s remporte %d$.",player->name,(player->bet * 2));
			change_highscore((player->bet * 2),Highscore);// Si son gain est un nouveau record.
			
			player->money += player->bet * 2;
			player->bet = 0;
			player->player_line = NOTHING;
		}
		else if(point_tmp == Point){// Il perd.
			printf("\n%s perd %d$.",player->name,player->bet);
			player->bet =0;
			player->player_line = NOTHING;
		}
	}
	else if(point_tmp == Point || point_tmp == 7) {//Si il n'a miser nul part.
		printf("\n%s n'a mise sur aucune ligne.",player->name);
	}
}

int dice(player_t *player){
	int de1, de2, sum;
	srand(time(NULL));
	// random entre 1 et 6.
	de1 = (rand() % 6) + 1;
	de2 = (rand() % 6) + 1;
	
	sum = de1 +de2;// Le score obtenu.
	
	fprintf(stdout,"\n%s a vous de lancer les des..\n",player->name);
	system("pause");
	fprintf(stdout,">>> Vous lancez les des et obtenez %d et %d ce qui fait donc %d <<<\n",de1,de2,sum);
	return sum;
}


