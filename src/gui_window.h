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

#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <Elementary.h>
#include "server_data.h"

typedef struct {
	char *name;
	Evas_Object *elm_win;
	Evas_Object *elm_layout;
	Evas_Object *edje_obj;
} rockon_window;

void window_new      (server_data *sdata, const char *emission);
void window_del_cb   (void *data, Evas_Object *obj, void *event_info);
void window_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source);
void app_close_cb    (void *data, Evas_Object *eo, const char *emission, const char *source);

void* window_find_by_win  (const Eina_List *list, const void *data);
void* window_find_by_edje (const Eina_List *list, const void *data);

#endif /* GUI_WINDOW_H */
