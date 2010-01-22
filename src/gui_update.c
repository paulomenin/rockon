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

#include "gui_update.h"
#include "gui_window.h"
#include "gui_widgets.h"

#define DBG(...) EINA_LOG_DOM_DBG(gui_upd_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(gui_upd_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(gui_upd_log_dom, __VA_ARGS__)

extern int gui_upd_log_dom;

void gui_upd_playback_id (server_data *sdata) {
	INFO("Playback ID: %d", sdata->playback_id);
}

void gui_upd_playback_status (server_data *sdata) {
	const Eina_List *l;
	void *win;
	Edje_Message_Int msg;

	INFO("Playback STATUS: %d", sdata->playback_status);
	
	EINA_LIST_FOREACH (sdata->windows, l, win) {
		msg.val = sdata->playback_status;
		edje_object_message_send( ((rockon_window*)win)->edje_obj, EDJE_MESSAGE_INT, 2, &msg);
	}
}

void gui_upd_playback_playtime (server_data *sdata) {
	const Eina_List *l;
	void *obj;
	Edje_Message_Int msg;

	EINA_LIST_FOREACH (sdata->windows, l, obj) {
		msg.val = sdata->playback_playtime;
		edje_object_message_send( ((rockon_window*)obj)->edje_obj, EDJE_MESSAGE_INT, 1, &msg);
	}
	EINA_LIST_FOREACH (sdata->widgets->seekbars, l, obj) {
		if (((widget*)obj)->update == 1) {
			elm_slider_value_set(((widget*)obj)->widget, sdata->playback_playtime / 1000);
		}
	}
}

void gui_upd_playback_info (server_data *sdata) {
	const Eina_List *l;
	void *obj;
	Edje_Message_String_Set *msg;
	Edje_Message_Int msg_dur;
	Edje_Message_Int msg_id;

	if (sdata->playback_info == NULL) return;
	if (sdata->widgets == NULL) return;

	INFO("--------- MEDIA INFO ---------");
	INFO("%s - %s", sdata->playback_info->artist, sdata->playback_info->title);
	INFO("Album: %s", sdata->playback_info->album);
	INFO("Track Number: %d", sdata->playback_info->tracknr);
	INFO("URL: %s", sdata->playback_info->url);
	INFO("Comment: %s", sdata->playback_info->comment);
	INFO("Genre: %s Date: %s", sdata->playback_info->genre, sdata->playback_info->date);
	INFO("Bitrate: %d Duration: %d:%02d", sdata->playback_info->bitrate,
										(sdata->playback_info->duration / 1000)/60,
										(sdata->playback_info->duration / 1000)%60);
	INFO("-----------------------------");

	EINA_LIST_FOREACH (sdata->windows, l, obj) {
		msg = calloc(1, sizeof(Edje_Message_String_Set) - sizeof(char*) + (7 * sizeof(char*)));
		msg->count = 7;
		msg->str[0] = (char*) sdata->playback_info->artist;
		msg->str[1] = (char*) sdata->playback_info->album;
		msg->str[2] = (char*) sdata->playback_info->title;
		msg->str[3] = (char*) sdata->playback_info->url;
		msg->str[4] = (char*) sdata->playback_info->comment;
		msg->str[5] = (char*) sdata->playback_info->genre;
		msg->str[6] = (char*) sdata->playback_info->date;
		edje_object_message_send( ((rockon_window*)obj)->edje_obj, EDJE_MESSAGE_STRING_SET, 4, msg);
		free(msg);

		msg_dur.val = sdata->playback_info->duration;
		edje_object_message_send( ((rockon_window*)obj)->edje_obj, EDJE_MESSAGE_INT, 5, &msg_dur);

		msg_id.val = sdata->playback_id;
		edje_object_message_send( ((rockon_window*)obj)->edje_obj, EDJE_MESSAGE_INT, 6, &msg_id);
	}

	EINA_LIST_FOREACH (sdata->widgets->seekbars, l, obj) {
		elm_slider_min_max_set(((widget*)obj)->widget, 0, sdata->playback_info->duration / 1000);
	}

}

void gui_upd_playlist (server_data *sdata) {
	Eina_List *l, *l2;
	void *data, *data2;

	if (sdata->playlist_current == NULL) return;
	
	INFO("--------- PLAYLIST ---------");
	INFO("Name: %s Items: %d Pos: %d",sdata->playlist_current->name,
									  sdata->playlist_current->num_items,
									  sdata->playlist_current->current_pos);

	EINA_LIST_FOREACH(sdata->playlist_current->items, l, data) {
		INFO("%02d %s",((playlist_item*)data)->pos, ((playlist_item*)data)->title);
	}
	INFO("----------------------------");

	EINA_LIST_FOREACH (sdata->widgets->playlists, l, data) {
		elm_list_clear(((widget*)data)->widget);
		EINA_LIST_FOREACH (sdata->playlist_current->items, l2, data2) {
			elm_list_item_append(((widget*)data)->widget, ((playlist_item*)data2)->title, NULL, NULL,  NULL, data2);
		}
		elm_list_go(((widget*)data)->widget);
	}
}

void gui_upd_playlist_list (server_data *sdata) {
	Eina_List *l, *l2;
	void *data, *data2;

	if (sdata->playlists == NULL) return;

	INFO("------ PLAYLIST LIST -------");
	EINA_LIST_FOREACH(sdata->playlists->playlists_, l, data) {
		INFO("%s", (char*)data);
	}
	EINA_LIST_FOREACH(sdata->playlists->playlists, l, data) {
		INFO("%s", (char*)data);
	}
	INFO("----------------------------");

	EINA_LIST_FOREACH (sdata->widgets->playlist_lists, l, data) {
		elm_list_clear(((widget*)data)->widget);
		EINA_LIST_FOREACH (sdata->playlists->playlists, l2, data2) {
			elm_list_item_append(((widget*)data)->widget, (char*)data2, NULL, NULL,  NULL, NULL);
		}
		elm_list_go(((widget*)data)->widget);
	}

}

void gui_upd_playlist_pos (server_data *sdata) {
	if (sdata->playlist_current == NULL) return;
	INFO("Name: %s Items: %d POS: %d",sdata->playlist_current->name,
									  sdata->playlist_current->num_items,
									  sdata->playlist_current->current_pos);
}

