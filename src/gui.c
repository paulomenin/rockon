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
	Evas_Object *win, *ly, *edje_obj, *bg;
	Evas_Object *ic_logo, *ly_info, *ic_config;
	Evas_Object *pb, *tb, *seekbar;

	win = elm_win_add(NULL, "rockon", ELM_WIN_BASIC);
	elm_win_title_set(win, "Rockon");
	elm_win_borderless_set(win, 0);
	elm_win_autodel_set(win, 1);
	evas_object_smart_callback_add(win, "delete,request", app_exit, NULL);

	ly = elm_layout_add(win);
	elm_layout_file_set(ly, rdata->config->edj_data_path, "layout");
	evas_object_size_hint_weight_set(ly, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, ly);
	evas_object_show(ly);

	edje_obj = elm_layout_edje_get(ly);

	bg = elm_bg_add(win);
	elm_bg_file_set(bg, rdata->config->edj_data_path, "background");
	elm_layout_content_set(ly, "background", bg);
	evas_object_show(bg);


	ic_logo = elm_icon_add(win);
	elm_icon_file_set(ic_logo, rdata->config->edj_data_path, "logo");
	elm_layout_content_set(ly, "content", ic_logo);
	evas_object_show(ic_logo);

	ly_info = elm_layout_add(win);
	elm_layout_file_set(ly_info, rdata->config->edj_data_path, "playback_info");
	evas_object_hide(ly_info);

	ic_config = elm_icon_add(win);
	elm_icon_file_set(ic_config, rdata->config->edj_data_path, "config");
	evas_object_hide(ic_config);


	pb = gui_playback_buttons_set(win, rdata);
	evas_object_size_hint_weight_set(pb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_content_set(ly, "playbackbar", pb);

	seekbar = elm_slider_add(win);
	evas_object_size_hint_align_set(seekbar, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(seekbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_slider_indicator_format_function_set(seekbar, seekbar_format_indicator);
	evas_object_smart_callback_add(seekbar, "slider,drag,start", seekbar_drag_start_cb, rdata);
	evas_object_smart_callback_add(seekbar, "slider,drag,stop", seekbar_drag_stop_cb, rdata);
	elm_layout_content_set(ly, "seekbar", seekbar);
	evas_object_show(seekbar);

	tb = elm_toolbar_add(win);
	evas_object_size_hint_align_set(tb, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_layout_content_set(ly, "toolbar", tb);
	evas_object_show(tb);

	elm_toolbar_item_add(tb, NULL, "Playback", playlist_show_cb, rdata);
	elm_toolbar_item_add(tb, NULL, "Info", info_show_cb, rdata);
	elm_toolbar_item_add(tb, NULL, "Config", config_show_cb, rdata);

	evas_object_show(win);

	rdata->widgets.win = win;
	rdata->widgets.edje = edje_obj;
	rdata->widgets.layout = ly;
	rdata->widgets.seekbar = seekbar;
	rdata->widgets.seekbar_update = 1;
	rdata->widgets.playback_info = ly_info;
	rdata->widgets.playlist = ic_logo;
	rdata->widgets.config = ic_config;
	rdata->widgets.current_content = ic_logo;

	edje_object_signal_callback_add (edje_obj, "cmd,play", "*", edje_cb_play, (void*)rdata);
	edje_object_signal_callback_add (edje_obj, "cmd,stop", "*", edje_cb_stop, (void*)rdata);
	edje_object_signal_callback_add (edje_obj, "cmd,next", "*", edje_cb_next, (void*)rdata);
	edje_object_signal_callback_add (edje_obj, "cmd,prev", "*", edje_cb_prev, (void*)rdata);
	edje_object_signal_callback_add (edje_obj, "cmd,pause", "*",edje_cb_pause,(void*)rdata);
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
