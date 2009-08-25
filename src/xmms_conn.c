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

#include "xmms_conn.h"

/* helper function: print all values from a dict */
void _my_dict_foreach (const char *key, xmmsv_t *value, void *user_data);

int xmms2_connect (xmms_status *status) {

	status->connection = xmmsc_init ("rockon");
	if (! status->connection) {
		print_error ("Couldn't create XMMS2 connection.", ERR_NORMAL);
		return FALSE;
	}
	if (!xmmsc_connect (status->connection, getenv ("XMMS_PATH"))) {
		print_error ("Connection to XMMS2 failed.", ERR_NORMAL);
		print_error (xmmsc_get_last_error (status->connection), ERR_NORMAL);
		return FALSE;
	}

	XMMS_CALLBACK_SET (status->connection,
					xmmsc_broadcast_playback_current_id,
					broadcast_playback_id_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_broadcast_playback_volume_changed,
					broadcast_playback_volume_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_broadcast_playlist_current_pos,
					broadcast_playlist_pos_cb, status);

	xmmsc_disconnect_callback_set (status->connection, xmms2_disconnect_cb, (void*)status);
	status->connected = 1;
	xmmsc_mainloop_ecore_init (status->connection);

	return TRUE;
}

void xmms2_shutdown (xmms_status* status) {
	if ((status) && (status->connection)) {
		xmmsc_unref (status->connection);
		status->connection = NULL;
		status->connected = 0;
	}
}

void xmms2_disconnect_cb (void *data) {
	xmms2_shutdown ((xmms_status*)data);
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
	xmms_status *s = (xmms_status*)data;

	if (! check_error(value, NULL)) {
		if (!xmmsv_get_int (value, &(s->playback_id)))
			s->playback_id = 0; // nothing is playing
		//status_gui_update((xmms_status*)data);
		return TRUE; // keep broadcast alive
	}
	return FALSE;
}

int broadcast_playlist_pos_cb (xmmsv_t *value, void *data) {
	xmms_status *s = (xmms_status*)data;
	const char *pls_name;
	xmmsv_t *dict_entry;

	if (! check_error(value, NULL)) {
		if (!xmmsv_dict_get (value, "name", &dict_entry) ||
			!xmmsv_get_string (dict_entry, &pls_name)) {
			pls_name = "No Name";
		}
		if (!xmmsv_dict_get (value, "position", &dict_entry) ||
			!xmmsv_get_int (dict_entry, &(s->playlist_pos))) {
			s->playlist_pos = -1;
		}

		printf("DEBUG: S: %s END: %p",s->playlist_name,&(s->playlist_name) );
		if (s->playlist_name) {
			free (s->playlist_name);
			s->playlist_name = NULL;
		}

		s->playlist_name = (char*) malloc (sizeof(char)*(strlen(pls_name) + 1));
		if (s->playlist_name)
			strcpy(s->playlist_name, pls_name);
		else
			print_error("Memory allocation error.", ERR_CRITICAL);

		printf("PLS: %s POS: %d\n", s->playlist_name, s->playlist_pos);
		return TRUE;
	}
	return FALSE;
}

int broadcast_playback_volume_cb (xmmsv_t *value, void *data) {
	xmmsv_t *dict_entry;
	xmms_status *s = (xmms_status*)data;

	if (! check_error(value, NULL)) {
		if (!xmmsv_dict_get (value, "left", &dict_entry) ||
			!xmmsv_get_int (dict_entry, &(s->volume_left))) {
			s->volume_left = 0;
		}
		if (!xmmsv_dict_get (value, "right", &dict_entry) ||
			!xmmsv_get_int (dict_entry, &(s->volume_right))) {
			s->volume_right = 0;
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
