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
#include <assert.h>
#include "error.h"
#include "config.h"
#include "xmms_conn.h"
#include "status.h"
#include "elm_bridge.h"
#include "playlist.h"

EAPI int elm_main (int argc, char** argv) {
	rockon_config *app_config = NULL;
	rockon_status *app_status = NULL;

	ecore_file_init();

	app_config = config_new();
	assert(app_config);

	app_status = status_new();
	assert(app_status);
	app_status->config = app_config;

	pls_item_class_set(app_status);

	xmms2_connect (app_status);

	elm_cb_set (app_status, NULL, "Rockon", "elm_set.win");

	elm_run();

	if ( ! config_save (app_config))
		print_error ("Couldn't save config.", ERR_WARNING);

	/* Clean resources */
	xmms2_shutdown (app_status);
	config_del(app_config);
	status_del(app_status);

	ecore_file_shutdown();
	elm_shutdown();

	return 0;
}
ELM_MAIN()
