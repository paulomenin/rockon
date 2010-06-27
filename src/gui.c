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
#include "rockon_data.h"
#include "commands.h"

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

Evas_Object* gui_playback_buttons_set(Evas_Object* parent, rockon_data *rdata) {
	Evas_Object *box, *prev, *play, *pause, *stop, *next;

	box = elm_box_add(parent);
	elm_box_horizontal_set(box, 1);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(box);

	prev = elm_button_add(parent);
	elm_button_label_set(prev, "Prev");
	evas_object_size_hint_align_set(prev, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, prev);
	evas_object_smart_callback_add(prev, "clicked", elm_cb_prev, rdata);
	evas_object_show(prev);

	play = elm_button_add(parent);
	elm_button_label_set(play, "Play");
	evas_object_size_hint_align_set(play, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, play);
	evas_object_smart_callback_add(play, "clicked", elm_cb_play, rdata);
	evas_object_show(play);
	
	pause = elm_button_add(parent);
	elm_button_label_set(pause, "Pause");
	evas_object_size_hint_align_set(pause, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, pause);
	evas_object_smart_callback_add(pause, "clicked", elm_cb_pause, rdata);
	evas_object_show(pause);
	
	stop = elm_button_add(parent);
	elm_button_label_set(stop, "Stop");
	evas_object_size_hint_align_set(stop, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, stop);
	evas_object_smart_callback_add(stop, "clicked", elm_cb_stop, rdata);
	evas_object_show(stop);
	
	next = elm_button_add(parent);
	elm_button_label_set(next, "Next");
	evas_object_size_hint_align_set(next, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, next);
	evas_object_smart_callback_add(next, "clicked", elm_cb_next, rdata);
	evas_object_show(next);

	return box;
}

void gui_window_set(rockon_data *rdata) {
	Evas_Object *win, *bg, *lb_title, *ic_logo;
	Evas_Object *table, *pb;

	win = elm_win_add(NULL, "rockon", ELM_WIN_BASIC);
	elm_win_title_set(win, "Rockon");
	elm_win_borderless_set(win, 0);
	elm_win_autodel_set(win, 1);
	evas_object_smart_callback_add(win, "delete,request", app_exit, NULL);

	bg = elm_bg_add(win);
	elm_bg_file_set(bg, rdata->config->edj_data_path, "background");
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	table = elm_table_add(win);
	elm_win_resize_object_add(win, table);
	evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(table);

	ic_logo = elm_icon_add(win);
	elm_icon_file_set(ic_logo, rdata->config->edj_data_path, "logo");
	elm_icon_scale_set(ic_logo, 0, 0);
	elm_icon_no_scale_set(ic_logo, 1);
	evas_object_size_hint_weight_set(ic_logo, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_fill_set(ic_logo, 0.5, 0.5);
	elm_table_pack(table, ic_logo, 0, 0, 1, 2);
	evas_object_show(ic_logo);

	pb = gui_playback_buttons_set(win, rdata);
	elm_table_pack(table, pb, 1, 1, 1, 1);
	
	lb_title = elm_label_add(win);
	evas_object_size_hint_weight_set(lb_title, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(lb_title, -1.0, 0.5);
	elm_table_pack(table, lb_title, 1, 0, 1, 1);
	evas_object_show(lb_title);

	evas_object_show(win);

	rdata->widgets.win = win;
	rdata->widgets.table = table;
	rdata->widgets.background = bg;
	rdata->widgets.label_title = lb_title;
	rdata->widgets.logo = ic_logo;
}

void app_exit(void *data, Evas_Object *obj, void *event_info) {
   elm_exit();
}

