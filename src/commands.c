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
#include <limits.h>
#include <Ecore_File.h>
#include <Eina.h>
#include "xmms_conn.h"
#include "commands.h"
#include "util.h"
#include "ui_update.h"

#define DBG(...) EINA_LOG_DOM_DBG(cmd_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(cmd_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(cmd_log_dom, __VA_ARGS__)

extern int cmd_log_dom;

int cmd_coll_load_inner (xmmsv_t *value, void* data);
void cmd_coll_load_inner2(const char *key, xmmsv_t *value, void *data);

void cmd_play (rockon_data *rdata) {
	assert(rdata);

	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (rdata->connection, xmmsc_playback_start,
					check_error, NULL);
}

void cmd_pause (rockon_data *rdata) {
	assert(rdata);

	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (rdata->connection, xmmsc_playback_pause,
					check_error, NULL);
}

void cmd_stop (rockon_data *rdata) {
	assert(rdata);

	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (rdata->connection, xmmsc_playback_stop,
					check_error, NULL);
}

void cmd_next (rockon_data *rdata) {
	assert(rdata);

	xmmsc_result_t *result;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_set_next_rel (rdata->connection, 1);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (rdata->connection, xmmsc_playback_tickle,
					check_error, NULL);
}

void cmd_prev (rockon_data *rdata) {
	assert(rdata);

	xmmsc_result_t *result;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_set_next_rel (rdata->connection, -1);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (rdata->connection, xmmsc_playback_tickle,
					check_error, NULL);
}

