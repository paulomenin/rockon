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
#include "gui.h"


EAPI int elm_main (int argc, char** argv) {
	rockon_config app_config;
	xmms_status app_status;

	ecore_config_init ("Rockon");
	ecore_file_init();

	if ( ! config_load (&app_config))
		print_error ("Couldn't load config. Loaded default values.", ERR_WARNING);

	app_status.connected = 0;
	app_status.connection = NULL;
	app_status.edje_gui = NULL;
	app_status.playlist_name = NULL;

	xmms2_connect (&app_status);

	gui_setup(&app_config, &app_status);

	if ( ! config_save (&app_config))
		print_error ("Couldn't save config.", ERR_WARNING);

	/* Clean resources */
	xmms2_shutdown (&app_status);
	//efl_shutdown ();

	printf("DEBUG: PLS: %s END: %p\n",app_status.playlist_name,&(app_status.playlist_name) );
	if (app_status.playlist_name)
		free (app_status.playlist_name);

	printf("\nDEBUG: exit main\n");
	return EXIT_SUCCESS;
}
ELM_MAIN()
