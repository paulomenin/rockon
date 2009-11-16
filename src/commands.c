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

#include "commands.h"

void cmd_play (rockon_status *status) {
	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (status->connection, xmmsc_playback_start,
					check_error, NULL);
}

void cmd_pause (rockon_status *status) {
	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (status->connection, xmmsc_playback_pause,
					check_error, NULL);
}

void cmd_stop (rockon_status *status) {
	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (status->connection, xmmsc_playback_stop,
					check_error, NULL);
}

void cmd_next (rockon_status *status) {
	xmmsc_result_t *result;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_set_next_rel (status->connection, 1);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (status->connection, xmmsc_playback_tickle,
					check_error, NULL);
}

void cmd_prev (rockon_status *status) {
	xmmsc_result_t *result;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_set_next_rel (status->connection, -1);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (status->connection, xmmsc_playback_tickle,
					check_error, NULL);
}

void cmd_jump_to (rockon_status *status, int pos) {
	xmmsc_result_t *result;
	XMMS_CONN_IS_VALID();
	result = xmmsc_playlist_set_next (status->connection, pos);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (status->connection, xmmsc_playback_tickle,
					check_error, NULL);
}
