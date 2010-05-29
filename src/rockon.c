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

#include <Ecore.h>
#include <Eina.h>
#include "rockon_data.h"

/* eina_log domains */
int config_log_dom = -1;

Eina_Bool log_init(void);
void log_shutdown(void);

int main (int argc, char** argv) {
	rockon_data *rdata;

	ecore_init();

	if (log_init() == EINA_FALSE) {
		EINA_LOG_ERR("Log domains init failed");
	}

	rdata = rockon_data_new();

	EINA_LOG_DBG("MainLoop Start");
	ecore_main_loop_begin();
	EINA_LOG_DBG("MainLoop End");

	log_shutdown();
	ecore_shutdown();

	return 0;
}

Eina_Bool log_init(void) {
	if (config_log_dom < 0) {
		config_log_dom = eina_log_domain_register("rck_config", NULL);
	} else return EINA_FALSE;

	return EINA_TRUE;
}

void log_shutdown(void) {
	if (config_log_dom >= 0) {
		eina_log_domain_unregister(config_log_dom);
		config_log_dom = -1;
	}
}
