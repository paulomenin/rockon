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

#include "config.h"
#include <string.h>
#include <Ecore_File.h>
#include <xmmsclient/xmmsclient.h>
#include <assert.h>

char* get_config_filename(const char* filename);
char* get_theme_filename(const char* themename);
enum lcfg_status config_visitor_load(const char *key, void *data, size_t len, void *user_data);

rockon_config* config_new() {
	rockon_config *config = NULL;

	config = (rockon_config*) malloc(sizeof(rockon_config));
	if (config == NULL)
		EINA_LOG_CRIT("Couldn't allocate memory for config.");

	if (config_load (config) == 0)
		WARN("Couldn't load config. Loaded default values.");

	return config;
}

void config_del(rockon_config *config) {
	assert(config);

	free(config->config_filename);
	free(config->theme);
	free(config->theme_name);
	if (config->lcfg_obj) lcfg_delete(config->lcfg_obj);
	free(config);
}

int config_load (rockon_config *config) {

	assert(config);

	/* default values */
	config->launch_server = 0;
//	config->reconnect = 0;
//	config->reconnect = 0;
	config->theme = NULL;

	config->theme_name = (char*) malloc(sizeof(char) * 12);
	if (config->theme_name == NULL) {
		EINA_LOG_CRIT("malloc theme_name failed");
		return 0;
	}
	strcpy(config->theme_name, "default.edj");

	if ((config->config_filename = get_config_filename ("rockon.conf"))) {
		config->lcfg_obj = lcfg_new(config->config_filename);
		if (lcfg_parse(config->lcfg_obj) != lcfg_status_ok) {
			config->theme = get_theme_filename(config->theme_name);
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
		fprintf(fd,"theme = \"%s\"\n", config->theme_name);
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

char* get_theme_filename(const char* themename) {
	Ecore_Path_Group *pg = NULL;
	char *home_path = NULL;
	char *conf_path = NULL;
	char *theme = NULL;
	int conf_path_len;

	conf_path = get_config_filename("");
	conf_path_len = strlen(conf_path);
	home_path = (char*) malloc (sizeof(char)*(conf_path_len + 7 ));
	if (home_path) {
		strcpy (home_path, conf_path);
		strcat (home_path, "themes");
	} else {
		return NULL;
	}

	pg = ecore_path_group_new();
	ecore_path_group_add (pg, "build/default/themes/default"); // FIXME remove this line
	ecore_path_group_add (pg, (const char*)home_path);
	ecore_path_group_add (pg, "/usr/share/rockon/themes");
	ecore_path_group_add (pg, "/usr/local/share/rockon/themes");

	theme = ecore_path_group_find(pg, themename);

	ecore_path_group_del(pg);

	free(home_path);
	free(conf_path);
	return theme;
}

enum lcfg_status config_visitor_load(const char *key, void *data, size_t len, void *user_data) {
	rockon_config *config = (rockon_config*) user_data;

	if (strcmp(key, "launch_server") == 0) {
		config->launch_server = atoi((const char*)data);
		DBG("LOADED: launch_server %d", config->launch_server);
	} else
	if (strcmp(key, "theme") == 0) {
		if (config->theme_name) free(config->theme_name);
		if (config->theme) free(config->theme);
		config->theme_name = strdup((char*)data);
		config->theme = get_theme_filename((char*)data);
		DBG("LOADED: theme %s", config->theme_name);
	}

	return lcfg_status_ok;
}
