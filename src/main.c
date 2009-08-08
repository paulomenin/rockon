#include "libs_setup.h"
#include "error.h"
#include "config.h"
#include "xmms_conn.h"
#include "status.h"
#include "gui.h"

int main (int argc, char** argv) {
	rockon_config app_config;
	xmms_status app_status;

	efl_init ();

	if ( ! config_load (&app_config))
		print_error ("Couldn't load config. Loaded default values.", ERR_WARNING);

	app_status.connected = 0;
	app_status.connection = NULL;
	app_status.edje_gui = NULL;
	app_status.playlist_name = NULL;

	xmms2_connect (&app_status);

	gui_setup(&app_status);

	if ( ! config_save (&app_config))
		print_error ("Couldn't save config.", ERR_WARNING);

	/* Clean resources */
	xmms2_shutdown (&app_status);
	efl_shutdown ();

	printf("DEBUG: PLS: %s END: %p",app_status.playlist_name,&(app_status.playlist_name) );
	if (app_status.playlist_name)
		free (app_status.playlist_name);

	printf("\nDEBUG: exit main\n");
	return EXIT_SUCCESS;
}

