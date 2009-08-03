#include "gui.h"

static void _refresh_trans (Ecore_Evas *ee);

int gui_setup () {
	Ecore_Evas *ee = NULL;
	Evas_Object *edje_o = NULL;
	Evas *evas_trans = NULL;
	Evas_Object *trans = NULL;
	int x, y, w, h;

	ee = ecore_evas_new (NULL, 0, 0, 320, 240, NULL);
	if (!ee)
		print_error("Cannot get canvas.", ERR_CRITICAL);

	ecore_evas_title_set (ee, "Rockon");

	ecore_evas_callback_delete_request_set (ee, win_del_cb);
	ecore_evas_callback_resize_set (ee, win_resize_cb);
	ecore_evas_callback_move_set (ee, win_move_cb);

	/* load .edj */
	edje_o = edje_object_add (ecore_evas_get(ee));
	if (! edje_object_file_set (edje_o, "../data/theme/default/gui.edj", "main"))
		print_error("Cannot load theme.", ERR_CRITICAL);

	evas_object_name_set (edje_o, "main");
	evas_object_resize (edje_o, 320, 240);
	evas_object_show (edje_o);

	/* create transparency */

	evas_trans = ecore_evas_get (ee);

	trans = esmart_trans_x11_new (evas_trans);
	evas_object_move (trans, 0, 0);
	evas_object_layer_set (trans, -5);
	evas_object_name_set (trans, "trans_bg");

	ecore_evas_geometry_get (ee, &x, &y, &w, &h);
	evas_object_resize (trans, w, h);

	evas_object_show (trans);
	ecore_evas_show (ee);

	esmart_trans_x11_freshen (trans, x, y, w, h);

	/* set callbacks */

	edje_object_signal_callback_add (edje_o, "app_close", "main", app_close_cb, NULL);
	edje_object_signal_callback_add (edje_o, "cmd_play", "main", cmd_play_cb, NULL);
	edje_object_signal_callback_add (edje_o, "cmd_pause", "main", cmd_pause_cb, NULL);
	edje_object_signal_callback_add (edje_o, "cmd_stop", "main", cmd_stop_cb, NULL);
	edje_object_signal_callback_add (edje_o, "cmd_next", "main", cmd_next_cb, NULL);
	edje_object_signal_callback_add (edje_o, "cmd_prev", "main", cmd_prev_cb, NULL);

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
			if ((o = evas_object_name_find (ecore_evas_get (ee), "trans_bg")))
				evas_object_resize (o, w, h);
			if ((o = evas_object_name_find (ecore_evas_get (ee), "main")))
				evas_object_resize (o, w, h);
		}
	}
	_refresh_trans (ee);
}

void win_move_cb (Ecore_Evas *ee) {
	_refresh_trans (ee);
}

void win_del_cb (Ecore_Evas *ee) {
	ecore_main_loop_quit();
}

void app_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	win_del_cb (NULL);
}

static void _refresh_trans (Ecore_Evas *ee) {
	int x, y, w, h;
	Evas_Object *o = NULL;

	if (!ee) return;

	ecore_evas_geometry_get (ee, &x, &y, &w, &h);
	o = evas_object_name_find (ecore_evas_get (ee), "trans_bg");

	if (!o) {
		print_error ("Trans object not found. Exiting...\n", ERR_NORMAL);
		ecore_main_loop_quit ();
	}
	esmart_trans_x11_freshen (o, x, y, w, h);
}

