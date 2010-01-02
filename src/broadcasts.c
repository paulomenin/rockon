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
#include "broadcasts.h"
#include "server_data.h"
#include "xmms_conn.h"
#include "gui_update.h"

#include "playlist.h"

int get_media_info_cb (xmmsv_t *value, void *data);

int broadcast_playback_status_cb (xmmsv_t *value, void *data) {
	server_data *sdata = (server_data*)data;
	if (! check_error(value, NULL)) {
		xmmsv_get_int (value, &sdata->playback_status);

		gui_upd_playback_status(sdata);
		return 1; // keep broadcast alive
	}
	return 0;
}

int broadcast_playback_id_cb (xmmsv_t *value, void *data) {
	server_data *sdata = (server_data*)data;
	xmmsc_result_t *result;

	if (! check_error(value, NULL)) {
		if (!xmmsv_get_int (value, &(sdata->playback_id)))
			sdata->playback_id = 0; // nothing is playing

		if (sdata->playback_id != 0) {
			result = xmmsc_medialib_get_info(sdata->connection, sdata->playback_id);
			xmmsc_result_notifier_set (result, get_media_info_cb, sdata);
			xmmsc_result_unref(result);
		}

		gui_upd_playback_id(sdata);

		return 1; // keep broadcast alive
	}

	return 0;
}

int signal_playback_playtime_cb (xmmsv_t *value, void *data) {
	static int time = 0;
	int new_time;
	server_data *sdata = (server_data*)data;
	if (! check_error(value, NULL)) {
		xmmsv_get_int (value, &new_time);

		if (((new_time - time) > 999) ||
			(time > new_time)) {
			sdata->playback_playtime = new_time;
			time = new_time;
			
			gui_upd_playback_playtime(sdata);
		}

		return 1; // keep signal alive
	}
	return 0;
}

int broadcast_playlist_pos_cb (xmmsv_t *value, void *data) {
	server_data *sdata = (server_data*)data;
	const char *pls_name;
	int pos;
	xmmsv_t *dict_entry;

	if (! check_error(value, NULL)) {
		if (!xmmsv_dict_get (value, "name", &dict_entry) ||
			!xmmsv_get_string (dict_entry, &pls_name)) {
			pls_name = "No Name";
		}
		if (!xmmsv_dict_get (value, "position", &dict_entry) ||
			!xmmsv_get_int (dict_entry, &pos)) {
			pos = -1;
		}

		if (sdata->playlist_current != NULL) {

			if (sdata->playlist_current->name) {
				free (sdata->playlist_current->name);
				sdata->playlist_current->name = NULL;
			}

			sdata->playlist_current->name = strdup(pls_name);
			sdata->playlist_current->current_pos = pos;

			gui_upd_playlist_pos(sdata);
			return TRUE;
		}
	}
	return FALSE;
}

int get_media_info_cb (xmmsv_t *value, void *data) {
	server_data *sdata = (server_data*)data;
	assert(data);

	if (! check_error(value, NULL)) {

		if (sdata->playback_info != NULL) {
			media_info_del(sdata->playback_info);
		}
		sdata->playback_info = media_info_new();
		media_info_get(value, sdata->playback_info);

		gui_upd_playback_info(sdata);
		return 1;
	}
	return 0;
}

