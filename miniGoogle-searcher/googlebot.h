#ifndef _GOOGLEBOT_H_
#define _GOOGLEBOT_H_

#include "utils.h"
#include "site.h"
#include "command.h"

enum{
	SUCCESS,
	FAILURE,
	ABORT
};

typedef struct{
	SITE *googlebot_head;
	short int site_num;
} GOOGLEBOT_STRUCT;

GOOGLEBOT_STRUCT *command_gbotSetup();
boolean command_gbotLoadDatabase(GOOGLEBOT_STRUCT *, int *);
boolean command_gbotRemoveSite(GOOGLEBOT_STRUCT *);
boolean command_gbotLoadSite(GOOGLEBOT_STRUCT *, FILE *, int *, int *);
boolean command_gbotAddSite(GOOGLEBOT_STRUCT *);
boolean command_gbotRefreshRelevance(GOOGLEBOT_STRUCT *);
boolean command_gbotAddKeyword(GOOGLEBOT_STRUCT *);
boolean command_gbotRemoveKeyword(GOOGLEBOT_STRUCT *);
boolean command_googlebotDismiss(GOOGLEBOT_STRUCT **);
boolean command_gbotDumpDatabase(GOOGLEBOT_STRUCT *);
boolean command_gbotRefreshDatabase(GOOGLEBOT_STRUCT *);
boolean command_gbotAbortSession(GOOGLEBOT_STRUCT *, COMMAND_STRUCT *);
boolean command_gbotSearch(GOOGLEBOT_STRUCT *, char *);

SITE *function_SiteGet(GOOGLEBOT_STRUCT *, short int);
boolean function_SiteAutoinsertion(GOOGLEBOT_STRUCT *, SITE *);

#endif