#ifndef CONFIG_H
#define CONFIG_H

#include <Ecore_Config.h>
#include "error.h"

typedef struct _Rockon_config Rockon_config;
struct _Rockon_config
{
	int launch_server;
	char *theme;
};

int config_load(Rockon_config *config);
int config_save(Rockon_config *config);

#endif /* CONFIG_H */
