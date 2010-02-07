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
#include <xmmsclient/xmmsclient.h>
#include "xmms_conn.h"
#include "commands.h"

#define DBG(...) EINA_LOG_DOM_DBG(cmd_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(cmd_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(cmd_log_dom, __VA_ARGS__)

extern int cmd_log_dom;

void cmd_play (server_data *sdata) {
	assert(sdata);

	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (sdata->connection, xmmsc_playback_start,
					check_error, NULL);
}

void cmd_pause (server_data *sdata) {
	assert(sdata);

	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (sdata->connection, xmmsc_playback_pause,
					check_error, NULL);
}

void cmd_stop (server_data *sdata) {
	assert(sdata);

	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (sdata->connection, xmmsc_playback_stop,
					check_error, NULL);
}

void cmd_next (server_data *sdata) {
	assert(sdata);

	xmmsc_result_t *result;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_set_next_rel (sdata->connection, 1);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (sdata->connection, xmmsc_playback_tickle,
					check_error, NULL);
}

void cmd_prev (server_data *sdata) {
	assert(sdata);

	xmmsc_result_t *result;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_set_next_rel (sdata->connection, -1);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (sdata->connection, xmmsc_playback_tickle,
					check_error, NULL);
}

void cmd_seek_ms (server_data *sdata, int miliseconds) {
	xmmsc_result_t *result;
	assert(sdata);

	XMMS_CONN_IS_VALID();
	result = xmmsc_playback_seek_ms(sdata->connection, miliseconds, XMMS_PLAYBACK_SEEK_SET);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_playlist_load(server_data* sdata, const char *playlist) {
	xmmsc_result_t *result;
	assert(sdata);
	if (playlist == NULL) return;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_load(sdata->connection, playlist);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_jump_to (server_data *sdata, int pos) {
	assert(sdata);
	assert(pos >= 0);

	xmmsc_result_t *result;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_set_next (sdata->connection, pos);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
}

void cmd_jump_and_play (server_data *sdata, int pos) {
	assert(sdata);
	assert(pos >= 0);

	cmd_jump_to(sdata, pos);
	if (sdata->playback_status == 1) {
		XMMS_CALLBACK_SET (sdata->connection, xmmsc_playback_tickle,
					check_error, NULL);
	} else {
		cmd_play(sdata);
	}
}

void cmd_server_launch(server_data *sdata) {
	if (sdata->config->launch_server == 1) {
		INFO("Launching xmms2d");
		if (system("xmms2-launcher") == 0) {
			ecore_job_add( (void(*)(void*))xmms2_reconnect_cb,
			               (const void*)sdata);
		}
	}
}

void cmd_server_shutdown(server_data *sdata) {
	xmmsc_result_t *result;
	if (sdata->connection != NULL) {
		DBG("server QUIT");
		result = xmmsc_quit (sdata->connection);
		xmmsc_result_wait (result);
		xmmsc_result_unref(result);
	}
}

