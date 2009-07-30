#include "libs_setup.h"
#include "error.h"
#include "config.h"
#include "xmms_conn.h"
#include "gui.h"

/* Globals */
Rockon_config app_config;
xmmsc_connection_t *xmms_conn_async = NULL;

int main (int argc, char** argv) {

	efl_init ();

	/* load config */

	if (!config_load (&app_config)) {
        print_error("Failed to load config", ERR_CRITICAL);
	}

	/* Set xmms2 connection */

	if (! xmms2_connect (&xmms_conn_async)) {
		fprintf(stderr,"Exiting");
		return -2;
	}

	/* set gui */

	gui_setup();

	/* clean resources */
	xmms2_shutdown ();
	efl_shutdown ();

	return EXIT_SUCCESS;
}

