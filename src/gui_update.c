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
	void *win;
	Edje_Message_Int msg;

	/*INFO("Playback Playtime: %d:%02d",
			(sdata->playback_playtime / 1000)/60,
			(sdata->playback_playtime / 1000)%60);
	*/

	EINA_LIST_FOREACH (sdata->windows, l, win) {
		msg.val = sdata->playback_playtime;
		edje_object_message_send( ((rockon_window*)win)->edje_obj, EDJE_MESSAGE_INT, 1, &msg);
	}
}

void gui_upd_playback_info (server_data *sdata) {
	const Eina_List *l;
	void *win;
	Edje_Message_String_Set *msg;
	Edje_Message_Int msg_dur;
	Edje_Message_Int msg_id;

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

	EINA_LIST_FOREACH (sdata->windows, l, win) {
		msg = calloc(1, sizeof(Edje_Message_String_Set) - sizeof(char*) + (7 * sizeof(char*)));
		msg->count = 7;
		msg->str[0] = (char*) sdata->playback_info->artist;
		msg->str[1] = (char*) sdata->playback_info->album;
		msg->str[2] = (char*) sdata->playback_info->title;
		msg->str[3] = (char*) sdata->playback_info->url;
		msg->str[4] = (char*) sdata->playback_info->comment;
		msg->str[5] = (char*) sdata->playback_info->genre;
		msg->str[6] = (char*) sdata->playback_info->date;
		edje_object_message_send( ((rockon_window*)win)->edje_obj, EDJE_MESSAGE_STRING_SET, 4, msg);
		free(msg);

		msg_dur.val = sdata->playback_info->duration;
		edje_object_message_send( ((rockon_window*)win)->edje_obj, EDJE_MESSAGE_INT, 5, &msg_dur);

		msg_id.val = sdata->playback_id;
		edje_object_message_send( ((rockon_window*)win)->edje_obj, EDJE_MESSAGE_INT, 6, &msg_id);
	}
}

void gui_upd_playlist (server_data *sdata) {
	Eina_List *l;
	void *data;

	INFO("--------- PLAYLIST ---------");
	INFO("Name: %s Items: %d Pos: %d",sdata->playlist_current->name,
									  sdata->playlist_current->num_items,
									  sdata->playlist_current->current_pos);

	EINA_LIST_FOREACH(sdata->playlist_current->items, l, data) {
		INFO("%02d %s",((playlist_item*)data)->pos, ((playlist_item*)data)->title);
	}
	INFO("----------------------------");
}

void gui_upd_playlist_list (server_data *sdata) {
	Eina_List *l;
	void *data;

	INFO("------ PLAYLIST LIST -------");
	EINA_LIST_FOREACH(sdata->playlists->playlists_, l, data) {
		INFO("%s", (char*)data);
	}
	EINA_LIST_FOREACH(sdata->playlists->playlists, l, data) {
		INFO("%s", (char*)data);
	}
	INFO("----------------------------");
}

void gui_upd_playlist_pos (server_data *sdata) {
	INFO("Name: %s Items: %d POS: %d",sdata->playlist_current->name,
									  sdata->playlist_current->num_items,
									  sdata->playlist_current->current_pos);
}

