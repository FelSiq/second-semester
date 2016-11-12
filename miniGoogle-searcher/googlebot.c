#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "googlebot.h"
#include <time.h>

GOOGLEBOT_STRUCT *command_gbotSetup(){
	GOOGLEBOT_STRUCT *googlebot = (GOOGLEBOT_STRUCT *) malloc(sizeof(GOOGLEBOT_STRUCT));
	if (googlebot != NULL){
		googlebot->site_num = EMPTY;
		googlebot->googlebot_head = (SITE *) malloc(sizeof(SITE));
		if (googlebot->googlebot_head != NULL){
			googlebot->googlebot_head->site_next = googlebot->googlebot_head;
			googlebot->googlebot_head->site_prev = googlebot->googlebot_head;
			googlebot->googlebot_head->site_code = INVALID;
			return googlebot;
		}
	}
	return NULL;
}

boolean command_gbotLoadDatabase(GOOGLEBOT_STRUCT *googlebot, int *success_counter){
	int error_counter = EMPTY;
	if (googlebot != NULL){
		FILE *fp = fopen("Database.gbot", "r");
		if (fp != NULL){
			while(!feof(fp))
				command_gbotLoadSite(googlebot, fp, &error_counter, success_counter);
			fclose(fp);
			printf("STATUS: Process of data get endded, total of %d errors occurred.\n", error_counter);
			return TRUE;
		}
	}
	return FALSE;
}

boolean command_gbotLoadSite(GOOGLEBOT_STRUCT *googlebot, FILE *fp, int *error_counter, int *success_counter){
	SITE *new_site = NULL; 
	char *aux = NULL, *aux_adress = NULL, *aux_token = NULL, *destiny = NULL;
	int counter = EMPTY, aux_size = EMPTY;
	if (googlebot != NULL && fp != NULL){
		new_site = function_SiteAutocomplete();
		aux = (char *) malloc(sizeof(char) * (AUX_MAX_SIZE + UNITY));
		if (new_site != NULL && aux != NULL){
			aux_adress = aux;
			fgets(aux, AUX_MAX_SIZE, fp);
			aux_token = strtok(aux, ",\n");

			if (aux_token != NULL && strcmp(aux_token, "END") == EMPTY){
				function_SiteDestroy(new_site);
				free(aux_adress);
				return TRUE;
			}

			new_site->site_code = atoi(aux_token);
			if(function_SiteGet(googlebot, new_site->site_code) == NULL){
				while(aux_token != NULL && new_site->site_kwnum < MAX_KEYWORD_NUMBER){
					aux_token = strtok(NULL, ",\n");
					if (aux_token != NULL){
						aux_size = strlen(aux_token);
						switch(counter++){
							case 0:
								new_site->site_name = (char *) malloc(sizeof(char) * (aux_size + UNITY));
								destiny = new_site->site_name;
								break;
							case 1:
								new_site->site_relevance = atoi(aux_token);
								destiny = NULL;
								break;
							case 2:
								new_site->site_homelink = (char *) malloc(sizeof(char) * (aux_size + UNITY));
								destiny = new_site->site_homelink;
								break;
							default:
								new_site->site_keywords = (char **) realloc(new_site->site_keywords, sizeof(char *) * (new_site->site_kwnum + UNITY));
								new_site->site_keywords[new_site->site_kwnum] = (char *) malloc(sizeof(char) * (aux_size + UNITY));
								destiny = new_site->site_keywords[new_site->site_kwnum];
								new_site->site_kwnum++;
								break; 
						}				
						if (destiny != NULL){
							memcpy(destiny, aux_token, aux_size);
							destiny[aux_size] = '\0';

							if (destiny[aux_size - UNITY] == ENTER || destiny[aux_size - UNITY] == SPACE_BAR || destiny[aux_size - UNITY] == CARRIAGE_RETURN){
								destiny = (char *) realloc(destiny, sizeof(char) * (aux_size));
								destiny[aux_size - UNITY] = '\0';
								aux_size--;
							} 
							destiny = NULL;
						}
					} else break;
				}				
				function_SiteAutoinsertion(googlebot, new_site);
				free(aux_adress);
				(*success_counter)++;
				return TRUE;
			}
			function_SiteDestroy(new_site);
			free(aux_adress);
		}
	}
	(*error_counter)++;
	return FALSE;
}