void cmd_seek_ms (rockon_data *rdata, int miliseconds) {
	xmmsc_result_t *result;
	assert(rdata);

	XMMS_CONN_IS_VALID();
	result = xmmsc_playback_seek_ms(rdata->connection, miliseconds, XMMS_PLAYBACK_SEEK_SET);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_playlist_create(rockon_data *rdata, const char* name) {
	xmmsc_result_t *result;
	assert(rdata);
	if (name == NULL) return;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_create(rdata->connection, name);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	DBG("pls created %s", name);
}

void cmd_playlist_delete(rockon_data *rdata, const char* name) {
	xmmsc_result_t *result;
	assert(rdata);
	if (name == NULL) return;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_remove(rdata->connection, name);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_playlist_load(rockon_data* rdata, const char *playlist) {
	xmmsc_result_t *result;
	assert(rdata);
	if (playlist == NULL) return;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_load(rdata->connection, playlist);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_playlist_clear(rockon_data* rdata, const char *playlist) {
	xmmsc_result_t *result;
	assert(rdata);
	if (playlist == NULL) return;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_clear(rdata->connection, playlist);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_playlist_add(rockon_data *rdata, unsigned int id) {
	xmmsc_result_t *result;

	assert(rdata);
	XMMS_CONN_IS_VALID();

	result = xmmsc_playlist_add_id(rdata->connection, rdata->current_playlist->name, id);
	xmmsc_result_notifier_set(result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_playlist_add_coll(rockon_data *rdata, xmmsv_coll_t *coll) {
	Eina_List *l;
	playlist_item* pi;

	assert(rdata);
	if (coll == NULL) return; 

	EINA_LIST_FOREACH(rdata->coll_queried, l, pi) {
		cmd_playlist_add(rdata, pi->id);
	}
}

void cmd_playlist_remove_item(rockon_data *rdata, const char *playlist, int pos) {
	xmmsc_result_t *result;
	assert(rdata);
	if (playlist == NULL) return;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_remove_entry(rdata->connection, playlist, pos);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_jump_to (rockon_data *rdata, int pos) {
	assert(rdata);
	assert(pos >= 0);

	xmmsc_result_t *result;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_set_next (rdata->connection, pos);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_jump_and_play (rockon_data *rdata, int pos) {
	assert(rdata);
	assert(pos >= 0);

	cmd_jump_to(rdata, pos);
	XMMS_CALLBACK_SET (rdata->connection, xmmsc_playback_tickle,
		check_error, NULL);

	if (rdata->playback_status != 1) {
		cmd_play(rdata);
	}
}

void cmd_server_launch(rockon_data *rdata) {
	if (rdata->config->launch_server == 1) {
		INFO("Launching xmms2d");
		if (system("xmms2-launcher") == 0) {
			ecore_job_add( (void(*)(void*))xmms2_reconnect_cb,
			               (const void*)rdata);
		}
	}
}

void cmd_server_shutdown(rockon_data *rdata) {
	xmmsc_result_t *result;
	if (rdata->connection != NULL) {
		DBG("server QUIT");
		result = xmmsc_quit (rdata->connection);
		xmmsc_result_wait (result);
		xmmsc_result_unref(result);
	}
}

void cmd_mlib_add_media(rockon_data* rdata, const char *path) {
	xmmsc_result_t *result;
	char new_path[PATH_MAX];
	XMMS_CONN_IS_VALID();
	assert(path);

	snprintf(new_path, PATH_MAX, "file://%s", path);

	if (ecore_file_is_dir(path)) {
		result = xmmsc_medialib_import_path (rdata->connection, new_path);
		xmmsc_result_notifier_set (result, check_error, NULL);
		xmmsc_result_unref (result);
	} else {
		result = xmmsc_medialib_add_entry (rdata->connection, new_path);
		xmmsc_result_notifier_set (result, check_error, NULL);
		xmmsc_result_unref (result);
	}
}

void cmd_volume_change(rockon_data* rdata, const char *channel, int volume) {
	xmmsc_result_t *result;
	assert(rdata);

	XMMS_CONN_IS_VALID();
	result = xmmsc_playback_volume_set(rdata->connection, channel, volume);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_volume_change_all(rockon_data* rdata, int volume) {
	Eina_List *l;
	volume_channel *ch;

	EINA_LIST_FOREACH(rdata->volume, l, ch) {
		cmd_volume_change(rdata, ch->name, volume);
	}
}

void cmd_coll_load (rockon_data* rdata, xmmsv_coll_t* coll) {
	xmmsc_result_t* result;
	xmmsv_t *title, *album, *artist, *id, *tracknr, *disc;
	xmmsv_t *order, *fetch, *group = NULL;
	if (coll == NULL) return;

	album = xmmsv_new_string("album");
	artist = xmmsv_new_string("artist");
	title = xmmsv_new_string("title");
	id = xmmsv_new_string("id");
	tracknr = xmmsv_new_string("tracknr");
	disc = xmmsv_new_string("disc");
	order = xmmsv_new_list();
	fetch = xmmsv_new_list();

	xmmsv_list_append(order, artist);
	xmmsv_list_append(order, album);
	xmmsv_list_append(order, disc);
	xmmsv_list_append(order, tracknr);

	xmmsv_list_append(fetch, title);
	xmmsv_list_append(fetch, album);
	xmmsv_list_append(fetch, artist);
	xmmsv_list_append(fetch, id);
	xmmsv_list_append(fetch, tracknr);

	result = xmmsc_coll_query_infos (rdata->connection, coll, order, 0, 0, fetch, group);
	xmmsc_result_notifier_set (result, cmd_coll_load_inner, rdata);
	xmmsc_result_unref (result);

	xmmsv_unref(artist);
	xmmsv_unref(album);
	xmmsv_unref(disc);
	xmmsv_unref(tracknr);
	xmmsv_unref(title);
	xmmsv_unref(id);
	xmmsv_unref(order);
	xmmsv_unref(fetch);
}

int cmd_coll_load_inner (xmmsv_t* value, void* data) {
	xmmsv_list_iter_t *it;
	rockon_data *rdata = (rockon_data*)data;
	
	rockon_data_coll_queried_clean(rdata);

	xmmsv_get_list_iter(value, &it);
	for (; xmmsv_list_iter_valid (it); xmmsv_list_iter_next (it)) {
		xmmsv_t *list_entry;
		playlist_item *pi;
		pi = playlist_item_new();
		xmmsv_list_iter_entry(it, &list_entry);
		xmmsv_dict_foreach (list_entry, cmd_coll_load_inner2, pi);

		rdata->coll_queried = eina_list_append(rdata->coll_queried, pi);
	}

	ui_upd_coll_queried(rdata);

	return 1;
}

void cmd_coll_load_inner2(const char *key, xmmsv_t *value, void *data) {
	playlist_item *pi = (playlist_item*)data;
	const char *buf;
	int number;

	if (strcmp(key, "title") == 0) {
		xmmsv_get_string(value, &buf);
		pi->title = strdup(buf);
	} else if (strcmp(key, "album") == 0) {
		xmmsv_get_string(value, &buf);
		pi->album = strdup(buf);
	} else if (strcmp(key, "artist") == 0) {
		xmmsv_get_string(value, &buf);
		pi->artist = strdup(buf);
	} else if (strcmp(key, "id") == 0) {
		xmmsv_get_int(value, &number);
		pi->id = number;
	} else if (strcmp(key, "tracknr") == 0) {
		xmmsv_get_int(value, &number);
		pi->tracknr = number;
	}
}

void cmd_coll_search(rockon_data* rdata, const char *pattern) {
	if (pattern == NULL) return;

	if (xmmsv_coll_parse(pattern, &(rdata->coll))) {
		cmd_coll_load(rdata, rdata->coll);
	} else {
		INFO("Can't parse pattern: %s", pattern);
	}
}

void cmd_coll_save(rockon_data *rdata, xmmsv_coll_t *coll, const char *name) {
	xmmsc_result_t *result;

	if (name == NULL) return;

	XMMS_CONN_IS_VALID();
	result = xmmsc_coll_save  (rdata->connection, coll, name, XMMS_COLLECTION_NS_COLLECTIONS);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}
