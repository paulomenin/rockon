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

#include <assert.h>
#include "xmms_conn.h"
#include "playlist.h"
#include "media_info.h"

int  _get_media_info(xmmsv_t *value, void *data);
void _dict_volume_foreach (const char *key, xmmsv_t *value, void *data);

void _playlist_get(rockon_status *s);
int  _playlist_fetch(xmmsv_t *value, void *data);
int  _playlist_item_add(xmmsv_t *value, void *data);
void _playlist_item_info_get(rockon_status *data, int id);

/* helper functions: print all values from a dict
 * FIXME remove these functions after finish debug
 */
void _my_dict_foreach (const char *key, xmmsv_t *value, void *user_data);
void _my_propdict_foreach (const char *key, xmmsv_t *src_val_dict,void *user_data);
void _my_propdict_inner_foreach (const char *source, xmmsv_t *value,void *user_data);



int xmms2_connect (rockon_status *status) {
	status->connection = xmmsc_init ("rockon");
	if (! status->connection) {
		print_error ("Couldn't create XMMS2 connection.", ERR_NORMAL);
		return FALSE;
	}
	if (!xmmsc_connect (status->connection, getenv ("rockon_PATH"))) {
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
					xmmsc_broadcast_playback_status,
					broadcast_playback_status_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_signal_playback_playtime,
					signal_playback_playtime_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_broadcast_playlist_current_pos,
					broadcast_playlist_pos_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_broadcast_playlist_changed,
					broadcast_playlist_changed_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_broadcast_playlist_loaded,
					broadcast_playlist_loaded_cb, status);

	xmmsc_disconnect_callback_set (status->connection, xmms2_disconnect_cb, (void*)status);
	status->connected = 1;

	xmmsc_mainloop_ecore_init (status->connection);

	return TRUE;
}

void xmms2_shutdown (rockon_status* status) {
	if ((status) && (status->connection)) {
		xmmsc_unref (status->connection);
		status->connection = NULL;
		status->connected = 0;
	}
}

void xmms2_disconnect_cb (void *data) {
	xmms2_shutdown ((rockon_status*)data);
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
	rockon_status *status = (rockon_status*)data;
	xmmsc_result_t *result;

	if (! check_error(value, NULL)) {
		if (!xmmsv_get_int (value, &(status->playback_id)))
			status->playback_id = 0; // nothing is playing

		if (status->playback_id != 0) {
			result = xmmsc_medialib_get_info(status->connection, status->playback_id);
			xmmsc_result_notifier_set (result, _get_media_info, status);
			xmmsc_result_unref(result);
		}

		status->changed_playback_id = 1;
		status_gui_update(status);
		return TRUE; // keep broadcast alive
	}

	return FALSE;
}

int broadcast_playlist_pos_cb (xmmsv_t *value, void *data) {
	rockon_status *status = (rockon_status*)data;
	const char *pls_name;
	xmmsv_t *dict_entry;

	if (! check_error(value, NULL)) {
		if (!xmmsv_dict_get (value, "name", &dict_entry) ||
			!xmmsv_get_string (dict_entry, &pls_name)) {
			pls_name = "No Name";
		}
		if (!xmmsv_dict_get (value, "position", &dict_entry) ||
			!xmmsv_get_int (dict_entry, &(status->playlist_pos))) {
			status->playlist_pos = -1;
		}

		if (status->playlist_name) {
			free (status->playlist_name);
			status->playlist_name = NULL;
		}

		status->playlist_name = (char*) malloc (sizeof(char)*(strlen(pls_name) + 1));
		if (status->playlist_name)
			strcpy(status->playlist_name, pls_name);
		else
			print_error("Memory allocation error.", ERR_CRITICAL);

		status->changed_playlist_pos = 1;
		status_gui_update(status);
		return TRUE;
	}
	return FALSE;
}

int broadcast_playlist_changed_cb (xmmsv_t *value, void *data) {
	rockon_status *status = (rockon_status*)data;

	// FIXME make proper handling of playlist changes!
	if (! check_error(value, NULL)) {
		_playlist_get(status);

		status->changed_playlist = 1;
		status_gui_update(status);
		return TRUE;
	}
	return FALSE;
}

int broadcast_playlist_loaded_cb (xmmsv_t *value, void *data) {
	rockon_status *status = (rockon_status*)data;

	if (! check_error(value, NULL)) {
		_playlist_get(status);

		status->changed_playlist = 1;
		status_gui_update(status);
		return TRUE;
	}
	return FALSE;
}

int broadcast_playback_volume_cb (xmmsv_t *value, void *data) {
	rockon_status *status = (rockon_status*)data;

	if (! check_error(value, NULL)) {
		xmmsv_dict_foreach (value, _dict_volume_foreach, status);

		status->changed_playback_volume = 1;
		status_gui_update(status);
		return TRUE; // keep broadcast alive
	}
	status->volume = 0;
	return FALSE;
}

int broadcast_playback_status_cb (xmmsv_t *value, void *data) {
	rockon_status *status = (rockon_status*)data;
	if (! check_error(value, NULL)) {
		xmmsv_get_int (value, &status->playback_status);

		status->changed_playback = 1;
		status_gui_update(status);
		return TRUE;
	}
	return FALSE;
}

