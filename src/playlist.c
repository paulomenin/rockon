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

#include "playlist.h"

void pls_item_class_set(rockon_status *s) {
	s->pls_item_class.item_style     = "double_label";
	s->pls_item_class.func.label_get = pls_item_label_get;
	s->pls_item_class.func.icon_get  = pls_item_icon_get;
	s->pls_item_class.func.state_get = pls_item_state_get;
	s->pls_item_class.func.del       = pls_item_del;
}

void pls_free(rockon_status *s) {
	playlist_item *data;
	Eina_List *l;

	EINA_LIST_FOREACH(s->playlist, l, data) {
		free(data->title);
		free(data->artist);
		free(data->album);
		free(data);
	}

	s->playlist = NULL;
}

void set_playlist(rockon_status *s, const char *emission, Evas_Object *eo) {
	Eina_List *win;
	win = status_find_window_by_edje(s->windows, eo);
	if (win == NULL) return;
	if (((rockon_window*)win)->playlist) return;

	((rockon_window*)win)->playlist =
					elm_genlist_add( ((rockon_window*)win)->elm_win );
	elm_genlist_horizontal_mode_set( 
				((rockon_window*)win)->playlist,
				ELM_LIST_LIMIT );
	elm_layout_content_set(
				((rockon_window*)win)->elm_layout,
				emission,
				((rockon_window*)win)->playlist );

	evas_object_show( ((rockon_window*)win)->playlist );

	evas_object_smart_callback_add(((rockon_window*)win)->playlist, "clicked", elm_cb_pls_click, s);

	pls_populate( (rockon_window*)win, s);
}

void pls_populate(rockon_window *win, rockon_status *s) {
	Elm_Genlist_Item *pls_item;
	playlist_item *data;
	Eina_List *l;

	elm_genlist_clear(win->playlist);

	EINA_LIST_FOREACH(s->playlist, l, data) {
		pls_item = elm_genlist_item_append(
			win->playlist,
			&(s->pls_item_class),
			data,
			NULL,
			ELM_GENLIST_ITEM_NONE,
			pls_item_sel,
			NULL);
	}
}

void pls_update_playback_pos(rockon_window *win) {
	Elm_Genlist_Item *pls_item;

	if (win->playlist == NULL) return;

	pls_item = elm_genlist_first_item_get(win->playlist);

	while (pls_item) {
		elm_genlist_item_update(pls_item);
		pls_item = elm_genlist_item_next_get(pls_item);
	}
}


char *pls_item_label_get(const void *data, Evas_Object *obj, const char *part) {
	char buf[256];

	if (!strcmp(part, "elm.text")) {
		snprintf(buf, sizeof(buf), "%s", ((playlist_item*)data)->title);
	}
	else if (!strcmp(part, "elm.text.sub")) {
		snprintf(buf, sizeof(buf), "by %s from %s",((playlist_item*)data)->artist, ((playlist_item*)data)->album);
	}

   return strdup(buf);
}

Evas_Object *pls_item_icon_get(const void *data, Evas_Object *obj, const char *part) {
	Evas_Object *icon = NULL;
	playlist_item *pi = (playlist_item*)data;

	if (!strcmp(part, "elm.swallow.icon")) {
		if ( pi->pos == pi->status->playlist_pos ) {
			icon = elm_icon_add(obj);
			elm_icon_file_set (icon, pi->status->config->theme, "icon.pls_playing");
			evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
			return icon;
		} else {
		
		}
	}
	else if (!strcmp(part, "elm.swallow.end")) {
		
	}
	return NULL;
}

Eina_Bool pls_item_state_get(const void *data, Evas_Object *obj, const char *part) {
   return EINA_FALSE;
}

void pls_item_del(const void *data, Evas_Object *obj) {
}

void pls_item_sel(void *data, Evas_Object *obj, void *event_info) {
}
