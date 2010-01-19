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
#include <xmmsclient/xmmsclient-ecore.h>
#include "xmms_conn.h"
#include "broadcasts.h"

#define DBG(...) EINA_LOG_DOM_DBG(conn_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(conn_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(conn_log_dom, __VA_ARGS__)

extern int conn_log_dom;

int  xmms2_connect (server_data *sdata) {
	char *xmms_path = NULL;
	char *xmms_error = NULL;
	assert(sdata);

	sdata->connection = xmmsc_init ("rockon");
	if (! sdata->connection) {
		ERR("Couldn't create XMMS2 connection.");
		return 0;
	}

	xmms_path = getenv("XMMS_PATH");
	DBG("XMMS_PATH %s", xmms_path);
	if (xmms_path == NULL) {
		xmms_path = sdata->config->ipc_path;
	}

	if (!xmmsc_connect (sdata->connection, xmms_path)) {
		INFO("Connection to XMMS2 failed.");
		xmms_error = xmmsc_get_last_error(sdata->connection);
		INFO(xmms_error);
		if (sdata->config->auto_reconnect == 1) {
			xmms2_disconnect_cb((void*)sdata);
		}
		return 0;
	}

	xmmsc_disconnect_callback_set (sdata->connection,
	                               (void(*)(void*))xmms2_disconnect_cb,
	                               sdata);

	XMMS_CALLBACK_SET (sdata->connection,
					xmmsc_broadcast_playback_current_id,
					broadcast_playback_id_cb, sdata);
	XMMS_CALLBACK_SET (sdata->connection,
					xmmsc_broadcast_playback_status,
					broadcast_playback_status_cb, sdata);
	XMMS_CALLBACK_SET (sdata->connection,
					xmmsc_signal_playback_playtime,
					signal_playback_playtime_cb, sdata);
	XMMS_CALLBACK_SET (sdata->connection,
					xmmsc_broadcast_playlist_current_pos,
					broadcast_playlist_pos_cb, sdata);
	XMMS_CALLBACK_SET (sdata->connection,
					xmmsc_broadcast_playlist_loaded,
					broadcast_playlist_loaded_cb, sdata);
	XMMS_CALLBACK_SET (sdata->connection,
					xmmsc_broadcast_playlist_changed,
					broadcast_playlist_changed_cb, sdata);

	sdata->ecore_fdh = xmmsc_mainloop_ecore_init (sdata->connection);
	DBG("Ecore_fdh: %p", sdata->ecore_fdh);
	DBG("XMMS2 Connected");
	xmms2_get_status(sdata);
	return 1;
}

int xmms2_reconnect_cb (server_data *sdata) {
	assert(sdata);

	if (sdata->connection != NULL) {
		sdata->reconn_timer = NULL;
		return ECORE_CALLBACK_CANCEL;
	}
	DBG("try to reconnect");
	if (xmms2_connect(sdata)) {
		sdata->reconn_timer = NULL;
		return ECORE_CALLBACK_CANCEL;
	}
	return ECORE_CALLBACK_RENEW;
}

void xmms2_disconnect_cb (server_data *sdata) {
	INFO("xmms2 connection lost.");
	xmms2_shutdown(sdata);
	if (sdata->config->auto_reconnect == 1) {
		if (sdata->reconn_timer == NULL) {
			sdata->reconn_timer = ecore_timer_add (
			             sdata->config->reconnect_interval,
			             (int(*)(void*))xmms2_reconnect_cb,
			             sdata);
		}
	}
}

void xmms2_shutdown(server_data *sdata) {
	if ((sdata) && (sdata->connection)) {
		if (sdata->ecore_fdh != NULL) {
			DBG("Ecore_fdh: %p", sdata->ecore_fdh);
			xmmsc_mainloop_ecore_shutdown(sdata->connection, sdata->ecore_fdh);
			sdata->ecore_fdh = NULL;
		}
		xmmsc_unref (sdata->connection);
		sdata->connection = NULL;
		DBG("XMMS2 connection shutdown");
	}
}

int  check_error (xmmsv_t *value, void *data) {
	const char *err_buf;
	if ( (value) && xmmsv_is_error (value) &&
		xmmsv_get_error (value, &err_buf)) {
		ERR(err_buf);
		return 1;
	}
	return 0;
}

void xmms2_get_status (server_data *sdata) {
	xmmsc_result_t *res;
	assert(sdata);
	DBG("--------- GET STATUS ---------");

	if (sdata->connection == NULL) return;

	XMMS_CALLBACK_SET (sdata->connection,
					xmmsc_playback_current_id,
					broadcast_playback_id_cb, sdata);
	XMMS_CALLBACK_SET (sdata->connection,
					xmmsc_playback_status,
					broadcast_playback_status_cb, sdata);
	XMMS_CALLBACK_SET (sdata->connection,
					xmmsc_playback_playtime,
					signal_playback_playtime_cb, sdata);

	if (sdata->playlists != NULL) {
		playlist_list_del(sdata->playlists);
	}
	sdata->playlists = playlist_list_get(sdata->connection, sdata);

	if (sdata->playlist_current != NULL) {
		playlist_wait(sdata->playlist_current);
		playlist_del(sdata->playlist_current);
	}
	sdata->playlist_current = playlist_get(sdata->connection, "_active", sdata);

	res = xmmsc_playlist_current_pos(sdata->connection, sdata->playlist_current->name);
	xmmsc_result_notifier_set_full(res, broadcast_playlist_pos_cb, sdata, NULL);
	xmmsc_result_unref(res);

	DBG("------------------------------");
}
