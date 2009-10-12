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

#include "error.h"
#include "config.h"
#include "xmms_conn.h"
#include "status.h"
#include "elm_bridge.h"


EAPI int elm_main (int argc, char** argv) {
	rockon_config app_config;
	xmms_status app_status;

	ecore_file_init();

	if ( ! config_load (&app_config))
		print_error ("Couldn't load config. Loaded default values.", ERR_WARNING);

	app_status.connected = 0;
	app_status.connection = NULL;
	app_status.config = &app_config;
	app_status.windows = NULL;
	app_status.changed_playback = 0;
	app_status.changed_playback_volume = 0;
	app_status.changed_playtime = 0;
	app_status.changed_mediainfo = 0;
	app_status.changed_playlist = 0;
	app_status.playlist_name = NULL;

	xmms2_connect (&app_status);

	elm_cb_set (&app_status, NULL, "Rockon", "elm_set.win");

	elm_run();

	if ( ! config_save (&app_config))
		print_error ("Couldn't save config.", ERR_WARNING);

	/* Clean resources */
	elm_shutdown();
	xmms2_shutdown (&app_status);
	config_free(&app_config);
	status_free(&app_status);

	printf("\nDEBUG: exit main\n");
	return 0;
}
ELM_MAIN()