int signal_playback_playtime_cb (xmmsv_t *value, void *data) {
	static int time = 0;
	int new_time;
	rockon_status *status = (rockon_status*)data;
	if (! check_error(value, NULL)) {
		xmmsv_get_int (value, &new_time);

		if (((new_time - time) > 999) ||
			(time > new_time)) {
			status->playtime = new_time;
			time = new_time;
			
			status->changed_playtime = 1;
			status_gui_update(status);
		}

		return TRUE;
	}
	return FALSE;
}

int _get_media_info(xmmsv_t *value, void *data) {
	rockon_status *status = (rockon_status*)data;

	assert(data);

	if (! check_error(value, NULL)) {

//xmmsv_dict_foreach (value, _my_propdict_foreach, NULL);
		if (status->playback_info) {
			media_info_del(status->playback_info);
		}
		status->playback_info = media_info_new();
		media_info_get(value, status->playback_info);

		status->changed_mediainfo = 1;
		status_gui_update(status);
		return TRUE;
	}
	return FALSE;
}

int _playlist_item_add(xmmsv_t *value, void *data) {
	static int pos;
	xmmsv_t *dict_entry;
	xmmsv_t *infos;
	const char *artist;
	const char *title;
	const char *album;
	playlist_item *pi;
	rockon_status *s = (rockon_status*)data;

	if (s == NULL) {
		pos = 0;
		return 0;
	}

	infos = xmmsv_propdict_to_dict (value, NULL);
	/*
	printf("\n\n\n");
	xmmsv_dict_foreach (infos, _my_dict_foreach, NULL);
	printf("\n\n\n");
	*/
	pi = (playlist_item*) malloc (sizeof(playlist_item));
	if (pi == NULL) return FALSE;

	if (!xmmsv_dict_get (infos, "id", &dict_entry) ||
	    !xmmsv_get_int (dict_entry, &(pi->id))) {
		pi->id = 0;
	}
	if (!xmmsv_dict_get (infos, "artist", &dict_entry) ||
	    !xmmsv_get_string (dict_entry, &artist)) {
		artist = "[Unknown Artist]";
	}
	if (!xmmsv_dict_get (infos, "title", &dict_entry) ||
	    !xmmsv_get_string (dict_entry, &title)) {
		title = "[Unknown Title]";
	}
	if (!xmmsv_dict_get (infos, "album", &dict_entry) ||
	    !xmmsv_get_string (dict_entry, &album)) {
		album = "[Unknown Album]";
	}

	pi->title = strdup(title);
	pi->artist = strdup(artist);
	pi->album = strdup(album);
	pi->status = s;
	pi->pos = pos;

	s->playlist = eina_list_append(s->playlist, pi);
	pos++;

	xmmsv_unref (infos);

	s->changed_playlist = 1;
	status_gui_update(s);

	return TRUE;
}

void _playlist_item_info_get(rockon_status *data, int id) {
	xmmsc_result_t *result;
	result = xmmsc_medialib_get_info(((rockon_status*)data)->connection, id);
	xmmsc_result_notifier_set (result, _playlist_item_add, data);
	xmmsc_result_unref (result);
}

int _playlist_fetch(xmmsv_t *value, void *data) {
	xmmsv_list_iter_t *it;
	if (! check_error(value, NULL)) {
		if (!xmmsv_get_list_iter (value, &it)) {
			print_error ("get list iterator failed!", ERR_NORMAL);
			return FALSE;
		}
		_playlist_item_add(NULL, NULL); // restart pos counter
		for (; xmmsv_list_iter_valid (it); xmmsv_list_iter_next (it)) {
			int id;
			xmmsv_t *list_entry;

			xmmsv_list_iter_entry (it, &list_entry);
			xmmsv_get_int (list_entry, &id);

			_playlist_item_info_get((rockon_status*)data, id);

		}

		return TRUE;
	}
	return FALSE;
}

void _playlist_get(rockon_status *s) {
	xmmsc_result_t *result;

	pls_free(s);

	result = xmmsc_playlist_list_entries(s->connection, NULL);
	xmmsc_result_notifier_set (result, _playlist_fetch, s);
	xmmsc_result_unref (result);

}

void _dict_volume_foreach (const char *key, xmmsv_t *value, void *data) {
	if (xmmsv_get_type (value) == XMMSV_TYPE_INT32) {
		xmmsv_get_int (value, &(((rockon_status*)data)->volume));
	}
}

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
				printf ("(int) %s = %d\n", key, val);
				break;
			}
		case XMMSV_TYPE_STRING:
			{
				const char *val;
				xmmsv_get_string (value, &val);
				printf ("(str) %s = %s\n", key, val);
				break;
			}
		default:
			break;
	}
}

void
_my_propdict_foreach (const char *key, xmmsv_t *src_val_dict,
                     void *user_data)
{
	xmmsv_dict_foreach (src_val_dict, _my_propdict_inner_foreach,
	                    (void *) key);
}

void
_my_propdict_inner_foreach (const char *source, xmmsv_t *value,
                           void *user_data)
{
	const char *key = (const char *) user_data;

	switch (xmmsv_get_type (value)) {
		case XMMSV_TYPE_NONE:
			/* nothing to do, empty value */
			break;
		case XMMSV_TYPE_INT32:
			{
				int val;
				xmmsv_get_int (value, &val);
				printf ("(int) %s:%s = %d\n", source, key, val);
				break;
			}
		case XMMSV_TYPE_STRING:
			{
				const char *val;
				xmmsv_get_string (value, &val);
				printf ("(str) %s:%s = %s\n", source, key, val);
				break;
			}
		default:
			break;
	}

}

