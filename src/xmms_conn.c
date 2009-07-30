#include "xmms_conn.h"

int xmms2_connect (xmmsc_connection_t **xmms_conn) {
	*xmms_conn = xmmsc_init ("rockon");
	if (! *xmms_conn) {
		fprintf (stderr, "xmms2 connection failed!\n");
		return 0;
	}
	if (!xmmsc_connect ( *xmms_conn, getenv ("XMMS_PATH"))) {
		fprintf (stderr, "Connection failed: %s\n",
				xmmsc_get_last_error (*xmms_conn));
		return 0;
	}
	xmmsc_mainloop_ecore_init (*xmms_conn);
	return 1;
}
