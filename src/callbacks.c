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

#include "callbacks.h"
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

void seekbar_drag_start_cb(void *data, Evas_Object *obj, void *event_info) {
	((rockon_data*)data)->widgets.seekbar_update = 0;
}
void seekbar_drag_stop_cb(void *data, Evas_Object *obj, void *event_info) {
	int seconds = (int)elm_slider_value_get(obj);
	cmd_seek_ms((rockon_data*)data, seconds*1000);
	((rockon_data*)data)->widgets.seekbar_update = 1;
}



void content_show(rockon_data* rdata, Evas_Object* obj) {
	if (rdata->widgets.current_content == obj) return;

	evas_object_hide(rdata->widgets.current_content);
	elm_layout_content_set(rdata->widgets.layout, "content", obj);
	evas_object_show(obj);
	rdata->widgets.current_content = obj;
}

void info_show_cb(void *data, Evas_Object *obj, void *event_info){
	content_show( ((rockon_data*)data),
				  ((rockon_data*)data)->widgets.playback_info);
}
void playlist_show_cb(void *data, Evas_Object *obj, void *event_info){
	content_show( ((rockon_data*)data),
				  ((rockon_data*)data)->widgets.playlist);
}
void config_show_cb(void *data, Evas_Object *obj, void *event_info){
	content_show( ((rockon_data*)data),
				  ((rockon_data*)data)->widgets.config);
}




void edje_cb_play  (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_play((rockon_data*)data);
}

void edje_cb_pause (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_pause((rockon_data*)data);
}

void edje_cb_stop (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_stop((rockon_data*)data);
}

void edje_cb_next (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_next((rockon_data*)data);
}

void edje_cb_prev (void *data, Evas_Object *eo, const char *emission, const char *source) {
	cmd_prev((rockon_data*)data);
}
