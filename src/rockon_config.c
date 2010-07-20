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

#include <assert.h>
#include <string.h>
#include <Ecore_File.h>
#include <xmmsclient/xmmsclient.h>
#include "rockon_config.h"

#define DBG(...) EINA_LOG_DOM_DBG(config_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(config_log_dom, __VA_ARGS__)

extern int config_log_dom;

char* get_config_filename(const char* filename);
char* get_theme_filename(rockon_config* config);
enum lcfg_status config_visitor_load(const char *key, void *data, size_t len, void *user_data);

rockon_config* config_new() {
	rockon_config *config = NULL;
	char *home_theme_path;

	config = (rockon_config*) malloc(sizeof(rockon_config));
	if (config == NULL)
		EINA_LOG_CRIT("Couldn't allocate memory for config.");
	config->theme_path_list = NULL;

	home_theme_path = get_config_filename("themes");

	config->theme_path_list = eina_list_append(config->theme_path_list, strdup("/usr/share/rockon/themes"));
	config->theme_path_list = eina_list_append(config->theme_path_list, strdup("/usr/local/share/rockon/themes"));
	config->theme_path_list = eina_list_append(config->theme_path_list, strdup(home_theme_path));

	if (config_load(config) == 0)
		INFO("Couldn't load config. Loaded default values.");
	
	config->edj_data_path = get_theme_filename(config);

	free(home_theme_path);
	return config;
}

void config_del(rockon_config *config) {
	char *data;
	assert(config);

	free(config->config_filename);
	free(config->ipc_path);
	if (config->lcfg_obj) lcfg_delete(config->lcfg_obj);
	if (config->edj_data_path) free(config->edj_data_path);
	if (config->theme) free(config->theme);

	EINA_LIST_FREE(config->theme_path_list, data) {
		free(data);
	}

	free(config);
	config = NULL;
}

int config_load (rockon_config *config) {
	char *username;
	int ipc_path_lenght;

	assert(config);

	/* default values */
	config->launch_server = 0;
	config->auto_reconnect = 0;
	config->reconnect_interval = 3;
	config->theme = strdup("default.edj");

	username = getenv("USER");
	ipc_path_lenght = strlen(username) + 22;
	config->ipc_path = (char*) malloc(sizeof(char) * ipc_path_lenght);
	if (config->ipc_path == NULL) {
		EINA_LOG_CRIT("malloc ipc_path failed");
		return 0;
	}
	snprintf(config->ipc_path, ipc_path_lenght, "unix:///tmp/xmms-ipc-%s", username);

	if ((config->config_filename = get_config_filename ("rockon.conf"))) {
		config->lcfg_obj = lcfg_new(config->config_filename);
		if (lcfg_parse(config->lcfg_obj) != lcfg_status_ok) {
			return 0;
		}
		lcfg_accept(config->lcfg_obj, config_visitor_load, config);
		return 1;
	}
	return 0;
}

int config_save (rockon_config *config) {
	FILE *fd;
	char *dir;
	int count = 0;
	Eina_List *l;
	char *path;

	if (config == NULL)  return 0;
	if (config->config_filename == NULL)  return 0;

	dir = ecore_file_dir_get(config->config_filename);
	if (! ecore_file_is_dir(dir)) {
		if (! ecore_file_mkpath(dir)) {
			free(dir);
			return 0;
		}
	}
	free(dir);

	fd = fopen(config->config_filename, "w");
	if (fd) {
		fprintf(fd,"launch_server = \"%d\"\n", config->launch_server);
		fprintf(fd,"terminate_server = \"%d\"\n", config->terminate_server);
		fprintf(fd,"auto_reconnect = \"%d\"\n", config->auto_reconnect);
		fprintf(fd,"reconnect_interval = \"%d\"\n", config->reconnect_interval);
		fprintf(fd,"ipc_path = \"%s\"\n", config->ipc_path);
		fprintf(fd,"theme = \"%s\"\n", config->theme);

		EINA_LIST_FOREACH(config->theme_path_list, l, path) {
			if (count > 2) {
				fprintf(fd,"additional_theme_path = \"%s\"\n", path);
			}
			count++;
		}

		fclose(fd);
		return 1;
	}
	return 0;
}

char* get_config_filename(const char* filename) {
	char config_dir[XMMS_PATH_MAX] = {'\0'};
	char *config_file = NULL;
	int path_len, filename_len;

	if (! filename)
		return NULL;

	if (xmmsc_userconfdir_get (config_dir, XMMS_PATH_MAX)) {
		path_len = strlen (config_dir);
		filename_len = strlen (filename);
		config_file = (char*) malloc (sizeof(char) * (path_len + filename_len + 17));
		if (config_file) {
			strcpy (config_file, config_dir);
			strcat (config_file, "/clients/rockon/");
			strcat (config_file, filename);
			return config_file;
		}
	}
	return NULL;
}

char* get_theme_filename(rockon_config *config) {
	Eina_List *l;
	char *path;
	char *theme = NULL;
	int filename_len, dir_len;

	assert(config);

	filename_len = strlen(config->theme);
	EINA_LIST_FOREACH(config->theme_path_list, l, path) {
		free(theme);
		dir_len = strlen(path);
		theme = (char*) malloc(sizeof(char*)*(dir_len+filename_len+2));
		snprintf(theme,(dir_len+filename_len+2),"%s/%s", path, config->theme);
		DBG("possible theme: %s", theme);
		if ((ecore_file_exists(theme)) && (!ecore_file_is_dir(theme))) {
			return theme;
		}
	}
	free(theme);
	return NULL;
}

enum lcfg_status config_visitor_load(const char *key, void *data, size_t len, void *user_data) {
	rockon_config *config = (rockon_config*) user_data;

	if (strcmp(key, "launch_server") == 0) {
		config->launch_server = atoi((const char*)data);
		DBG("LOADED: launch_server %d", config->launch_server);
	} else if (strcmp(key, "terminate_server") == 0) {
		config->terminate_server = atoi((const char*)data);
		DBG("LOADED: terminate_server %d", config->terminate_server);
	} else if (strcmp(key, "auto_reconnect") == 0) {
		config->auto_reconnect = atoi((const char*)data);
		DBG("LOADED: auto_reconnect %d", config->auto_reconnect);
	} else if (strcmp(key, "reconnect_interval") == 0) {
		config->reconnect_interval = atoi((const char*)data);
		DBG("LOADED: reconnect_interval %d", config->reconnect_interval);
	} else if (strcmp(key, "ipc_path") == 0) {
		if (config->ipc_path) free(config->ipc_path);
		config->ipc_path = strdup((const char*)data);
		DBG("LOADED: ipc_path %s", config->ipc_path);
	} else if (strcmp(key, "theme") == 0) {
		if (config->theme) free(config->theme);
		config->theme = strdup((const char*)data);
		DBG("LOADED: theme %s", config->theme);
	} else if (strcmp(key, "additional_theme_path") == 0) {
		config->theme_path_list = eina_list_append(config->theme_path_list, strdup((const char*)data));
		DBG("LOADED: additional_theme_path %s", (const char*)data);
	}

	return lcfg_status_ok;
}
