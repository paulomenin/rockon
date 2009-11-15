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
#include "elm_bridge.h"

void _set_window(rockon_status *s, const char *emission);

void elm_cb_set (void *data, Evas_Object *eo, const char *emission, const char *source) {
	rockon_status *s = (rockon_status*)data;

printf("ELM_CALLBACK: emmission: %s source: %s\n", emission, source);
	
	if (strcmp(source, "elm_set.win") == 0) {
		_set_window(s, emission);
	} else
	if (strcmp(source, "elm_set.playlist") == 0) {
		set_playlist(s, emission, eo);
	}
}

void elm_cb_play(void *data, Evas_Object *obj, void *event_info) {
	cmd_play ((rockon_status*)data, NULL);
}

void elm_cb_pls_click(void *data, Evas_Object *obj, void *event_info) {
	playlist_item *pi = (playlist_item*) elm_genlist_item_data_get(event_info);
	int pos = pi->pos;
	cmd_jump_to ((rockon_status*)data, NULL, pos);
}


void win_del_cb (void *data, Evas_Object *obj, void *event_info) {
	rockon_status *s = (rockon_status*)data;
	Eina_List *win;

	win = status_find_window_by_win(s->windows, obj);

	if (win != NULL) {
		free(((rockon_window*)win)->name);
		free((rockon_window*)win);
		s->windows = eina_list_remove(s->windows, win);
	}

	if (eina_list_count(s->windows) == 0)
		elm_exit();

}

void win_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	rockon_status *s = (rockon_status*)data;
	Eina_List *win;

	win = status_find_window_by_edje(s->windows, eo);

	if (win != NULL) {
		evas_object_del(((rockon_window*)win)->elm_win);
		win_del_cb(s, ((rockon_window*)win)->elm_win, NULL);
	}
}

void app_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	elm_exit();
}


void _set_window(rockon_status *s, const char *emission) {
	rockon_window *win = NULL;

	win = (rockon_window*) malloc(sizeof(rockon_window));
	if (win == NULL)
		print_error("Memory allocation error.", ERR_CRITICAL);

	win->elm_win = elm_win_add(NULL, emission, ELM_WIN_BASIC);
	elm_win_title_set(win->elm_win, emission);
	elm_win_autodel_set(win->elm_win, 1);
	//elm_win_shaped_set(win->elm_win, 1);
	//elm_win_alpha_set(win->elm_win, 1);

	evas_object_smart_callback_add(win->elm_win, "delete-request", win_del_cb, s);

	win->elm_layout = elm_layout_add(win->elm_win);
	if (!elm_layout_file_set(win->elm_layout, (const char*)s->config->theme, emission))
		print_error("Cannot load theme.", ERR_CRITICAL);
	evas_object_size_hint_weight_set(win->elm_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win->elm_win, win->elm_layout);
	evas_object_show(win->elm_layout);
	win->edje_obj = elm_layout_edje_get(win->elm_layout);

	win->name = strdup(emission);

	s->windows = eina_list_append(s->windows, win);

	//printf("SET_WIN: win: %p elm_win: %p edje_obj: %p \n", win, win->elm_win, win->edje_obj);
	/* set callbacks */

	/* FIXME remove this function call when finish debuging */
	//edje_object_signal_callback_add (main_window->edje_obj, "*", "*", _p_signal, (void*)status);

	edje_object_signal_callback_add (win->edje_obj, "*", "elm_set.*", elm_cb_set, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "win_close", "*", win_close_cb, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "app_close", "*", app_close_cb, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "cmd_play", "*", edje_cb_play, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "cmd_pause", "*",edje_cb_pause, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "cmd_stop", "*", edje_cb_stop, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "cmd_next", "*", edje_cb_next, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "cmd_prev", "*", edje_cb_prev, (void*)s);

	status_fetch(s);

	win->playlist = NULL;

	evas_object_show(win->elm_win);
}
