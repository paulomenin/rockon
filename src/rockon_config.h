/* This file is part of Rockon.
 * 
 * Rockon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * Rockon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Rockon.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ROCKON_CONFIG_H
#define ROCKON_CONFIG_H

#include "lcfg_static.h"

typedef struct {
	char *config_filename;
	struct lcfg *lcfg_obj;

	int launch_server;
	int terminate_server;
	int auto_reconnect;
	int reconnect_interval;
	char *ipc_path;
	char *edj_data_path;
} rockon_config;

rockon_config* config_new();
void config_del(rockon_config *config);
int config_load(rockon_config *config);
int config_save(rockon_config *config);

#endif /* ROCKON_CONFIG_H */
