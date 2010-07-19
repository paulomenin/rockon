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

#include "callbacks.h"
#include "commands.h"
#include "ui_update.h"

/* callbacks from elementary widgets */

void elm_cb_prev(void *data, Evas_Object *obj, void *event_info) {
	cmd_prev((rockon_data*)data);
}
void elm_cb_play(void *data, Evas_Object *obj, void *event_info) {
	cmd_play((rockon_data*)data);
}
void elm_cb_pause(void *data, Evas_Object *obj, void *event_info) {
	cmd_pause((rockon_data*)data);
}
void elm_cb_stop(void *data, Evas_Object *obj, void *event_info) {
	cmd_stop((rockon_data*)data);
}
void elm_cb_next(void *data, Evas_Object *obj, void *event_info) {
	cmd_next((rockon_data*)data);
}

void elm_cb_coll_search (void *data, Evas_Object *obj, void *event_info) {
	edje_cb_coll_search(data, NULL, NULL, NULL);
}

void elm_cb_coll_load (void *data, Evas_Object *obj, void *event_info) {
	rockon_data *rdata = (rockon_data*)data;
	collection *coll;
	Elm_List_Item *it = elm_list_selected_item_get(obj);

	coll = (collection*) elm_list_item_data_get(it);
	rdata->coll = coll->coll;
	xmmsv_coll_ref(rdata->coll);
	cmd_coll_load((rockon_data*)data, rdata->coll);
}

void seekbar_drag_start_cb(void *data, Evas_Object *obj, void *event_info) {
	((rockon_data*)data)->widgets.seekbar_update = 0;
}

void seekbar_drag_stop_cb(void *data, Evas_Object *obj, void *event_info) {
	int seconds = (int)elm_slider_value_get(obj);
	cmd_seek_ms((rockon_data*)data, seconds*1000);
	((rockon_data*)data)->widgets.seekbar_update = 1;
}

void volumebar_drag_start_cb(void *data, Evas_Object *obj, void *event_info) {
	((rockon_data*)data)->widgets.volumebar_update = 0;
}

void volumebar_drag_stop_cb(void *data, Evas_Object *obj, void *event_info) {
	int volume = (int)elm_slider_value_get(obj);
	cmd_volume_change_all((rockon_data*)data, volume);
	((rockon_data*)data)->widgets.volumebar_update = 1;
}

void playlist_list_click_cb(void *data, Evas_Object *obj, void *event_info) {
	Elm_List_Item *it = elm_list_selected_item_get(obj);
	cmd_playlist_load((rockon_data*)data, elm_list_item_label_get(it));
}

void playlist_list_selected_cb (void *data, Evas_Object *obj, void *event_info) {
	rockon_data* rdata = (rockon_data*)data;
	Elm_List_Item *it = elm_list_selected_item_get(obj);
	const char *name;
	if (it == NULL) {
		ui_upd_playlist_edit(rdata, NULL);
	} else {
		playlist *pls;
		name = elm_list_item_label_get(it);
		pls = playlist_find(rdata->playlists, name);
		if (! playlist_is_fetched(pls)) {
			playlist_get(rdata->connection, pls, rdata);
			playlist_wait(pls);
		}
		ui_upd_playlist_edit(rdata, pls);
	}
}

void playlist_click_cb(void *data, Evas_Object *obj, void *event_info){
	playlist_item *item;
	Elm_List_Item *it = elm_list_selected_item_get(obj);
	item = (playlist_item*) elm_list_item_data_get(it);
	cmd_jump_and_play((rockon_data*)data, item->pos);
}

void coll_search_click_cb(void *data, Evas_Object *obj, void *event_info){
	playlist_item *item;
	Elm_List_Item *it = elm_list_selected_item_get(obj);
	item = (playlist_item*) elm_list_item_data_get(it);
	cmd_playlist_add((rockon_data*)data, item->id);
}

/* callbacks from edje objects */

void edje_cb_play  (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_play((rockon_data*)data);
}

void edje_cb_pause (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_pause((rockon_data*)data);
}

void edje_cb_stop (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_stop((rockon_data*)data);
}

void edje_cb_next (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_next((rockon_data*)data);
}

void edje_cb_prev (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_prev((rockon_data*)data);
}

void edje_cb_pls_new (void *data, Evas_Object *eo, const char *emission, const char *source) {
	rockon_data *rdata = (rockon_data*) data;
	const char *name = elm_scrolled_entry_entry_get(rdata->widgets.playlist_name);
	if (name[0] == '_') {
		return;
	}
	cmd_playlist_create((rockon_data*)data, name);
}

void edje_cb_pls_delete (void *data, Evas_Object *eo, const char *emission, const char *source) {
	rockon_data* rdata = (rockon_data*)data;
	Elm_List_Item *it = elm_list_selected_item_get(rdata->widgets.playlists);
	const char *name;
	if (it == NULL) return;
	name = elm_list_item_label_get(it);
	cmd_playlist_delete(rdata, name);
}

void edje_cb_pls_remove_entry(void *data, Evas_Object *eo, const char *emission, const char *source) {
	rockon_data* rdata = (rockon_data*)data;
	playlist_item *pi;
	Elm_List_Item *it_list = elm_list_selected_item_get(rdata->widgets.playlists);
	Elm_List_Item *it_edit = elm_list_selected_item_get(rdata->widgets.playlist_edit);
	const char *name;
	if ((it_list == NULL) || (it_edit == NULL)) return;
	name = elm_list_item_label_get(it_list);
	pi = (playlist_item*) elm_list_item_data_get(it_edit);
	cmd_playlist_remove_item(rdata, name, pi->pos);
}

void edje_cb_pls_load (void *data, Evas_Object *eo, const char *emission, const char *source) {
	rockon_data* rdata = (rockon_data*)data;
	Elm_List_Item *it = elm_list_selected_item_get(rdata->widgets.playlists);
	const char *name;
	if (it == NULL) return;
	name = elm_list_item_label_get(it);
	cmd_playlist_load(rdata, name);
}

void edje_cb_coll_search (void *data, Evas_Object *eo, const char *emission, const char *source){
	rockon_data *rdata = (rockon_data*) data;
	const char *pattern = elm_scrolled_entry_entry_get(rdata->widgets.coll_entry);
	cmd_coll_search(rdata, pattern);
}
void edje_cb_coll_save (void *data, Evas_Object *eo, const char *emission, const char *source){
	rockon_data *rdata = (rockon_data*) data;
	const char *name = elm_scrolled_entry_entry_get(rdata->widgets.coll_name);
	if (rdata->coll) {
		cmd_coll_save(rdata, rdata->coll, name);
	}
}
void edje_cb_coll_add_to_playlist (void *data, Evas_Object *eo, const char *emission, const char *source) {
	rockon_data *rdata = (rockon_data*) data;
	cmd_playlist_add_coll(rdata, rdata->coll);
}
