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
#include "ui_update.h"
#include "util.h"

#define DBG(...) EINA_LOG_DOM_DBG(conn_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(conn_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(conn_log_dom, __VA_ARGS__)

extern int conn_log_dom;

int  xmms2_connect (rockon_data *rdata) {
	char *xmms_path = NULL;
	char *xmms_error = NULL;
	assert(rdata);

	rdata->connection = xmmsc_init ("rockon");
	if (! rdata->connection) {
		ERR("Couldn't create XMMS2 connection.");
		return 0;
	}

	xmms_path = getenv("XMMS_PATH");
	DBG("XMMS_PATH %s", xmms_path);
	if (xmms_path == NULL) {
		xmms_path = rdata->config->ipc_path;
	}

	if (!xmmsc_connect (rdata->connection, xmms_path)) {
		xmms_error = xmmsc_get_last_error(rdata->connection);
		INFO("Connection to XMMS2 failed: %s", xmms_error);
		xmmsc_unref(rdata->connection);
		rdata->connection = NULL;
		if (rdata->config->auto_reconnect == 1) {
			xmms2_disconnect_cb((void*)rdata);
		}
		ui_upd_disconnect(rdata);
		return 0;
	}

	xmmsc_disconnect_callback_set (rdata->connection,
	                               (void(*)(void*))xmms2_disconnect_cb,
	                               rdata);

	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_broadcast_playback_current_id,
					broadcast_playback_id_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_broadcast_playback_status,
					broadcast_playback_status_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_signal_playback_playtime,
					signal_playback_playtime_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_broadcast_playlist_current_pos,
					broadcast_playlist_pos_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_broadcast_playlist_loaded,
					broadcast_playlist_loaded_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_broadcast_playlist_changed,
					broadcast_playlist_changed_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_broadcast_mediainfo_reader_status,
					mlib_reader_status_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_signal_mediainfo_reader_unindexed,
					mlib_reader_unindexed_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_broadcast_playback_volume_changed,
					broadcast_playback_volume_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_broadcast_collection_changed,
					broadcast_collection_changed_cb, rdata);

	rdata->ecore_fdh = xmmsc_mainloop_ecore_init (rdata->connection);
	DBG("Ecore_fdh: %p", rdata->ecore_fdh);
	DBG("XMMS2 Connected %p", rdata->connection);
	xmms2_get_status(rdata);
	ui_upd_connect(rdata);
	return 1;
}

int xmms2_reconnect_cb (rockon_data *rdata) {
	assert(rdata);

	if (rdata->connection != NULL) {
		rdata->reconn_timer = NULL;
		return ECORE_CALLBACK_CANCEL;
	}
	DBG("try to reconnect");
	if (xmms2_connect(rdata)) {
		rdata->reconn_timer = NULL;
		return ECORE_CALLBACK_CANCEL;
	}
	return ECORE_CALLBACK_RENEW;
}

void xmms2_disconnect_cb (rockon_data *rdata) {
	ui_upd_disconnect(rdata);
	xmms2_shutdown(rdata);
	if (rdata->config->auto_reconnect == 1) {
		if (rdata->reconn_timer == NULL) {
			rdata->reconn_timer = ecore_timer_add (
			             rdata->config->reconnect_interval,
			             (int(*)(void*))xmms2_reconnect_cb,
			             rdata);
		}
	}
}

void xmms2_shutdown(rockon_data *rdata) {
	if ((rdata) && (rdata->connection)) {
		if (rdata->ecore_fdh != NULL) {
			DBG("Ecore_fdh: %p", rdata->ecore_fdh);
			xmmsc_mainloop_ecore_shutdown(rdata->connection, rdata->ecore_fdh);
			rdata->ecore_fdh = NULL;
		}
		DBG("xmmsc_unref connection %p", rdata->connection);
		xmmsc_unref (rdata->connection);
		rdata->connection = NULL;
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

void xmms2_get_status (rockon_data *rdata) {
	xmmsc_result_t *res;

	assert(rdata);
	if (rdata->connection == NULL) return;

	DBG("--------- GET STATUS ---------");

	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_playback_current_id,
					broadcast_playback_id_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_playback_status,
					broadcast_playback_status_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_playback_playtime,
					signal_playback_playtime_cb, rdata);
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_playback_volume_get,
					broadcast_playback_volume_cb, rdata);

	if (rdata->collections != NULL) {
		coll_list_del(rdata->collections);
	}
	rdata->collections = coll_list_get(rdata->connection, rdata);
	coll_list_wait(rdata->collections);

	if (rdata->playlists != NULL) {
		playlist_list_del(rdata->playlists);
	}
	rdata->playlists = playlist_list_get(rdata->connection, rdata);
	playlist_list_wait(rdata->playlists);

	up_mutex(&(rdata->mutex_playlist));
	XMMS_CALLBACK_SET (rdata->connection,
					xmmsc_playlist_current_active,
					playlist_load_current_cb, rdata);
	wait_mutex(&(rdata->mutex_playlist));
	res = xmmsc_playlist_current_pos(rdata->connection, rdata->current_playlist->name);
	xmmsc_result_notifier_set_full(res, broadcast_playlist_pos_cb, rdata, NULL);
	xmmsc_result_unref(res);
}
