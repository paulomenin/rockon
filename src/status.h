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

#ifndef STATUS_H
#define STATUS_H

#include <Evas.h>
#include <xmmsclient/xmmsclient.h>

typedef struct {
	int connected;
	xmmsc_connection_t *connection;

	Evas_Object *edje_gui;

	char *playlist_name;
	int playlist_pos;

	int playback_id;
	int volume_left;
	int volume_right;
	
	int playtime;

} xmms_status;

void status_gui_update(xmms_status *status);

#endif /* STATUS_H */
/*
char pos[5];
snprintf(pos, 5, "%d", app_status.playback_id);
edje_object_signal_emit(edje_o , pos, "rockon_playback_id");
*/
