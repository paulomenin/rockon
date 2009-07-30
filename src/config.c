#include "config.h"

int config_load (Rockon_config *config) {
    ecore_config_int_default ("/server/launch", 1);
    ecore_config_string_default ("/gui/theme", "data/theme/default/gui.edj");

    if (ecore_config_load () != ECORE_CONFIG_ERR_SUCC)
        print_error ("Could not load config from ~/.e/apps/exmms2a/config.eet. Loaded default values.", ERR_WARNING);

    if (config) {
        config->launch_server = ecore_config_int_get ("/server/launch");
        config->theme = ecore_config_string_get ("/gui/theme");
    }
    else return 0;

    return 1;
}

int config_save (Rockon_config *config) {
    if (config) {   
        ecore_config_int_set("/server/launch", config->launch_server);
        ecore_config_string_set ("/gui/theme", config->theme);
        
        if (ecore_config_save() != ECORE_CONFIG_ERR_SUCC) {
            print_error ("Could not save config to ~/.e/apps/exmms2a/config.eet", ERR_NORMAL);
            return 0;
        }
    }
	return 1;
}

