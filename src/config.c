#include "config.h"

char* _get_config_filename(const char* filename);
char* _get_theme_filename(const char* themename);

int config_load (rockon_config *config) {
	char *file;

	ecore_config_int_default ("/server/launch", 0);
	ecore_config_string_default ("/gui/theme", "gui.edj");

	if ((file = _get_config_filename ("rockon.cfg"))) {
		if (ecore_config_file_load (file) == ECORE_CONFIG_ERR_SUCC) {
			if (config) {
				config->launch_server = ecore_config_int_get ("/server/launch");
				config->theme = _get_theme_filename(ecore_config_string_get("/gui/theme"));

				free (file);
				return TRUE;
			}
		}
		free (file);
	}
	return FALSE;
}

int config_save (rockon_config *config) {
	char *file;
	char *theme;

	if (config) {
		theme = ecore_file_file_get((const char*)config->theme);

		ecore_config_int_set("/server/launch", config->launch_server);
		ecore_config_string_set ("/gui/theme", theme);

		if ((file = _get_config_filename ("rockon.cfg"))) {
			if (ecore_config_file_save (file) == ECORE_CONFIG_ERR_SUCC) {
				free (file);
				return TRUE;
			}
			free (file);
		}
	}
	return FALSE;
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
	}

	pg = ecore_path_group_new();
	ecore_path_group_add (pg, (const char*)home_path);
	ecore_path_group_add (pg, "/usr/local/share/rockon/themes");
	ecore_path_group_add (pg, "/usr/share/rockon/themes");

	theme = ecore_path_group_find(pg, themename);

	ecore_path_group_del(pg);
	printf("DEBUG: theme: %s\n", theme);
	return theme;
}
