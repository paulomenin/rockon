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

char* _get_config_filename(const char* filename);
char* _get_theme_filename(const char* themename);
enum lcfg_status _config_visitor_load(const char *key, void *data, size_t len, void *user_data);
enum lcfg_status _config_visitor_save(const char *key, void *data, size_t len, void *user_data);

int config_load (rockon_config *config) {
	char *file;

	/* default values */
	config->launch_server = 0;
	config->theme = (char*) malloc(sizeof(char) * 8);
	if (!config->theme) return 0;
	strcpy(config->theme, "gui.edj");


	if (file = _get_config_filename ("rockon.conf")) {
		config->lcfg_obj = lcfg_new(file);
		if (lcfg_parse(config->lcfg_obj) != lcfg_status_ok) {
			free(file);
			return 0;
		}
		lcfg_accept(config->lcfg_obj, _config_visitor_load, config);

		config->theme = _get_theme_filename(config->theme);

		free (file);
		return 1;
	}
	return 0;
}

void config_free(rockon_config *config) {
	if (config->lcfg_obj) lcfg_delete(config->lcfg_obj);
	if (config->theme)    free(config->theme);
}

int config_save (rockon_config *config) {
	FILE *fd;
	char *file;
	char *theme;

	if (config) {
		if ((file = _get_config_filename ("rockon.conf"))) {
			fd = fopen(file, "w");
			if (fd) {
				lcfg_accept(config->lcfg_obj, _config_visitor_save, fd);
				fclose(fd);
				free(file);
				return 1;
			}
			free(file);
			return 0;
		}
	}
	return 0;
}

char* _get_config_filename(const char* filename) {
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

char* _get_theme_filename(const char* themename) {
	Ecore_Path_Group *pg = NULL;
	char *home_path = NULL;
	char *conf_path = NULL;
	char *theme = NULL;
	int conf_path_len;
	
	conf_path = _get_config_filename("");
	conf_path_len = strlen(conf_path);
	home_path = (char*) malloc (sizeof(char)*(conf_path_len + 7 ));
	if (home_path) {
		strcpy (home_path, conf_path);
		strcat (home_path, "themes");
	} else {
		return NULL;
	}

	pg = ecore_path_group_new();
	ecore_path_group_add (pg, "data/theme/default"); // FIXME remove this line
	ecore_path_group_add (pg, (const char*)home_path);
	ecore_path_group_add (pg, "/usr/local/share/rockon/themes");
	ecore_path_group_add (pg, "/usr/share/rockon/themes");

	theme = ecore_path_group_find(pg, themename);

	ecore_path_group_del(pg);
	printf("DEBUG: theme: %s\n", theme);
	return theme;
}

enum lcfg_status _config_visitor_load(const char *key, void *data, size_t len, void *user_data) {
	rockon_config *config = (rockon_config*) user_data;

	if (strcmp(key, "launch_server") == 0) {
		config->launch_server = atoi((const char*)data);
	} else
	if (strcmp(key, "theme") == 0) {
		config->theme = (char*)data;
	}

	return lcfg_status_ok;
}

enum lcfg_status _config_visitor_save(const char *key, void *data, size_t len, void *user_data) {
	FILE *fd = (FILE*) user_data;
	const char* theme;

	if (strcmp(key, "theme") == 0) {
		theme = ecore_file_file_get((char*)data);
		fprintf(fd, "theme = \"%s\"\n", theme);
	} else {
		fprintf(fd, "%s = \"%s\"\n", key, (char*)data);
	}

	return lcfg_status_ok;
}
