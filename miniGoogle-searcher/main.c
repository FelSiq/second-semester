#include <stdlib.h>
#include <stdio.h>
#include "googlebot.h"
#include <string.h>

int main(int argc, char *argv[]){
	GOOGLEBOT_STRUCT *googlebot = command_gbotSetup();
	COMMAND_STRUCT *command_struct = NULL;
	int aux_counter = EMPTY;
	short int leak_signal = EMPTY, change_signal = EMPTY, aux_size = EMPTY;
	char *aux_char = NULL;

	if (argc >= 2){
		command_gbotLoadDatabase(googlebot, &aux_counter);
		aux_char = get_keyword(argv, argc);
		if (aux_char != NULL){
			command_gbotSearch(googlebot, aux_char);
			command_googlebotDismiss(&googlebot);
		}
	}else{
		command_struct = command_getList();
		if (googlebot != NULL){
			printf("STATUS: Googlebot awakened.\n");
			if (command_gbotLoadDatabase(googlebot, &aux_counter)){
				printf("STATUS: Successfully loaded googlebot database, total of %d sites loaded.\n", aux_counter);
				aux_counter = EMPTY;
				if (command_struct != NULL){
					printf("STATUS: Loaded command list, total of %d commands.\n", command_count(command_struct));
					while(command_value(command_struct) != CASE_EXIT){
						printf("\nPlease insert a valid command: (Type 'commands' for list)\n");
						command_get(command_struct);
						switch(command_value(command_struct)){
							case CASE_EXIT:
								printf("Googlebot is preparing to shut down...\n");
								break;
							case CASE_REMOVE_SITE:
								if(command_gbotRemoveSite(googlebot)){
									change_signal = UNITY;
									printf("Successfully removed site.\n");
								}
								else
									printf("Can't remove site.\n");
								break;
							case CASE_ADD_SITE:
								if(command_gbotAddSite(googlebot)){
									change_signal = UNITY;
									printf("Successfully added new site.\n");
								}
								else
									printf("Can't add site.\n");
								break;
							case CASE_REFRESH_RELEVANCE:
								if (command_gbotRefreshRelevance(googlebot)){
									change_signal = UNITY;
									printf("Site relevance updated.\n");
								}
								else
									printf("Can't update site relevance.\n");
								break;
							case CASE_ADD_KEYWORD:
								if(command_gbotAddKeyword(googlebot)){
									change_signal = UNITY;
									printf("Added new keyword to selected site.\n");
								}
								else
									printf("Can't add new keyword to selected site.\n");
								break;
							case CASE_REMOVE_KEYWORD:
								if(command_gbotRemoveKeyword(googlebot)){
									change_signal = UNITY;
									printf("Removed selected keyword of selected site.\n");
								}
								else
									printf("Can't remove selected keyword of selected site.\n");
								break;
							case CASE_DUMP:
								if (command_gbotDumpDatabase(googlebot))
									printf("Successfully created output database file.\n");
								else
									printf("Unable to create output database file.\n");
								break;
							case CASE_ABORT_SESSION:
								if (!command_gbotAbortSession(googlebot, command_struct))
									printf("Action canceled.\n");
								break;
							case CASE_REFRESH_DATABASE:
								if(command_gbotRefreshDatabase(googlebot)){
									change_signal = EMPTY;
									printf("STATUS: Successfully refreshed database.\n");
								}
								else
									printf("ERROR: Unable to refresh database.\n");
								break;
							case CASE_SEARCH:
								command_gbotSearch(googlebot, NULL);
								break;
							case CASE_COMMANDS:
								command_list(command_struct);
								break;
							default:
								printf("STATUS: Invalid command.\n");
								break;
						}

						if (command_value(command_struct) != CASE_EXIT)
							command_ForceInvalid(command_struct);
					}
					if(change_signal == UNITY){
						printf("STATUS: Refreshing database due to recent changes...\n");
						if(command_gbotRefreshDatabase(googlebot))
							printf("STATUS: Successfully refreshed database.\n");
						else
							printf("ERROR: Unable to refresh database.\n");
					}

					if (command_destroyList(&command_struct))
						printf("STATUS: Successfully deallocated command list...\n");
					else{
						printf("STATUS: Failed to deallocate command list... something went wrong.\n");
						leak_signal = UNITY;
					}
				}
			} else printf("STATUS: Can't load googlebot database.\n");
			if(command_googlebotDismiss(&googlebot))
				printf("STATUS: Googlebot successfully dismissed.\n");
			else{
				printf("STATUS: Can't dismiss Googlebot, something went wrong.\n");
				leak_signal = UNITY;
			}
		}
		if (leak_signal == UNITY){
			printf("WARNING! POSSIBLE MEMORY LEAKS!\n");
			return FAILURE;
		}
		printf("Program finished.\n");
	}
	return SUCCESS;
}