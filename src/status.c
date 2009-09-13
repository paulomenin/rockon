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

#include "status.h"
#include "xmms_conn.h"

void _print_status(xmms_status *status);

void status_free(xmms_status *status) {
	if (status->playlist_name)
		free (status->playlist_name);
}

void status_fetch(xmms_status *status) {
	xmmsc_result_t *res;

	if ( ! status->connected)
		return;
	
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_playback_current_id,
					broadcast_playback_id_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_playback_volume_get,
					broadcast_playback_volume_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_playback_status,
					broadcast_playback_status_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_playback_playtime,
					signal_playback_playtime_cb, status);

	res = xmmsc_playlist_current_pos(status->connection, status->playlist_name);
	xmmsc_result_notifier_set_full(res, broadcast_playlist_pos_cb, status, NULL);
	xmmsc_result_unref(res);

}

void status_gui_update(xmms_status *status) {
	_print_status(status);
}

void _print_status(xmms_status *status) {

	printf("======== STATUS ========\n");
	printf("Playback Status: %d\n", status->playback_status);
	printf("Volume: L %d R %d\n",status->volume_left,status->volume_right);
	printf("Playback ID: %d\n", status->playback_id);
	printf("Playtime: %d:%02d\n", status->playtime/60000, (status->playtime%60000)/1000);
	printf("Playlist: %s POS: %d\n",status->playlist_name,status->playlist_pos);
	printf("====== STATUS END ======\n");
	fflush(stdout);
}
