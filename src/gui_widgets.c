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

#include "gui_widgets.h"
#include "gui_callbacks.h"
#include "gui_update.h"

#define DBG(...) EINA_LOG_DOM_DBG(gui_widgets_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(gui_widgets_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(gui_widgets_log_dom, __VA_ARGS__)
#define CRIT(...) EINA_LOG_DOM_CRIT(gui_widgets_log_dom, __VA_ARGS__)

extern int gui_widgets_log_dom;

void clean_widgets(widgets_list *widgets, rockon_window *window) {
	Eina_List *l, *l_next;
	void *data;

	DBG("clean widgets");
	if (widgets == NULL) return;

	EINA_LIST_FOREACH_SAFE(widgets->seekbars, l, l_next, data) {
		if (((widget*)data)->window == window) {
			widgets->seekbars = eina_list_remove_list(widgets->seekbars, l);
			free((widget*)data);
		}
	}
	EINA_LIST_FOREACH_SAFE(widgets->playlist_lists, l, l_next, data) {
		if (((widget*)data)->window == window) {
			widgets->playlist_lists = eina_list_remove_list(widgets->playlist_lists, l);
			free((widget*)data);
		}
	}
	EINA_LIST_FOREACH_SAFE(widgets->playlists, l, l_next, data) {
		if (((widget*)data)->window == window) {
			widgets->playlists = eina_list_remove_list(widgets->playlists, l);
			free((widget*)data);
		}
	}
}

const char* seekbar_format_indicator(double val) {
	char indicator[8];
	int minutes = (int)val/60;
	int seconds = (int)val%60;
	if (seconds < 10) {
		snprintf(indicator, 8, "%d:0%d", minutes, seconds);
	} else {
		snprintf(indicator, 8, "%d:%d", minutes, seconds);
	}
	return strdup(indicator);
}

void gui_seekbar_new (server_data *sdata, const char *emission, rockon_window *window) {
	widget* obj;
	Evas_Object *seekbar;

	obj = (widget*) malloc(sizeof(widget));
	if (obj == NULL) {
		EINA_LOG_CRIT("seekbar malloc failed");
	}

	seekbar = elm_slider_add(window->elm_win);
	elm_slider_indicator_format_function_set(seekbar, seekbar_format_indicator);
	elm_layout_content_set(window->elm_layout, emission, seekbar);
	evas_object_smart_callback_add(seekbar, "slider,drag,start", seekbar_drag_start_cb, sdata);
	evas_object_smart_callback_add(seekbar, "slider,drag,stop", seekbar_drag_stop_cb, sdata);
	evas_object_show(seekbar);

	obj->widget = seekbar;
	obj->window = window;
	obj->update = 1;
	sdata->widgets->seekbars = eina_list_append(sdata->widgets->seekbars, obj);
}

void gui_playlist_list_new (server_data *sdata, const char *emission, rockon_window *window) {
	widget* obj;
	Evas_Object *plist;

	obj = (widget*) malloc(sizeof(widget));
	if (obj == NULL) {
		EINA_LOG_CRIT("gui playlist_list malloc failed");
	}

	plist = elm_list_add(window->elm_win);
	elm_layout_content_set(window->elm_layout, emission, plist);
	evas_object_smart_callback_add(plist, "clicked", playlist_list_click_cb, sdata);
	evas_object_show(plist);

	obj->widget = plist;
	obj->window = window;
	obj->update = 1;
	sdata->widgets->playlist_lists = eina_list_append(sdata->widgets->playlist_lists, obj);

	gui_upd_playlist_list(sdata);
}

void gui_playlist_new (server_data *sdata, const char *emission, rockon_window *window) {
	widget* obj;
	Evas_Object *plist;

	obj = (widget*) malloc(sizeof(widget));
	if (obj == NULL) {
		EINA_LOG_CRIT("gui playlist malloc failed");
	}

	plist = elm_list_add(window->elm_win);
	elm_layout_content_set(window->elm_layout, emission, plist);
	evas_object_smart_callback_add(plist, "clicked", playlist_click_cb, sdata);
	evas_object_show(plist);

	obj->widget = plist;
	obj->window = window;
	obj->update = 1;
	sdata->widgets->playlists = eina_list_append(sdata->widgets->playlists, obj);

	gui_upd_playlist(sdata);
}
