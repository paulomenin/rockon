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

#include "gui_window.h"
#include "gui_callbacks.h"
#include "gui_widgets.h"
#include "gui_update.h"

#define DBG(...) EINA_LOG_DOM_DBG(gui_window_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(gui_window_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(gui_window_log_dom, __VA_ARGS__)
#define CRIT(...) EINA_LOG_DOM_CRIT(gui_window_log_dom, __VA_ARGS__)

extern int gui_window_log_dom;

void window_del_cb (void *data, Evas_Object *obj, void *event_info) {
	server_data *sdata = (server_data*)data;
	Eina_List *win;

	win = window_find_by_win(sdata->windows, obj);

	if (win != NULL) {
		clean_widgets(sdata->widgets, (rockon_window*)win);
		free(((rockon_window*)win)->name);
		free((rockon_window*)win);
		sdata->windows = eina_list_remove(sdata->windows, win);
	}

	if (eina_list_count(sdata->windows) == 0) {
		DBG("Number of windows: 0");
		app_close_cb(NULL, NULL, NULL, NULL);
	}
}

void window_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	server_data *sdata = (server_data*)data;
	Eina_List *win;

	win = window_find_by_edje(sdata->windows, eo);

	if (win != NULL) {
		evas_object_del(((rockon_window*)win)->elm_win);
		window_del_cb(sdata, ((rockon_window*)win)->elm_win, NULL);
	}
}

void app_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	elm_exit();
}

void* window_find_by_win(const Eina_List *list, const void *data) {
	const Eina_List *l;
	void *list_data;

	if (list == NULL) return NULL;

	EINA_LIST_FOREACH(list, l, list_data) {
		if (((rockon_window*)list_data)->elm_win == data)
			return (Eina_List *)list_data;
	}

	return NULL;
}

void* window_find_by_edje(const Eina_List *list, const void *data) {
	const Eina_List *l;
	void *list_data;

	if (list == NULL) return NULL;

	EINA_LIST_FOREACH(list, l, list_data) {
		if (((rockon_window*)list_data)->edje_obj == data)
			return (Eina_List *)list_data;
	}

	return NULL;
}

void window_new(server_data *sdata, const char *emission) {
	rockon_window *win = NULL;

	win = (rockon_window*) malloc(sizeof(rockon_window));
	if (win == NULL)
		CRIT("Memory allocation error.");

	win->elm_win = elm_win_add(NULL, emission, ELM_WIN_BASIC);
	elm_win_title_set(win->elm_win, emission);
	elm_win_autodel_set(win->elm_win, 1);

	evas_object_smart_callback_add(win->elm_win, "delete-request", window_del_cb, sdata);

	win->elm_layout = elm_layout_add(win->elm_win);
	INFO("Load Theme: %s", sdata->config->theme_path);
	if (!elm_layout_file_set(win->elm_layout, (const char*)sdata->config->theme_path, emission))
		ERR("Cannot load theme.");
	evas_object_size_hint_weight_set(win->elm_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win->elm_win, win->elm_layout);
	evas_object_show(win->elm_layout);
	win->edje_obj = elm_layout_edje_get(win->elm_layout);

	win->name = strdup(emission);

	sdata->windows = eina_list_append(sdata->windows, win);
	DBG("window address: %p",win);
	DBG("elmWin address: %p",win->elm_win);
	DBG("layout address: %p",win->elm_layout);
	DBG("edje   address: %p",win->edje_obj);

	/* set callbacks */

	edje_object_signal_callback_add (win->edje_obj, "*", "elm_set,*", elm_cb_set, (void*)sdata);
	edje_object_signal_callback_add (win->edje_obj, "win,close", "*", window_close_cb, (void*)sdata);
	edje_object_signal_callback_add (win->edje_obj, "app,close", "*", app_close_cb, (void*)sdata);
	edje_object_signal_callback_add (win->edje_obj, "cmd,play", "*", edje_cb_play, (void*)sdata);
	edje_object_signal_callback_add (win->edje_obj, "cmd,pause", "*",edje_cb_pause, (void*)sdata);
	edje_object_signal_callback_add (win->edje_obj, "cmd,stop", "*", edje_cb_stop, (void*)sdata);
	edje_object_signal_callback_add (win->edje_obj, "cmd,next", "*", edje_cb_next, (void*)sdata);
	edje_object_signal_callback_add (win->edje_obj, "cmd,prev", "*", edje_cb_prev, (void*)sdata);

	gui_upd_playback_id(sdata);
	gui_upd_playback_status(sdata);
	gui_upd_playback_playtime(sdata);
	gui_upd_playback_info(sdata);

	evas_object_show(win->elm_win);
}
