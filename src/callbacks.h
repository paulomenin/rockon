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
#include "rockon_data.h"

void elm_cb_prev (void *data, Evas_Object *obj, void *event_info);
void elm_cb_play (void *data, Evas_Object *obj, void *event_info);
void elm_cb_pause(void *data, Evas_Object *obj, void *event_info);
void elm_cb_stop (void *data, Evas_Object *obj, void *event_info);
void elm_cb_next (void *data, Evas_Object *obj, void *event_info);
void elm_cb_coll_search (void *data, Evas_Object *obj, void *event_info);
void elm_cb_coll_load (void *data, Evas_Object *obj, void *event_info);
void seekbar_drag_start_cb  (void *data, Evas_Object *obj, void *event_info);
void seekbar_drag_stop_cb   (void *data, Evas_Object *obj, void *event_info);
void volumebar_drag_start_cb(void *data, Evas_Object *obj, void *event_info);
void volumebar_drag_stop_cb (void *data, Evas_Object *obj, void *event_info);
void playlist_list_click_cb (void *data, Evas_Object *obj, void *event_info);
void playlist_list_selected_cb (void *data, Evas_Object *obj, void *event_info);
void playlist_click_cb(void *data, Evas_Object *obj, void *event_info);
void coll_search_click_cb(void *data, Evas_Object *obj, void *event_info);

void edje_cb_connect (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_play (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_pause(void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_stop (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_next (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_prev (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_pls_new (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_pls_delete (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_pls_remove_entry(void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_pls_load (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_pls_clear (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_coll_search (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_coll_save (void *data, Evas_Object *eo, const char *emission, const char *source);
void edje_cb_coll_add_to_playlist (void *data, Evas_Object *eo, const char *emission, const char *source);
