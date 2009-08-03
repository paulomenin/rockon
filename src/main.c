#include "libs_setup.h"
#include "error.h"
#include "config.h"
#include "xmms_conn.h"
#include "status.h"
#include "gui.h"

/* Globals */
rockon_config app_config;
xmms_status app_status;
xmmsc_connection_t *xmms_conn_async = NULL;

int main (int argc, char** argv) {

	efl_init ();

	if ( ! config_load (&app_config))
		print_error ("Couldn't load config. Loaded default values.", ERR_WARNING);

	app_status.connected = 0;

	/* Set xmms2 connection */

	if (! xmms2_connect (&xmms_conn_async)) {
		fprintf(stderr,"CONN XMMS MAIN ERROR");
		//return -2;
	}

	gui_setup();

	if ( ! config_save (&app_config))
		print_error ("Couldn't save config.", ERR_WARNING);

	xmms2_shutdown ();
	efl_shutdown ();

	printf("\nDEBUG: exit main\n");
	return EXIT_SUCCESS;
}

