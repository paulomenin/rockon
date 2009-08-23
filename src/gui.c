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

int gui_setup (rockon_config *config, xmms_status *status) {
	Ecore_Evas *ee = NULL;
	Evas_Object *edje_o = NULL;
	int x, y, w, h;

	ee = ecore_evas_new (NULL, 0, 0, 320, 240, NULL);
	if (!ee)
		print_error("Cannot get canvas.", ERR_CRITICAL);

	ecore_evas_title_set (ee, "Rockon");
	ecore_evas_name_class_set(ee, "rockon", "Rockon");

	ecore_evas_shaped_set(ee, 1);
	ecore_evas_alpha_set(ee, 1);
	//ecore_evas_borderless_set(ee, 1);


	ecore_evas_callback_delete_request_set (ee, win_del_cb);
	ecore_evas_callback_resize_set (ee, win_resize_cb);
	ecore_evas_callback_move_set (ee, win_move_cb);

	/* load .edj */
	edje_o = edje_object_add (ecore_evas_get(ee));
	if (! edje_object_file_set (edje_o, (const char*)config->theme, "main"))
		print_error("Cannot load theme.", ERR_CRITICAL);

	status->edje_gui = edje_o;
	evas_object_name_set (edje_o, "main");
	evas_object_resize (edje_o, 320, 240);
	evas_object_show (edje_o);

	ecore_evas_show (ee);
	
	/* set callbacks */

	edje_object_signal_callback_add (edje_o, "app_close", "main", app_close_cb, (void*)status);
	edje_object_signal_callback_add (edje_o, "cmd_play", "main", cmd_play_cb, (void*)status);
	edje_object_signal_callback_add (edje_o, "cmd_pause", "main", cmd_pause_cb, (void*)status);
	edje_object_signal_callback_add (edje_o, "cmd_stop", "main", cmd_stop_cb, (void*)status);
	edje_object_signal_callback_add (edje_o, "cmd_next", "main", cmd_next_cb, (void*)status);
	edje_object_signal_callback_add (edje_o, "cmd_prev", "main", cmd_prev_cb, (void*)status);

	ecore_main_loop_begin ();

	return TRUE;
}

void win_resize_cb (Ecore_Evas *ee) {
	int w, h;
	int minw, minh;
	int maxw, maxh;
	Evas_Object *o = NULL;

	if (ee) {
		ecore_evas_geometry_get (ee, NULL, NULL, &w, &h);
		ecore_evas_size_min_get (ee, &minw, &minh);
		ecore_evas_size_max_get (ee, &maxw, &maxh);

		if ((w >= minw) && (h >= minh) && (h <= maxh) && (w <= maxw)) {
			if ((o = evas_object_name_find (ecore_evas_get (ee), "main")))
				evas_object_resize (o, w, h);
		}
	}
}

void win_move_cb (Ecore_Evas *ee) {}

void win_del_cb (Ecore_Evas *ee) {
	ecore_main_loop_quit();
}

void app_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	win_del_cb (NULL);
}