boolean command_gbotAddSite(GOOGLEBOT_STRUCT *googlebot){
	SITE *new_site = NULL;
	short int steps_counter = UNITY;
	char *string = NULL;
	if(googlebot != NULL){
		new_site = function_SiteAutocomplete();
		printf("SITE MANAGEMENT - Please type the new site information:\n");
		do{
			printf("Step [%d/5] - Type a value to be the SITE CODE:\n", steps_counter);
			scanf("%hd", &new_site->site_code);
			getchar();
			if(function_SiteGet(googlebot, new_site->site_code) != NULL){
				printf("ERROR: Site code alredy in use.\n");
				new_site->site_code = INVALID;
			}
			if (new_site->site_code <= EMPTY){
				printf("ERROR: Invalid site code.\n");
				new_site->site_code = INVALID;
			}
		} while (new_site->site_code == INVALID);

		steps_counter++;
		do{
			printf("Step [%d/5] - Give a name to the new site (max %d characters):\n", steps_counter, MAX_SITENAME_LENGHT);
			new_site->site_name = get_string(MAX_SITENAME_LENGHT);
		} while (new_site->site_name == NULL);

		steps_counter++;
		do{
			printf("Step [%d/5] - Give a homelink to the new site (must be smaller than %d characters)\n", steps_counter, MAX_LINK_LENGHT);
			new_site->site_homelink = get_string(MAX_LINK_LENGHT);
		} while (new_site->site_homelink == NULL);

		steps_counter++;
		do{
			printf("Step [%d/5] - Type a value to be the SITE RELEVANCE (0 to %d):\n", steps_counter, MAX_RELEVANCE);
			scanf("%hd", &new_site->site_relevance);
			getchar();
			if(new_site->site_relevance < EMPTY || new_site->site_relevance > MAX_RELEVANCE){
				printf("ERROR: Site relevance must be between %d and %d.\n", EMPTY, MAX_RELEVANCE);
				new_site->site_relevance = INVALID;
			}
		} while (new_site->site_relevance == INVALID);

		steps_counter++;
		printf("Step [%d/5] - Now it's time to give some keywords to the site, in a max of %d.\n", steps_counter, MAX_KEYWORD_NUMBER);
		do{
			printf("You have more %d keywords to set to the site. Type 'END' do finish.\n", MAX_KEYWORD_NUMBER - new_site->site_kwnum);
			string = get_string(AUX_MAX_SIZE);
			if (string != NULL && strlen(string) > UNITY){
				if (strcmp(string, "END") != EMPTY){
					printf("Attached '%s' as a new keyword on site.\n", string);
					new_site->site_keywords = (char **) realloc(new_site->site_keywords, sizeof(char *) * (new_site->site_kwnum + UNITY));
					new_site->site_keywords[new_site->site_kwnum++] = string;
				}else {
					free(string);
					if (new_site->site_kwnum == EMPTY)
						printf("ERROR: The site must have at least one keyword attached to it.\n");
				}
			} else {
				if (string != NULL)
					free(string);
				printf("Invalid keyword. Try again.\n");
			}
		} while (new_site->site_kwnum == EMPTY || (strcmp(string, "END") != EMPTY && new_site->site_kwnum < MAX_KEYWORD_NUMBER));			

		printf("Process of site registering is complete...\n");
		if(function_SiteAutoinsertion(googlebot, new_site)){
			printf("Succesfully added it to the list. Please 'refresh database' to permanent add it to database.\n");
			return TRUE;
		}
		else
			printf("Failed to add new site to the list.\n");
	}
	return FALSE;
}

boolean command_gbotRemoveSite(GOOGLEBOT_STRUCT *googlebot){
	SITE *aux = NULL;
	boolean CONFIRMATION = EMPTY;
	short int site_number = EMPTY;
	if (googlebot != NULL){
		printf("SITE MANAGEMENT - Type the site code to remove: (WARNING: that action will be permanent!)\n");
		scanf("%hd", &site_number);
		getchar();
		aux = function_SiteGet(googlebot, site_number);
		if (aux != NULL){
			printf("Site founded in database, please verify its data:\nNAME: %s\t\tHome URL:%s\n", aux->site_name, aux->site_homelink);
			if (get_confirmation()){
				aux->site_prev->site_next = aux->site_next;
				aux->site_next->site_prev = aux->site_prev;
				function_SiteDestroy(aux);
				aux = NULL;
				return TRUE;
			}
		} else printf("Requested site code not found in database.\n");
	}
	return FALSE;
}

