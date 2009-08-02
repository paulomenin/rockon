#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>
#include <Ecore_Config.h>
#include <xmmsclient/xmmsclient.h>
#include "error.h"

typedef struct _rockon_config rockon_config;
struct _rockon_config
{
	int launch_server;
	char *theme;
};

int config_load(rockon_config *config);
int config_save(rockon_config *config);

#endif /* CONFIG_H */
