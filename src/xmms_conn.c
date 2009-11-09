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

int  _get_media_info(xmmsv_t *value, void *data);
void _dict_volume_foreach (const char *key, xmmsv_t *value, void *data);

/* helper functions: print all values from a dict
 * FIXME remove these functions after debbuging
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
	rockon_status *s = (rockon_status*)data;
	xmmsc_result_t *result;

	if (! check_error(value, NULL)) {
		if (!xmmsv_get_int (value, &(s->playback_id)))
			s->playback_id = 0; // nothing is playing

		if (s->playback_id != 0) {
			result = xmmsc_medialib_get_info(s->connection, s->playback_id);
			xmmsc_result_notifier_set (result, _get_media_info, s);
			xmmsc_result_unref(result);
		}

		return TRUE; // keep broadcast alive
	}

	return FALSE;
}

int broadcast_playlist_pos_cb (xmmsv_t *value, void *data) {
	rockon_status *s = (rockon_status*)data;
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

		if (s->playlist_name) {
			free (s->playlist_name);
			s->playlist_name = NULL;
		}

		s->playlist_name = (char*) malloc (sizeof(char)*(strlen(pls_name) + 1));
		if (s->playlist_name)
			strcpy(s->playlist_name, pls_name);
		else
			print_error("Memory allocation error.", ERR_CRITICAL);

		s->changed_playlist = 1;
		status_gui_update(s);
		return TRUE;
	}
	return FALSE;
}

int broadcast_playback_volume_cb (xmmsv_t *value, void *data) {
	rockon_status *s = (rockon_status*)data;

	if (! check_error(value, NULL)) {
		xmmsv_dict_foreach (value, _dict_volume_foreach, s);

		s->changed_playback_volume = 1;
		status_gui_update(s);
		return TRUE; // keep broadcast alive
	}
	s->volume = 0;
	return FALSE;
}

int broadcast_playback_status_cb (xmmsv_t *value, void *data) {
	rockon_status *s = (rockon_status*)data;
	if (! check_error(value, NULL)) {
		xmmsv_get_int (value, &s->playback_status);

		s->changed_playback = 1;
		status_gui_update(s);
		return TRUE;
	}
	return FALSE;
}

int signal_playback_playtime_cb (xmmsv_t *value, void *data) {
	static int time = 0;
	int new_time;
	rockon_status *s = (rockon_status*)data;
	if (! check_error(value, NULL)) {
		xmmsv_get_int (value, &new_time);

		if (((new_time - time) > 999) ||
			(time > new_time)) {
			s->playtime = new_time;
			time = new_time;
			
			s->changed_playtime = 1;
			status_gui_update(s);
		}

		return TRUE;
	}
	return FALSE;
}

int _get_media_info(xmmsv_t *value, void *data) {
	rockon_status *s = (rockon_status*)data;
	xmmsv_t *infos, *dict_entry;
	const char *artist, *album, *title, *url, *comment, *genre, *date;

	if (! check_error(value, NULL)) {

//xmmsv_dict_foreach (value, _my_propdict_foreach, NULL);

		infos = xmmsv_propdict_to_dict(value, NULL);

		if (!xmmsv_dict_get (infos, "artist", &dict_entry) ||
		    !xmmsv_get_string (dict_entry, &artist)) {
			artist = "[Unknown Artist]";
		}
		if (!xmmsv_dict_get (infos, "album", &dict_entry) ||
		    !xmmsv_get_string (dict_entry, &album)) {
			album = "[Unknown Album]";
		}
		if (!xmmsv_dict_get (infos, "title", &dict_entry) ||
		    !xmmsv_get_string (dict_entry, &title)) {
			title = "[Unknown Title]";
		}
		if (!xmmsv_dict_get (infos, "url", &dict_entry) ||
		    !xmmsv_get_string (dict_entry, &url)) {
			url = "[Unknown URL]";
		}
		if (!xmmsv_dict_get (infos, "comment", &dict_entry) ||
		    !xmmsv_get_string (dict_entry, &comment)) {
			comment = "";
		}
		if (!xmmsv_dict_get (infos, "genre", &dict_entry) ||
		    !xmmsv_get_string (dict_entry, &genre)) {
			genre = "[Unknown genre]";
		}
		if (!xmmsv_dict_get (infos, "date", &dict_entry) ||
		    !xmmsv_get_string (dict_entry, &date)) {
			date = "";
		}
		
		if (!xmmsv_dict_get (infos, "duration", &dict_entry) ||
		    !xmmsv_get_int (dict_entry, &s->media_duration)) {
			s->media_duration = 0;
		}

		s->media_artist = artist;
		s->media_album = album;
		s->media_title = title;
		s->media_url = url;
		s->media_comment = comment;
		s->media_genre = genre;
		s->media_date = date;

		s->changed_mediainfo = 1;
		status_gui_update(s);
		xmmsv_unref(infos);
		return TRUE;
	}
	return FALSE;
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

