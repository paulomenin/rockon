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
#include "config.h"

typedef struct {
	char 	    *name;
	Evas_Object *elm_win;
	Evas_Object *elm_layout;
	Evas_Object *edje_obj;
} rockon_window;

typedef struct {
	int connected;
	xmmsc_connection_t *connection;
	rockon_config *config;

	Eina_List *windows;

	char *playlist_name;
	int playlist_pos;

	int playback_status;
	int playback_id;
	int volume_left;
	int volume_right;
	
	int playtime;

} xmms_status;

void status_free(xmms_status *status);
void status_fetch(xmms_status *status);
void status_gui_update(xmms_status *status);
void* status_find_window_by_win(const Eina_List *list, const void *data);
void* status_find_window_by_edje(const Eina_List *list, const void *data);

#endif /* STATUS_H */
