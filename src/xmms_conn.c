#include "xmms_conn.h"

extern xmms_status app_status;

int xmms2_connect (xmmsc_connection_t **xmms_conn) {
	*xmms_conn = xmmsc_init ("rockon");
	if (! *xmms_conn) {
		print_error ("Couldn't create XMMS2 connection.", ERR_NORMAL);
		return 0;
	}
	if (!xmmsc_connect ( *xmms_conn, getenv ("XMMS_PATH"))) {
		print_error ("Connection to XMMS2 failed.", ERR_NORMAL);
		print_error (xmmsc_get_last_error (*xmms_conn), ERR_NORMAL);
		return 0;
	}
	xmmsc_disconnect_callback_set (*xmms_conn, xmms2_disconnect_cb, NULL);
	app_status.connected = 1;
	xmmsc_mainloop_ecore_init (*xmms_conn);
	return 1;
}

void xmms2_disconnect_cb (void *data) {
	xmms2_shutdown ();
	print_error ("xmms2 connection lost.", ERR_WARNING);
}

int check_error (xmmsv_t *value, void *data) {
	const char *err_buf;

	if ( (value) && xmmsv_is_error (value) &&
		xmmsv_get_error (value, &err_buf)) {
		print_error (err_buf, ERR_NORMAL);
		return 1;
	}
	return 0;
}
