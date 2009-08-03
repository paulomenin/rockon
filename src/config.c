#include "config.h"

char* _get_config_filename(const char* filename);

int config_load (rockon_config *config) {
	char *file;

	ecore_config_int_default ("/server/launch", 1);
	ecore_config_string_default ("/gui/theme", "../data/theme/default/gui.edj");

	if ((file = _get_config_filename ("rockon.cfg"))) {
		if (ecore_config_file_load (file) == ECORE_CONFIG_ERR_SUCC) {
			if (config) {
				config->launch_server = ecore_config_int_get ("/server/launch");
				config->theme = ecore_config_string_get ("/gui/theme");

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

	if (config) {
		ecore_config_int_set("/server/launch", config->launch_server);
		ecore_config_string_set ("/gui/theme", config->theme);

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
		config_file = (char*) malloc (sizeof(char) * (path_len + filename_len + 10));
		if (config_file) {
			strcpy (config_file, config_dir);
			strcat (config_file, "/clients/");
			strcat (config_file, filename);
			return config_file;
		}
	}
	return NULL;
}
