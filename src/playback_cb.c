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

#include "playback_cb.h"

void cmd_play_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (((xmms_status*)data)->connection,
					xmmsc_playback_start,
					check_error, NULL);
}

void cmd_pause_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (((xmms_status*)data)->connection,
					xmmsc_playback_pause,
					check_error, NULL);
}

void cmd_stop_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (((xmms_status*)data)->connection,
					xmmsc_playback_stop,
					check_error, NULL);
}

void cmd_next_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	XMMS_CONN_IS_VALID();
	xmmsc_result_t *result;
	result = xmmsc_playlist_set_next_rel (((xmms_status*)data)->connection, 1);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (((xmms_status*)data)->connection, xmmsc_playback_tickle,
					check_error, NULL);
}

void cmd_prev_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	XMMS_CONN_IS_VALID();
	xmmsc_result_t *result;
	result = xmmsc_playlist_set_next_rel (((xmms_status*)data)->connection, -1);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (((xmms_status*)data)->connection, xmmsc_playback_tickle, check_error, NULL);
}
