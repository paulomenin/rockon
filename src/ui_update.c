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
#include "ui_update.h"

#define DBG(...) EINA_LOG_DOM_DBG(ui_upd_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(ui_upd_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(ui_upd_log_dom, __VA_ARGS__)

extern int ui_upd_log_dom;

void ui_upd_disconnect (rockon_data *rdata) {
	INFO("xmms2 connection lost.");

	elm_list_clear(rdata->widgets.playlist);
	elm_list_clear(rdata->widgets.playlists);
	if (rdata->widgets.volumebar_update)
		elm_slider_value_set(rdata->widgets.volumebar, 0);
	if (rdata->widgets.seekbar_update)
		elm_slider_value_set(rdata->widgets.seekbar, 0);

	edje_object_signal_emit(rdata->widgets.edje, "main:disconnect", "rockon");
}

void ui_upd_playback_id (rockon_data *rdata) {
	INFO("Playback ID: %d", rdata->playback_id);
}

void ui_upd_playback_status (rockon_data *rdata) {
	Edje_Message_Int msg;
	switch (rdata->playback_status) {
		case XMMS_PLAYBACK_STATUS_STOP:
			INFO("Playback STATUS: Stopped"); break;
		case XMMS_PLAYBACK_STATUS_PLAY:
			INFO("Playback STATUS: Playing"); break;
		case XMMS_PLAYBACK_STATUS_PAUSE:
			INFO("Playback STATUS: Paused");
	}
	msg.val = rdata->playback_status;
	edje_object_message_send(rdata->widgets.edje, EDJE_MESSAGE_INT, PLAYBACK_STATUS, &msg);
}

void ui_upd_playback_playtime (rockon_data *rdata) {
	Edje_Message_Int msg;
	msg.val = rdata->playback_playtime;
	edje_object_message_send(rdata->widgets.edje, EDJE_MESSAGE_INT, PLAYBACK_PLAYTIME, &msg);
	if (rdata->widgets.seekbar_update)
		elm_slider_value_set(rdata->widgets.seekbar, rdata->playback_playtime / 1000);
}

void ui_upd_playback_info (rockon_data *rdata) {
	Edje_Message_String_Set *msg;
	Edje_Message_Int msg_dur;
	Edje_Message_Int msg_id;

	if (rdata->playback_info == NULL) return;

	INFO("--------- MEDIA INFO ---------");
	INFO("%s - %s", rdata->playback_info->artist, rdata->playback_info->title);
	INFO("Album: %s", rdata->playback_info->album);
	INFO("Track Number: %d", rdata->playback_info->tracknr);
	INFO("URL: %s", rdata->playback_info->url);
	INFO("Comment: %s", rdata->playback_info->comment);
	INFO("Genre: %s Date: %s", rdata->playback_info->genre, rdata->playback_info->date);
	INFO("Bitrate: %d Duration: %d:%02d", rdata->playback_info->bitrate,
										(rdata->playback_info->duration / 1000)/60,
										(rdata->playback_info->duration / 1000)%60);
	INFO("-----------------------------");

	msg = calloc(1, sizeof(Edje_Message_String_Set) - sizeof(char*) + (7 * sizeof(char*)));
	msg->count = 7;
	msg->str[0] = (char*) rdata->playback_info->artist;
	msg->str[1] = (char*) rdata->playback_info->album;
	msg->str[2] = (char*) rdata->playback_info->title;
	msg->str[3] = (char*) rdata->playback_info->url;
	msg->str[4] = (char*) rdata->playback_info->comment;
	msg->str[5] = (char*) rdata->playback_info->genre;
	msg->str[6] = (char*) rdata->playback_info->date;
	edje_object_message_send(rdata->widgets.edje, EDJE_MESSAGE_STRING_SET, PLAYBACK_INFO, msg);
	free(msg);

	msg_dur.val = rdata->playback_info->duration;
	msg_id.val  = rdata->playback_id;
	edje_object_message_send(rdata->widgets.edje, EDJE_MESSAGE_INT, PLAYBACK_DURATION, &msg_dur);
	edje_object_message_send(rdata->widgets.edje, EDJE_MESSAGE_INT, PLAYBACK_ID, &msg_id);

	elm_slider_min_max_set(rdata->widgets.seekbar, 0, rdata->playback_info->duration / 1000);
	
	ui_upd_playback_playtime(rdata);
}

