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
#include "callbacks.h"

void app_exit(void *data, Evas_Object *obj, void *event_info) {
   elm_exit();
}

void gui_window_set(rockon_data *rdata) {
	Evas_Object *win, *bg, *ic_logo;
	Evas_Object *lb_title, *lb_playtime;
	Evas_Object *bx, *bx_up, *pb, *tb, *seekbar;
	Evas_Object *flip;

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

	bx = elm_box_add(win);
	elm_win_resize_object_add(win, bx);
	evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(bx);
	
	bx_up = elm_box_add(win);
	elm_box_horizontal_set(bx_up, 1);
	evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_box_pack_end(bx, bx_up);
	elm_object_scale_set(bx_up, 1.5);
	evas_object_show(bx_up);

	lb_playtime = elm_label_add(win);
	evas_object_size_hint_weight_set(lb_playtime, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(lb_playtime, -1.0, 0.5);
	elm_box_pack_end(bx_up, lb_playtime);
	evas_object_show(lb_playtime);
	
	lb_title = elm_label_add(win);
	evas_object_size_hint_weight_set(lb_title, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(lb_title, -1.0, 0.5);
	elm_box_pack_end(bx_up, lb_title);
	evas_object_show(lb_title);

	flip = elm_flip_add(win);
	evas_object_size_hint_align_set(flip, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(flip, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_box_pack_end(bx, flip);
	evas_object_show(flip);

	ic_logo = elm_icon_add(win);
	elm_icon_file_set(ic_logo, rdata->config->edj_data_path, "logo");
	elm_icon_scale_set(ic_logo, 0, 0);
	elm_icon_no_scale_set(ic_logo, 1);
	elm_flip_content_front_set(flip, ic_logo);
	evas_object_show(ic_logo);

	pb = gui_playback_buttons_set(win, rdata);
	evas_object_size_hint_align_set(pb, 0.5, 0.5);
	elm_box_pack_end(bx, pb);
	
	seekbar = elm_slider_add(win);
	evas_object_size_hint_align_set(seekbar, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(seekbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_slider_indicator_format_function_set(seekbar, seekbar_format_indicator);
	evas_object_smart_callback_add(seekbar, "slider,drag,start", seekbar_drag_start_cb, rdata);
	evas_object_smart_callback_add(seekbar, "slider,drag,stop", seekbar_drag_stop_cb, rdata);
	elm_box_pack_end(bx, seekbar);
	evas_object_show(seekbar);

	tb = elm_toolbar_add(win);
	elm_toolbar_homogenous_set(tb, 1);
	evas_object_size_hint_weight_set(tb, 0.0, 0.0);
	evas_object_size_hint_align_set(tb, EVAS_HINT_FILL, 0.0);
	elm_box_pack_end(bx, tb);
	evas_object_show(tb);

	elm_toolbar_item_add(tb, NULL, "Playlist", toolbar_playlist_click_cb, rdata);
	elm_toolbar_item_add(tb, NULL, "Collections", toolbar_playlist_click_cb, rdata);

	evas_object_show(win);

	rdata->widgets.win = win;
	rdata->widgets.label_title = lb_title;
	rdata->widgets.label_playtime = lb_playtime;
	rdata->widgets.seekbar = seekbar;
	rdata->widgets.seekbar_update = 1;
	rdata->widgets.cover = ic_logo;
	rdata->widgets.flip = flip;
}

Evas_Object* gui_playback_buttons_set(Evas_Object* parent, rockon_data *rdata) {
	Evas_Object *box, *ic;
	Evas_Object *prev, *play, *pause, *stop, *next;

	box = elm_box_add(parent);
	elm_box_horizontal_set(box, 1);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(box);

	ic = elm_icon_add(parent);
	elm_icon_file_set(ic, rdata->config->edj_data_path, "icon_prev");
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
	evas_object_show(ic);
	prev = elm_button_add(parent);
	elm_button_icon_set(prev, ic);
	evas_object_size_hint_align_set(prev, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, prev);
	evas_object_smart_callback_add(prev, "clicked", elm_cb_prev, rdata);
	evas_object_show(prev);

	ic = elm_icon_add(parent);
	elm_icon_file_set(ic, rdata->config->edj_data_path, "icon_play");
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
	evas_object_show(ic);
	play = elm_button_add(parent);
	elm_button_icon_set(play, ic);
	evas_object_size_hint_align_set(play, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, play);
	evas_object_smart_callback_add(play, "clicked", elm_cb_play, rdata);
	evas_object_show(play);
	
	ic = elm_icon_add(parent);
	elm_icon_file_set(ic, rdata->config->edj_data_path, "icon_pause");
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
	evas_object_show(ic);
	pause = elm_button_add(parent);
	elm_button_icon_set(pause, ic);
	evas_object_size_hint_align_set(pause, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, pause);
	evas_object_smart_callback_add(pause, "clicked", elm_cb_pause, rdata);
	evas_object_show(pause);
	
	ic = elm_icon_add(parent);
	elm_icon_file_set(ic, rdata->config->edj_data_path, "icon_stop");
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
	evas_object_show(ic);
	stop = elm_button_add(parent);
	elm_button_icon_set(stop, ic);
	evas_object_size_hint_align_set(stop, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, stop);
	evas_object_smart_callback_add(stop, "clicked", elm_cb_stop, rdata);
	evas_object_show(stop);
	
	ic = elm_icon_add(parent);
	elm_icon_file_set(ic, rdata->config->edj_data_path, "icon_next");
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
	evas_object_show(ic);
	next = elm_button_add(parent);
	elm_button_icon_set(next, ic);
	evas_object_size_hint_align_set(next, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, next);
	evas_object_smart_callback_add(next, "clicked", elm_cb_next, rdata);
	evas_object_show(next);

	return box;
}

const char* seekbar_format_indicator(double val) {
	char indicator[8];
	int minutes = (int)val/60;
	int seconds = (int)val%60;
	
	snprintf(indicator, 8, "%d:%02d", minutes, seconds);
	
	return strdup(indicator);
}
