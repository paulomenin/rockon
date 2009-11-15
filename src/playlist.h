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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <Elementary.h>
#include "status.h"
#include "elm_bridge.h"

typedef struct {
	int id;
	int pos;
	rockon_status* status; //needed to get access to theme
	char *title;
	char *artist;
	char *album;
} playlist_item;

void set_playlist(rockon_status *s, const char *emission, Evas_Object *eo);
void pls_item_class_set(rockon_status *s);
char *pls_item_label_get(const void *data, Evas_Object *obj, const char *part);
Evas_Object *pls_item_icon_get(const void *data, Evas_Object *obj, const char *part);
Eina_Bool pls_item_state_get(const void *data, Evas_Object *obj, const char *part);
void pls_item_del(const void *data, Evas_Object *obj);
void pls_item_sel(void *data, Evas_Object *obj, void *event_info);

void pls_populate(rockon_window *win, rockon_status *s);
void pls_update_playback_pos(rockon_window *win);
void pls_free(rockon_status* s);

#endif /* PLAYLIST_H */
