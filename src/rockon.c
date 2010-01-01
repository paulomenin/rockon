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

EAPI int elm_main (int argc, char** argv) {
	rockon_config *app_config;

	app_config = config_new();

	EINA_LOG_DBG("THEME NAME: %s", app_config->theme_name);
	EINA_LOG_DBG("THEME: %s", app_config->theme);

	EINA_LOG_DBG("MainLoop Start");
	elm_run();
	EINA_LOG_DBG("MainLoop End");

	config_save(app_config);
	config_del(app_config);
	elm_shutdown();

	return 0;
}
ELM_MAIN()
