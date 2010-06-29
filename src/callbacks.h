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

#include <Elementary.h>

void elm_cb_prev(void *data, Evas_Object *obj, void *event_info);
void elm_cb_play(void *data, Evas_Object *obj, void *event_info);
void elm_cb_pause(void *data, Evas_Object *obj, void *event_info);
void elm_cb_stop(void *data, Evas_Object *obj, void *event_info);
void elm_cb_next(void *data, Evas_Object *obj, void *event_info);
void seekbar_drag_start_cb(void *data, Evas_Object *obj, void *event_info);
void seekbar_drag_stop_cb(void *data, Evas_Object *obj, void *event_info);
void toolbar_playlist_click_cb(void *data, Evas_Object *obj, void *event_info);