boolean command_gbotRefreshRelevance(GOOGLEBOT_STRUCT *googlebot){
	SITE *aux = NULL;
	short int site_number = EMPTY, site_newrelevance;
	if (googlebot != NULL){
		printf("SITE MANAGEMENT - Type the site code to refresh its relevance:\n");
		scanf("%hd", &site_number);
		getchar();
		aux = function_SiteGet(googlebot, site_number);
		if (aux != NULL){
			printf("Site founded in database.\nSITE NAME: %s\tCURRENT RELEVANCE: %d\nType a valid new relevance for site [0 to %d]:\n", 
				aux->site_name, aux->site_relevance, MAX_RELEVANCE);
			do{
				scanf("%hd", &site_newrelevance);
				getchar();
				if (site_newrelevance < EMPTY && site_newrelevance > MAX_RELEVANCE)
					printf("ERROR: Invalid number, try again:\n");
			} while (site_newrelevance < EMPTY && site_newrelevance > MAX_RELEVANCE);
			aux->site_relevance = site_newrelevance;
			return TRUE;
		} else printf("Requested site code not found in database.\n");
	}
	return FALSE;
}

boolean command_gbotAddKeyword(GOOGLEBOT_STRUCT *googlebot){
	SITE *aux = NULL;
	short int site_number = EMPTY, aux_size = EMPTY;
	char *string = NULL;
	if (googlebot != NULL){
		printf("SITE MANAGEMENT - Type the site code to add a new keyword:\n");
		scanf("%hd", &site_number);
		getchar();
		aux = function_SiteGet(googlebot, site_number);
		if (aux != NULL){
			printf("Site founded in database:\nName: %s\t\tHome URL: %s\t\tTotal keywords: [%d/%d]\n", aux->site_name, 
				aux->site_homelink, aux->site_kwnum, MAX_KEYWORD_NUMBER);
			if (aux->site_kwnum < MAX_KEYWORD_NUMBER){
				printf("Type a new keyword to this site:\n");
				string = get_string(INVALID);
				if (string != NULL && strcmp(string, "END") != EMPTY){
					aux_size = strlen(string);
					aux->site_kwnum++;
					aux->site_keywords = (char **) realloc(aux->site_keywords, sizeof(char *) * aux->site_kwnum);
					aux->site_keywords[aux->site_kwnum - UNITY] = (char *) malloc(sizeof(char) * (aux_size + UNITY));
					memcpy(aux->site_keywords[aux->site_kwnum - UNITY], string, aux_size);
					aux->site_keywords[aux->site_kwnum - UNITY][aux_size] = '\0';
					free(string);
					return TRUE;
				} else printf("Invalid new keyword.\n");
			} else printf("Requested site already has maximum keywords attached to it, remove one first.\n");
		} else printf("Requested site code not found in database.\n");
	}
	return FALSE;
}

boolean command_gbotRemoveKeyword(GOOGLEBOT_STRUCT *googlebot){
	SITE *aux = NULL;
	short int site_number = EMPTY, index;
	char *string = NULL;
	if (googlebot != NULL){
		printf("SITE MANAGEMENT - Type the site code to modify its keywords:\n");
		scanf("%hd", &site_number);
		getchar();
		aux = function_SiteGet(googlebot, site_number);
		if (aux != NULL){
			printf("Site founded in database:\nName: %s\t\tKeywords number: [%d/%d]\nCurrent keywords attached to this site are:\n", aux->site_name, aux->site_kwnum, MAX_KEYWORD_NUMBER);
			for(int i = EMPTY; i < aux->site_kwnum; i++)
				printf("%d. %s | ", i + UNITY, aux->site_keywords[i]);
			printf("\nType the keyword to delete:\n");
			string = get_string(INVALID);
			if (string != NULL){
				for(index = EMPTY; index < aux->site_kwnum; index++)
					if (strcmp(string, aux->site_keywords[index]) == EMPTY){
						free(aux->site_keywords[index]);
						matrix_shift(aux->site_keywords, index, aux->site_kwnum);
						aux->site_kwnum--;
						free(string);
						return TRUE;
					}
				free(string);
				printf("Requested keyword not found.\n");
			}
		} else printf("Requested site code not found in database.\n");
	}
	return FALSE;
}

boolean command_googlebotDismiss(GOOGLEBOT_STRUCT **googlebot){
	SITE *aux = NULL;
	if (googlebot != NULL && *googlebot != NULL){
		aux = (*googlebot)->googlebot_head->site_next;
		while(aux != (*googlebot)->googlebot_head){
			(*googlebot)->googlebot_head->site_next = aux->site_next;
			aux->site_next->site_prev = (*googlebot)->googlebot_head;
			function_SiteDestroy(aux);
			aux = (*googlebot)->googlebot_head->site_next;
		}
		aux = NULL;
		function_SiteDestroy((*googlebot)->googlebot_head);
		free((*googlebot));
		(*googlebot) = NULL;
		return TRUE;
	}
	return FALSE;
}

