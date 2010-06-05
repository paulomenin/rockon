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
#include <string.h>
#include "broadcasts.h"
#include "rockon_data.h"
#include "xmms_conn.h"
#include "ui_update.h"

#include "util.h" // TODO remove after debug

int get_media_info_cb (xmmsv_t *value, void *data);

int broadcast_playback_status_cb (xmmsv_t *value, void *data) {
	rockon_data *rdata = (rockon_data*)data;
	if (! check_error(value, NULL)) {
		xmmsv_get_int (value, &rdata->playback_status);

		ui_upd_playback_status(rdata);
		return 1; // keep broadcast alive
	}
	return 0;
}

int broadcast_playback_id_cb (xmmsv_t *value, void *data) {
	rockon_data *rdata = (rockon_data*)data;
	xmmsc_result_t *result;

	if (! check_error(value, NULL)) {
		if (!xmmsv_get_int (value, &(rdata->playback_id)))
			rdata->playback_id = 0; // nothing is playing

		if (rdata->playback_id != 0) {
			result = xmmsc_medialib_get_info(rdata->connection, rdata->playback_id);
			xmmsc_result_notifier_set (result, get_media_info_cb, rdata);
			xmmsc_result_unref(result);
		}

		ui_upd_playback_id(rdata);

		return 1; // keep broadcast alive
	}

	return 0;
}

int signal_playback_playtime_cb (xmmsv_t *value, void *data) {
	static int time = 0;
	int new_time;
	rockon_data *rdata = (rockon_data*)data;
	if (! check_error(value, NULL)) {
		xmmsv_get_int (value, &new_time);

		if (((new_time - time) > 999) ||
			(time > new_time)) {
			rdata->playback_playtime = new_time;
			time = new_time;
			
			ui_upd_playback_playtime(rdata);
		}

		return 1; // keep signal alive
	}
	return 0;
}

int broadcast_playlist_pos_cb (xmmsv_t *value, void *data) {
	rockon_data *rdata = (rockon_data*)data;
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

		if (strcmp(rdata->current_playlist->name, pls_name) == 0 ) {
			rdata->current_playlist->current_pos = pos;
			ui_upd_playlist_pos(rdata);
		} else {
			playlist_clear_items(rdata->current_playlist);
			rdata->current_playlist = playlist_get_by_name(rdata->connection, pls_name, rdata);
			rdata->current_playlist->current_pos = pos;
			ui_upd_playlist_pos(rdata);
		}
		return 1;
	}
	return 0;
}

int get_media_info_cb (xmmsv_t *value, void *data) {
	rockon_data *rdata = (rockon_data*)data;
	assert(data);

	if (! check_error(value, NULL)) {

		if (rdata->playback_info != NULL) {
			media_info_del(rdata->playback_info);
			rdata->playback_info = NULL;
		}
		rdata->playback_info = media_info_new();
		media_info_get(value, rdata->playback_info);

		ui_upd_playback_info(rdata);
		return 1;
	}
	return 0;
}

int broadcast_playlist_loaded_cb (xmmsv_t *value, void *data) {
	rockon_data *rdata = (rockon_data*)data;
	const char *pls_name;

	if (! check_error(value, NULL)) {
		xmmsv_get_string(value, &pls_name);

		playlist_clear_items(rdata->current_playlist);
		rdata->current_playlist = playlist_get_by_name(rdata->connection, pls_name, rdata);
		ui_upd_playlist(rdata, rdata->current_playlist);
		return 1;
	}
	return 0;
}

int broadcast_playlist_changed_cb (xmmsv_t *value, void *data) {
	rockon_data *rdata = (rockon_data*)data;
	playlist *pls;
	int type, pos, newpos, id;
	const char *name;

	EINA_LOG_DBG("PLS CHANGED CALLBACK");

	if (! check_error(value, NULL)) {
		xmmsv_t *vtype, *vname, *vpos, *vnewpos, *vid;
		//dump_xmms_value(value);
		if (xmmsv_dict_get(value, "type", &vtype)) {
			xmmsv_get_int(vtype, &type);
			xmmsv_dict_get(value, "name", &vname);
			xmmsv_get_string(vname, &name);
			xmmsv_dict_get(value, "position", &vpos);
			xmmsv_get_int(vpos, &pos);
			switch (type) {
				case 0:
					xmmsv_dict_get(value, "id", &vid);
					xmmsv_get_int(vid, &id);
					playlist_change_item_add(rdata, name, pos, id);
					break;
				case 3:
					playlist_change_item_del(rdata->playlists, name, pos);
					break;
				case 5:
					xmmsv_dict_get(value, "newposition", &vnewpos);
					xmmsv_get_int(vnewpos, &newpos);
					playlist_change_item_moved(rdata->playlists, name, pos, newpos);
					break;
				default:
				EINA_LOG_DBG("unknown type: %d", type);
				// reload entire playlist
				//pls = playlist_get_by_name(rdata->connection, name, rdata);
			}
		}
		pls = playlist_find(rdata->playlists, name);
		ui_upd_playlist(rdata, pls);
		return 1;
	}
	return 0;
}

int mlib_reader_status_cb (xmmsv_t *value, void *data) {
	int status;

	if (! check_error(value, NULL)) {
		xmmsv_get_int(value, &status);
		if (status == 0) {
			ui_upd_mlib_reader_status((rockon_data*)data, 0);
		} else if (status == 1) {
			ui_upd_mlib_reader_status((rockon_data*)data, -1);
		}
		return 1;
	}
	return 0;
}

int mlib_reader_unindexed_cb (xmmsv_t *value, void *data) {
	int number_of_files;

	if (! check_error(value, NULL)) {
		xmmsv_get_int(value, &number_of_files);
		ui_upd_mlib_reader_status((rockon_data*)data, number_of_files);
		return 1;
	}
	return 0;
}
