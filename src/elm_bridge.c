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

#include "elm_bridge.h"

void elm_cb_set (void *data, Evas_Object *eo, const char *emission, const char *source) {
	xmms_status *s = (xmms_status*)data;
	printf("ELM_CALLBACK: emmission: %s source: %s\n", emission, source);
	
	s->bt = elm_button_add(s->elm_win);
	elm_button_label_set(s->bt, "Play");
	elm_layout_content_set(s->ly, ":element1", s->bt);
	evas_object_show(s->bt);
	evas_object_smart_callback_add(s->bt, "clicked", elm_cb_play, data);
}

void elm_cb_play(void *data, Evas_Object *obj, void *event_info) {
	cmd_play ((xmms_status*)data, NULL);
}
