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
#include "gui.h"

void gui_window_set(rockon_data *rdata) {
	Evas_Object *win, *bg, *lb_title;

	win = elm_win_add(NULL, "rockon", ELM_WIN_BASIC);
	elm_win_title_set(win, "Rockon");
	elm_win_borderless_set(win, 0);
	elm_win_autodel_set(win, 1);
	evas_object_smart_callback_add(win, "delete,request", app_exit, NULL);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, 1.0, 1.0);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	lb_title = elm_label_add(win);
	elm_label_label_set(lb_title, "Title");
	evas_object_size_hint_weight_set(lb_title, 1.0, 1.0);
	elm_win_resize_object_add(win, lb_title);
	evas_object_show(lb_title);

	evas_object_show(win);

	rdata->widgets.win = win;
	rdata->widgets.background = bg;
	rdata->widgets.label_title = lb_title;
}

void app_exit(void *data, Evas_Object *obj, void *event_info) {
   elm_exit();
}

