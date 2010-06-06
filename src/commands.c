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
#include <xmmsclient/xmmsclient.h>
#include "xmms_conn.h"
#include "commands.h"

#define DBG(...) EINA_LOG_DOM_DBG(cmd_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(cmd_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(cmd_log_dom, __VA_ARGS__)

extern int cmd_log_dom;

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

void cmd_playlist_load(rockon_data* rdata, const char *playlist) {
	xmmsc_result_t *result;
	assert(rdata);
	if (playlist == NULL) return;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_load(rdata->connection, playlist);
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

