#include "xmms_conn.h"

extern xmms_status app_status;

/* helper function: print all values from a dict */
void _my_dict_foreach (const char *key, xmmsv_t *value, void *user_data);

int xmms2_connect (xmmsc_connection_t **xmms_conn) {
	*xmms_conn = xmmsc_init ("rockon");
	if (! *xmms_conn) {
		print_error ("Couldn't create XMMS2 connection.", ERR_NORMAL);
		return FALSE;
	}
	if (!xmmsc_connect ( *xmms_conn, getenv ("XMMS_PATH"))) {
		print_error ("Connection to XMMS2 failed.", ERR_NORMAL);
		print_error (xmmsc_get_last_error (*xmms_conn), ERR_NORMAL);
		return FALSE;
	}

	XMMS_CALLBACK_SET (*xmms_conn, xmmsc_broadcast_playback_current_id,
					broadcast_playback_id_cb, NULL);
	XMMS_CALLBACK_SET (*xmms_conn, xmmsc_broadcast_playback_volume_changed,
					broadcast_playback_volume_cb, NULL);
	XMMS_CALLBACK_SET (*xmms_conn, xmmsc_broadcast_playlist_current_pos,
					broadcast_playlist_pos_cb, NULL);


	xmmsc_disconnect_callback_set (*xmms_conn, xmms2_disconnect_cb, NULL);
	app_status.connected = 1;
	xmmsc_mainloop_ecore_init (*xmms_conn);

	return TRUE;
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
		return TRUE;
	}
	return FALSE;
}

int broadcast_playback_id_cb (xmmsv_t *value, void *data) {
	if (! check_error(value, NULL)) {
		if (!xmmsv_get_int (value, &(app_status.playback_id)))
			app_status.playlist_pos = 0; // nothing is playing
		return TRUE; // keep broadcast alive
	}
	return FALSE;
}

int broadcast_playlist_pos_cb (xmmsv_t *value, void *data) {
	const char *pls_name;
	xmmsv_t *dict_entry;

	if (! check_error(value, NULL)) {
		if (!xmmsv_dict_get (value, "name", &dict_entry) ||
			!xmmsv_get_string (dict_entry, &pls_name)) {
			pls_name = "No Name";
		}
		if (!xmmsv_dict_get (value, "position", &dict_entry) ||
			!xmmsv_get_int (dict_entry, &app_status.playlist_pos)) {
			app_status.playlist_pos = -1;
		}

		if (app_status.playlist_name) {
			free (app_status.playlist_name);
			app_status.playlist_name = NULL;
		}
		app_status.playlist_name = (char*) malloc (sizeof(char)*(strlen(pls_name) + 1));
		if (app_status.playlist_name)
			strcpy(app_status.playlist_name, pls_name);
		else
			print_error("Memory allocation error.", ERR_CRITICAL);

		return TRUE;
	}
	return FALSE;
}

int broadcast_playback_volume_cb (xmmsv_t *value, void *data) {
	xmmsv_t *dict_entry;

	if (! check_error(value, NULL)) {
		if (!xmmsv_dict_get (value, "left", &dict_entry) ||
			!xmmsv_get_int (dict_entry, &app_status.volume_left)) {
			app_status.volume_left = 0;
		}
		if (!xmmsv_dict_get (value, "right", &dict_entry) ||
			!xmmsv_get_int (dict_entry, &app_status.volume_right)) {
			app_status.volume_right = 0;
		}
		return TRUE; // keep broadcast alive
	}
	return FALSE;
}

//int signal_playback_playtime_cb (xmmsv_t *value, void *data);


/**********************************************************************/
/* PASTEBIN  */
/**********************************************************************/


/*
xmmsv_type_t tipo = xmmsv_get_type (value);
printf("TYPE: %d\n", tipo);
// 0 XMMSV_TYPE_NONE,
// 1 XMMSV_TYPE_ERROR,
// 2 XMMSV_TYPE_INT32,
// 3 XMMSV_TYPE_STRING,
// 4 XMMSV_TYPE_COLL,
// 5 XMMSV_TYPE_BIN,
// 6 XMMSV_TYPE_LIST,
// 7 XMMSV_TYPE_DICT,
// 8 XMMSV_TYPE_END
*/


/* xmmsv_dict_foreach (value, _my_dict_foreach, NULL); */
void _my_dict_foreach (const char *key, xmmsv_t *value, void *user_data) {
	switch (xmmsv_get_type (value)) {
		case XMMSV_TYPE_NONE:
			/* nothing to do, empty value */
			break;
		case XMMSV_TYPE_INT32:
			{
				int val;
				xmmsv_get_int (value, &val);
				printf ("%s = %d\n", key, val);
				break;
			}
		case XMMSV_TYPE_STRING:
			{
				const char *val;
				xmmsv_get_string (value, &val);
				printf ("%s = %s\n", key, val);
				break;
			}
		default:
			break;
	}
}