void ui_upd_playback_volume (rockon_data *rdata) {
	Eina_List *l;
	volume_channel* ch;
	int volume = 0;
	INFO("Volume Changed");
	EINA_LIST_FOREACH(rdata->volume, l, ch) {
		INFO("%s %d", ch->name, ch->value);
		volume = ch->value;
	}
	/* show only last channel */
	if (rdata->widgets.volumebar_update)
		elm_slider_value_set(rdata->widgets.volumebar, volume);

}

void ui_upd_playlist (rockon_data *rdata, playlist *pls) {
	Eina_List *l;
	void *data;
	Edje_Message_String pls_name;
	Edje_Message_Int pls_pos;
	Edje_Message_Int pls_itens;
	int num_items;

	elm_list_clear(rdata->widgets.playlist);
	if (pls == NULL) return;
	playlist_wait(pls);

	num_items = pls->num_items;
	if (num_items < 0) num_items = 0;

	INFO("--------- PLAYLIST ---------");
	INFO("Name: %s Items: %d Pos: %d",pls->name,
									  num_items,
									  pls->current_pos);

	EINA_LIST_FOREACH(pls->items, l, data) {
		INFO("%02d %s",((playlist_item*)data)->pos, ((playlist_item*)data)->title);
		elm_list_item_append(rdata->widgets.playlist, ((playlist_item*)data)->title, NULL, NULL,  NULL, data);
	}
	INFO("----------------------------");
	elm_list_go(rdata->widgets.playlist);

	pls_name.str  = pls->name;
	pls_pos.val   = pls->current_pos;
	pls_itens.val = num_items;
	edje_object_message_send (rdata->widgets.edje, EDJE_MESSAGE_STRING, PLAYLIST_NAME, &pls_name);
	edje_object_message_send (rdata->widgets.edje, EDJE_MESSAGE_INT, PLAYLIST_POS, &pls_pos);
	edje_object_message_send (rdata->widgets.edje, EDJE_MESSAGE_INT, PLAYLIST_ITENS, &pls_itens);

}

void ui_upd_playlist_list (rockon_data *rdata) {
	Eina_List *l;
	void *data;

	elm_list_clear(rdata->widgets.playlists);
	if (rdata->playlists == NULL) return;

	INFO("------ PLAYLIST LIST -------");
	EINA_LIST_FOREACH(rdata->playlists->playlists_, l, data) {
		INFO("%s", ((playlist*)data)->name);
	}
	EINA_LIST_FOREACH(rdata->playlists->playlists, l, data) {
		INFO("%s", ((playlist*)data)->name);
		elm_list_item_append(rdata->widgets.playlists, ((playlist*)data)->name, NULL, NULL,  NULL, NULL);
	}
	INFO("----------------------------");

	elm_list_go(rdata->widgets.playlists);
}

void ui_upd_playlist_pos (rockon_data *rdata) {
	Edje_Message_String pls_name;
	Edje_Message_Int pls_pos;
	Edje_Message_Int pls_itens;

	if (rdata->current_playlist == NULL) return;
	INFO("Name: %s Items: %d POS: %d",rdata->current_playlist->name,
									  rdata->current_playlist->num_items,
									  rdata->current_playlist->current_pos);

	pls_name.str  = rdata->current_playlist->name;
	pls_pos.val   = rdata->current_playlist->current_pos;
	pls_itens.val = rdata->current_playlist->num_items;
	edje_object_message_send (rdata->widgets.edje, EDJE_MESSAGE_STRING, PLAYLIST_NAME, &pls_name);
	edje_object_message_send (rdata->widgets.edje, EDJE_MESSAGE_INT, PLAYLIST_POS, &pls_pos);
	edje_object_message_send (rdata->widgets.edje, EDJE_MESSAGE_INT, PLAYLIST_ITENS, &pls_itens);
}

void ui_upd_coll_list (rockon_data *rdata) {
	Eina_List *l;
	void *data;

	if (rdata->collections == NULL) return;

	INFO("----- COLLECTION LIST ------");
	EINA_LIST_FOREACH(rdata->collections->Collections, l, data) {
		INFO("%s", ((collection*)data)->name);
	}
	INFO("----------------------------");
}

void ui_upd_mlib_reader_status(rockon_data *rdata, int files) {
	switch (files) {
		case -1:
			INFO("mlib updating files.");
			break;
		case  0:
			INFO("mlib updating files finished.");
			break;
		default:
			INFO("mlib updating files: %d remaining.", files);
	}
}
