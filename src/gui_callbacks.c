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

#include "server_data.h"
#include "gui_callbacks.h"
#include "commands.h"
#include "gui_window.h"
#include "gui_widgets.h"

void edje_cb_play  (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_play((server_data*)data);
}

void edje_cb_pause (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_pause((server_data*)data);
}

void edje_cb_stop (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_stop((server_data*)data);
}

void edje_cb_next (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_next((server_data*)data);
}

void edje_cb_prev (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_prev((server_data*)data);
}

void elm_cb_set (void *data, Evas_Object *eo, const char *emission, const char *source) {
	server_data *sdata = (server_data*)data;
	Eina_List *win = NULL;

	if (strcmp(source, "elm_set,window") == 0) {
		window_new(sdata, emission);
	} else
	if (strcmp(source, "elm_set,seek_bar") == 0) {
		win = window_find_by_edje(sdata->windows, eo);
		if (win != NULL) {
			gui_seekbar_new(sdata, emission, (rockon_window*)win);
		}
	} else
	if (strcmp(source, "elm_set,playlist_list") == 0) {
		win = window_find_by_edje(sdata->windows, eo);
		if (win != NULL) {
			gui_playlist_list_new(sdata, emission, (rockon_window*)win);
		}
	} else
	if (strcmp(source, "elm_set,playlist") == 0) {
		win = window_find_by_edje(sdata->windows, eo);
		if (win != NULL) {
			gui_playlist_new(sdata, emission, (rockon_window*)win);
		}
	} else
	if (strcmp(source, "elm_set,mlib_file_selector") == 0) {
		win = window_find_by_edje(sdata->windows, eo);
		if (win != NULL) {
			gui_mlib_file_selector_new(sdata, emission, (rockon_window*)win);
		}
	}

}

void seekbar_drag_start_cb(void *data, Evas_Object *obj, void *event_info) {
	Eina_List *l;
	void *widgt;

	EINA_LIST_FOREACH (((server_data*)data)->widgets->seekbars, l, widgt) {
		if (((widget*)widgt)->widget == obj) {
			((widget*)widgt)->update = 0;
			break;
		}
	}

}

void seekbar_drag_stop_cb(void *data, Evas_Object *obj, void *event_info) {
	Eina_List *l;
	void *widgt;
	int seconds = (int)elm_slider_value_get(obj);

	cmd_seek_ms((server_data*)data, seconds*1000);

	EINA_LIST_FOREACH (((server_data*)data)->widgets->seekbars, l, widgt) {
		if (((widget*)widgt)->update == 0) {
			((widget*)widgt)->update = 1;
		}
	}
}

void playlist_list_click_cb(void *data, Evas_Object *obj, void *event_info){
	Elm_List_Item *it = elm_list_selected_item_get(obj);

	cmd_playlist_load((server_data*)data, elm_list_item_label_get(it));
}

void playlist_click_cb(void *data, Evas_Object *obj, void *event_info){
	playlist_item *item;
	Elm_List_Item *it = elm_list_selected_item_get(obj);
	item = (playlist_item*) elm_list_item_data_get(it);
	cmd_jump_and_play((server_data*)data, item->pos);
}

void mlib_file_selector_done_cb(void *data, Evas_Object *obj, void *event_info) {
	const char *selected = event_info;

	if (selected) { // ok button clicked
		cmd_mlib_add_media((server_data*)data, selected);
	} else { // cancel button clicked
		widget *wid;
		wid = widget_find_by_widget( ((server_data*)data)->widgets->file_selectors, obj);
		if (wid) {
			Evas_Object *window = (Evas_Object*)((rockon_window*)(wid->window)->elm_win);
			evas_object_del(window);
			window_del_cb (data, window, NULL);
		}
	}

}
