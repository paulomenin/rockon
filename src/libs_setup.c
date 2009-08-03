#include "libs_setup.h"

extern xmmsc_connection_t *xmms_conn_async;
extern xmms_status app_status;

int efl_init () {
	ecore_init ();
	ecore_config_init ("Rockon");
	evas_init ();
	ecore_evas_init ();
	edje_init();
	return TRUE;
}

void efl_shutdown () {
	edje_shutdown ();
	ecore_evas_shutdown ();
	evas_shutdown ();
	ecore_config_shutdown ();
	ecore_shutdown ();

	if (app_status.playlist_name)
		free (app_status.playlist_name);

	printf ("DEBUG: shutdown\n");
}

/* FIXME maybe move to xmms_conn.c ? */
void xmms2_shutdown () {
	if (xmms_conn_async) {
		xmmsc_unref (xmms_conn_async);
		xmms_conn_async = NULL;
		app_status.connected = 0;
	}
}
