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

void _set_window(xmms_status *s, const char *emission);

void elm_cb_set (void *data, Evas_Object *eo, const char *emission, const char *source) {
	xmms_status *s = (xmms_status*)data;

printf("ELM_CALLBACK: emmission: %s source: %s\n", emission, source);
	
	if (strcmp(source, "elm_set.win") == 0) {
		_set_window(s, emission);
	} else
	if (strcmp(source, "elm_set.play") == 0) {
		/*s->bt = elm_button_add(s->elm_win);
		elm_button_label_set(s->bt, "Play");
		elm_layout_content_set(s->ly, ":element1", s->bt);
		evas_object_show(s->bt);
		evas_object_smart_callback_add(s->bt, "clicked", elm_cb_play, data);*/
	}
}

void elm_cb_play(void *data, Evas_Object *obj, void *event_info) {
	cmd_play ((xmms_status*)data, NULL);
}


void win_del_cb (void *data, Evas_Object *obj, void *event_info) {
	xmms_status *s = (xmms_status*)data;
	Eina_List *win;

	win = status_find_window_by_win(s->windows, obj);
	if (win == NULL) {
printf("by obj\n");
		win = status_find_window_by_edje(s->windows, obj);
	}
	if (win != NULL) {
		s->windows = eina_list_remove(s->windows, win);
	}
printf("COUNT %d\n", eina_list_count(s->windows));
	if (eina_list_count(s->windows) == 0)
		elm_exit();

}

void app_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
printf("CLOSE: %p\n", eo);
	win_del_cb (data, eo, NULL);
}


void _set_window(xmms_status *s, const char *emission) {
	rockon_window *win = NULL;

	win = (rockon_window*) malloc(sizeof(rockon_window));
	if (win == NULL)
		print_error("Memory allocation error.", ERR_CRITICAL);
	win->elm_win = elm_win_add(NULL, emission, ELM_WIN_BASIC);
	elm_win_title_set(win->elm_win, emission);
	evas_object_smart_callback_add(win->elm_win, "delete-request", win_del_cb, s);
	elm_win_shaped_set(win->elm_win, 1);
	elm_win_alpha_set(win->elm_win, 1);
	elm_win_autodel_set(win->elm_win, 1);

	win->elm_layout = elm_layout_add(win->elm_win);
	if (!elm_layout_file_set(win->elm_layout, (const char*)s->config->theme, emission))
		print_error("Cannot load theme.", ERR_CRITICAL);
	evas_object_size_hint_weight_set(win->elm_layout, 1.0, 1.0);
	elm_win_resize_object_add(win->elm_win, win->elm_layout);
	evas_object_show(win->elm_layout);
	win->edje_obj = elm_layout_edje_get(win->elm_layout);

	s->windows = eina_list_append(s->windows, win);

printf("SET_WIN: win: %p elm_win: %p edje_obj: %p \n", win, win->elm_win, win->edje_obj);
	/* set callbacks */

	/* FIXME remove this function call when finish debuging */
	//edje_object_signal_callback_add (main_window->edje_obj, "*", "*", _p_signal, (void*)status);

	edje_object_signal_callback_add (win->edje_obj, "*", "elm_set.*", elm_cb_set, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "app_close", "*", app_close_cb, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "cmd_play", "*", edje_cb_play, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "cmd_pause", "*",edje_cb_pause, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "cmd_stop", "*", edje_cb_stop, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "cmd_next", "*", edje_cb_next, (void*)s);
	edje_object_signal_callback_add (win->edje_obj, "cmd_prev", "*", edje_cb_prev, (void*)s);

	evas_object_show(win->elm_win);
}
