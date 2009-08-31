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

#include "gui.h"

/* FIXME remove when finish debuging */
void _p_signal(void *data, Evas_Object *eo, const char *emission, const char *source);

int gui_setup (rockon_config *config, xmms_status *status) {
	Evas_Object *main_window, *edje_o;
	Evas_Object *ly = NULL;
	int x, y, w, h;
	
	main_window = elm_win_add(NULL, "main_win", ELM_WIN_BASIC);
	elm_win_title_set(main_window, "Rockon");
	evas_object_smart_callback_add(main_window, "delete-request", win_del_cb, NULL);
	elm_win_shaped_set(main_window, 1);
    elm_win_alpha_set(main_window, 1);

	ly = elm_layout_add(main_window);
	elm_layout_file_set(ly, (const char*)config->theme, "main");
	evas_object_size_hint_weight_set(ly, 1.0, 1.0);
	elm_win_resize_object_add(main_window, ly);
	evas_object_show(ly);
	edje_o = elm_layout_edje_get(ly);
	
	status->edje_gui = edje_o;
	status->elm_win = main_window;
	status->ly = ly;
	
	/* set callbacks */

	/* FIXME remove this function call when finish debuging */
	edje_object_signal_callback_add (edje_o, "*", "*", _p_signal, (void*)status);
	
	edje_object_signal_callback_add (edje_o, "elm_set.*", "*", elm_cb_set, (void*)status);

	edje_object_signal_callback_add (edje_o, "app_close", "*", app_close_cb, (void*)status);

	edje_object_signal_callback_add (edje_o, "cmd_play", "*", edje_cb_play, (void*)status);
	edje_object_signal_callback_add (edje_o, "cmd_pause", "*",edje_cb_pause, (void*)status);
	edje_object_signal_callback_add (edje_o, "cmd_stop", "*", edje_cb_stop, (void*)status);
	edje_object_signal_callback_add (edje_o, "cmd_next", "*", edje_cb_next, (void*)status);
	edje_object_signal_callback_add (edje_o, "cmd_prev", "*", edje_cb_prev, (void*)status);

	evas_object_show(main_window);

	elm_run();
	elm_shutdown();
	return TRUE;
}

void win_del_cb (void *data, Evas_Object *obj, void *event_info) {
	elm_exit();
}

void app_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	win_del_cb (NULL, NULL, NULL);
}

/* FIXME remove this function when finish debuging */
void _p_signal(void *data, Evas_Object *eo, const char *emission, const char *source) {
	printf("SIG: \"%s\" | \"%s\"\n", emission, source);
}

