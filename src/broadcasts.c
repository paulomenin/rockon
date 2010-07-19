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
#include <Eina.h>
#include "broadcasts.h"
#include "rockon_data.h"
#include "xmms_conn.h"
#include "ui_update.h"

#include "util.h" // TODO remove after debug

#define DBG(...) EINA_LOG_DOM_DBG(broadcast_log_dom, __VA_ARGS__)
#define WARN(...) EINA_LOG_DOM_WARN(broadcast_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(broadcast_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(broadcast_log_dom, __VA_ARGS__)

extern int broadcast_log_dom;

int  get_media_info_cb   (xmmsv_t *value, void *data);
void playback_volume_add (const char *key, xmmsv_t *value, void *user_data);

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
void playback_volume_add(const char *key, xmmsv_t *value, void *user_data) {
	rockon_data *rdata = (rockon_data*)user_data;
	volume_channel *ch;

	ch = volume_channel_new();
	if (ch) {
		ch->name = strdup(key);
		xmmsv_get_int(value, &(ch->value));

		rdata->volume = eina_list_append(rdata->volume, ch);
	}
}

int broadcast_playback_volume_cb (xmmsv_t *value, void *data) {
	rockon_data *rdata = (rockon_data*)data;

	volume_del(rdata->volume);
	rdata->volume = NULL;
	xmmsv_dict_foreach (value, playback_volume_add, data);

	ui_upd_playback_volume(rdata);
	return 1;
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
	playlist* pls;
	xmmsv_t *dict_entry;

	if (! check_error(value, NULL)) {
		if (!xmmsv_dict_get (value, "name", &dict_entry) ||
			!xmmsv_get_string (dict_entry, &pls_name)) {
			WARN("playlist name not found");
			return 0;
		}
		if (!xmmsv_dict_get (value, "position", &dict_entry) ||
			!xmmsv_get_int (dict_entry, &pos)) {
			WARN("playlist position not found");
			return 0;
		}

		pls = playlist_find(rdata->playlists, pls_name);
		if (pls) {
			if (pls != rdata->current_playlist)
				rdata->current_playlist = pls;
			pls->current_pos = pos;
		} else {
			WARN("playlist \'%s\' not found", pls_name);
		}
		ui_upd_playlist_pos(rdata);
/*
		if (strcmp(rdata->current_playlist->name, pls_name) == 0 ) {
			rdata->current_playlist->current_pos = pos;
			ui_upd_playlist_pos(rdata);
		} else {
			playlist_clear_items(rdata->current_playlist);
			rdata->current_playlist = playlist_get_by_name(rdata->connection, pls_name, rdata);
			rdata->current_playlist->current_pos = pos;
			ui_upd_playlist_pos(rdata);
		}
*/
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

	DBG("PLS CHANGED CALLBACK");

	if (! check_error(value, NULL)) {
		xmmsv_t *vtype, *vname, *vpos, *vnewpos, *vid;
		if (xmmsv_dict_get(value, "type", &vtype)) {
			xmmsv_get_int(vtype, &type);
			xmmsv_dict_get(value, "name", &vname);
			xmmsv_get_string(vname, &name);
			pls = playlist_find(rdata->playlists, name);

			WARN("type: %d", type);
			dump_xmms_value(value);
			switch (type) {
				case XMMS_PLAYLIST_CHANGED_ADD:
				case XMMS_PLAYLIST_CHANGED_INSERT:
					xmmsv_dict_get(value, "id", &vid);
					xmmsv_get_int(vid, &id);
					xmmsv_dict_get(value, "position", &vpos);
					xmmsv_get_int(vpos, &pos);
					playlist_change_item_add(rdata, name, pos, id); // bugged
					break;
				case XMMS_PLAYLIST_CHANGED_REMOVE:
					xmmsv_dict_get(value, "position", &vpos);
					xmmsv_get_int(vpos, &pos);
					playlist_change_item_del(rdata->playlists, name, pos);
					break;
				case XMMS_PLAYLIST_CHANGED_MOVE:
					xmmsv_dict_get(value, "position", &vpos);
					xmmsv_get_int(vpos, &pos);
					xmmsv_dict_get(value, "newposition", &vnewpos);
					xmmsv_get_int(vnewpos, &newpos);
					playlist_change_item_moved(rdata->playlists, name, pos, newpos);
					break;
				case XMMS_PLAYLIST_CHANGED_CLEAR:
					playlist_clear_items(pls);
					break;
				case XMMS_PLAYLIST_CHANGED_SHUFFLE:
				case XMMS_PLAYLIST_CHANGED_SORT:
				case XMMS_PLAYLIST_CHANGED_UPDATE:
				default:
					playlist_get_by_name(rdata->connection, name, rdata);
			}
			ui_upd_playlist(rdata, pls);
		}
		return 1;
	}
	return 0;
}

int mlib_reader_status_cb (xmmsv_t *value, void *data) {
	int status;

	if (! check_error(value, NULL)) {
		xmmsv_get_int(value, &status);
		if (status == XMMS_MEDIAINFO_READER_STATUS_IDLE) {
			ui_upd_mlib_reader_status((rockon_data*)data, 0);
		} else if (status == XMMS_MEDIAINFO_READER_STATUS_RUNNING) {
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

int  broadcast_collection_changed_cb(xmmsv_t *value, void *data) {
	rockon_data* rdata = (rockon_data*)data;
	DBG("Coll Changed");
	dump_xmms_value(value);
	//XMMS_COLLECTION_CHANGED_ADD
	//XMMS_COLLECTION_CHANGED_UPDATE
	//XMMS_COLLECTION_CHANGED_RENAME
	//XMMS_COLLECTION_CHANGED_REMOVE
	if (rdata->collections != NULL) {
		coll_list_del(rdata->collections);
	}
	rdata->collections = coll_list_get(rdata->connection, rdata);
	coll_list_wait(rdata->collections);
	return 1; // keep broadcast alive
}
