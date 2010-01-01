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

#include <Elementary.h>
#include "config.h"

/* log domains */
int config_log_dom = -1;
int conn_log_dom = -1;

Eina_Bool log_init(void);
void log_shutdown(void);

EAPI int elm_main (int argc, char** argv) {
	rockon_config *app_config;

	if (log_init() == EINA_FALSE) {
		EINA_LOG_ERR("Log domains init failed");
	}

	app_config = config_new();

	EINA_LOG_DBG("THEME NAME: %s", app_config->theme_name);
	EINA_LOG_DBG("THEME: %s", app_config->theme);
	EINA_LOG_DBG("MainLoop Start");
	elm_run();
	EINA_LOG_DBG("MainLoop End");

	config_save(app_config);

	config_del(app_config);
	log_shutdown();
	elm_shutdown();

	return 0;
}
ELM_MAIN()

Eina_Bool log_init(void) {
	if (config_log_dom < 0) {
		config_log_dom = eina_log_domain_register("rck_config", NULL);
	} else return EINA_FALSE;
	if (conn_log_dom < 0) {
		conn_log_dom = eina_log_domain_register("rck_conn", NULL);
	} else return EINA_FALSE;

	return EINA_TRUE;
}

void log_shutdown(void) {
	if (config_log_dom >= 0) {
		eina_log_domain_unregister(config_log_dom);
		config_log_dom = -1;
	}
	if (conn_log_dom >= 0) {
		eina_log_domain_unregister(conn_log_dom);
		conn_log_dom = -1;
	}
}