boolean command_gbotDumpDatabase(GOOGLEBOT_STRUCT *googlebot){
	SITE *aux_site = NULL;
	char *aux_string = NULL;
	int counter = EMPTY;
	if (googlebot != NULL){
		FILE *fp = fopen("GBOT_DATABASE.out", "a+");

		time_t t = time(NULL);
		struct tm *tm = localtime(&t);
		aux_string = (char *) malloc(sizeof(char) * AUX_MAX_SIZE);
		strftime(aux_string, AUX_MAX_SIZE, "%c", tm);

		if (fp != NULL && aux_string != NULL){
			aux_site = googlebot->googlebot_head->site_next;
			fprintf(fp, "===== GBOT DATABASE =====\nGenerated at \t%s.\n\n", aux_string);
			while(aux_site != googlebot->googlebot_head){
				fprintf(fp, "%d. SITE CODE: %04d\n", ++counter, aux_site->site_code);
				fprintf(fp, "Name: %s\nHome URL: %s\nRelevance: %d\nKeywords [%d]:", aux_site->site_name, 
					aux_site->site_homelink, aux_site->site_relevance, aux_site->site_kwnum);
				for(int i = EMPTY; i < aux_site->site_kwnum; i++)
					fprintf(fp, " %s", aux_site->site_keywords[i]);
				aux_site = aux_site->site_next;
				fprintf(fp, "\n\n");
			}
			fprintf(fp, "===== END =====\n");
			fclose(fp);
			free(aux_string);
			return TRUE;
		}
	}
	return FALSE;
}

boolean command_gbotRefreshDatabase(GOOGLEBOT_STRUCT *googlebot){
	SITE *aux = NULL;
	if (googlebot != NULL){
		FILE *fp = fopen("Database.gbot", "w");
		if (fp != NULL){
			aux = googlebot->googlebot_head->site_next;
			while(aux != googlebot->googlebot_head){
				fprintf(fp, "%04d,%s,%d,%s", aux->site_code, aux->site_name, aux->site_relevance, aux->site_homelink);
				for(int i = EMPTY; i < aux->site_kwnum; i++)
					fprintf(fp, ",%s", aux->site_keywords[i]);
				aux = aux->site_next;
				fprintf(fp, "\n");
			}
			fprintf(fp, "END");
			fclose(fp);
			return TRUE;
		}
	}
	return FALSE;
}

boolean command_gbotAbortSession(GOOGLEBOT_STRUCT *googlebot, COMMAND_STRUCT *command_struct){
	if (googlebot != NULL && command_struct != NULL){
		printf("WARNING! Abort this session? All changes made recently will be permanently lost!\n(Unless you have made a 'refresh database' in this session)\n");
		if (get_confirmation()){
			printf("STATUS: Starting deallocation of memory...\n");
			command_destroyList(&command_struct);
			command_googlebotDismiss(&googlebot);
			printf("Program aborted.\n");
			exit(ABORT);
		}
	}
	return FALSE;
}

boolean command_gbotSearch(GOOGLEBOT_STRUCT *googlebot, char *quick_search){
	SITE **site_list = NULL, *site_aux = NULL;
	char *string = NULL;
	short int counter = EMPTY;
	if (googlebot != NULL){
		if (quick_search == NULL){
			printf("Tell us what you want, and we find everywhere, everytime:\n");
			string = get_string(INVALID);
		} else string = quick_search;

		if (string != NULL && strlen(string) > UNITY){
			site_aux = googlebot->googlebot_head->site_next;
			while(site_aux != googlebot->googlebot_head){
				for(int i = EMPTY; i < site_aux->site_kwnum; i++)
					if (strcmp(string, site_aux->site_keywords[i]) == EMPTY){
						site_list = (SITE **) realloc(site_list, sizeof(SITE *) * (counter + UNITY));
						site_list[counter++] = site_aux;
						break;
					}
				site_aux = site_aux->site_next;
			}

			if (counter > UNITY)
				merge_sort(site_list, EMPTY, counter - UNITY);

			if (counter > EMPTY){
				printf("_______________________________________\nSearch complete, showing %d results founded.", counter);
				for(int i = EMPTY; i < counter; i++)
					printf("\n\n%d. %s\n%s", i + UNITY, site_list[i]->site_name, site_list[i]->site_homelink);
				printf("\n_______________________________________\n");
			} else printf("Search complete, but no results founded.\n");
			if (site_list != NULL)
				free(site_list);
			free(string);
			return TRUE;
		} else {
			if (string != NULL)
				free(string);
			printf("Invalid search, try again.\n");
		}
	}
	return FALSE;
}