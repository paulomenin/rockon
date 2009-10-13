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

#include "status.h"
#include "xmms_conn.h"

void status_free(rockon_status *status) {
	rockon_window *data;
	Eina_List *l;

	if (status->playlist_name)
		free (status->playlist_name);

	if (status->windows) {
		EINA_LIST_FOREACH(status->windows, l, data)
			free(data->name);

		//eina_list_free(status->windows);
	}

}

void status_fetch(rockon_status *status) {
	xmmsc_result_t *res;

	if ( ! status->connected)
		return;

	XMMS_CALLBACK_SET (status->connection,
					xmmsc_playback_current_id,
					broadcast_playback_id_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_playback_volume_get,
					broadcast_playback_volume_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_playback_status,
					broadcast_playback_status_cb, status);
	XMMS_CALLBACK_SET (status->connection,
					xmmsc_playback_playtime,
					signal_playback_playtime_cb, status);

	res = xmmsc_playlist_current_pos(status->connection, status->playlist_name);
	xmmsc_result_notifier_set_full(res, broadcast_playlist_pos_cb, status, NULL);
	xmmsc_result_unref(res);

}

void* status_find_window_by_win(const Eina_List *list, const void *data) {
	const Eina_List *l;
	void *list_data;

	if (list == NULL) return NULL;

	EINA_LIST_FOREACH(list, l, list_data) {
		if (((rockon_window*)list_data)->elm_win == data)
			return (Eina_List *)list_data;
	}

	return NULL;
}

void* status_find_window_by_edje(const Eina_List *list, const void *data) {
	const Eina_List *l;
	void *list_data;

	if (list == NULL) return NULL;

	EINA_LIST_FOREACH(list, l, list_data) {
		if (((rockon_window*)list_data)->edje_obj == data)
			return (Eina_List *)list_data;
	}

	return NULL;
}

void status_gui_update(rockon_status *status) {
	const Eina_List *l;
	void *win;
	//const Evas_Object *obj = NULL;
	
	EINA_LIST_FOREACH (status->windows, l, win) {

	if (status->changed_playtime) {
		//printf("Playtime: %d:%02d\n", status->playtime/60000, (status->playtime%60000)/1000);
		Edje_Message_Int msg;
		msg.val = status->playtime;
		edje_object_message_send( ((rockon_window*)win)->edje_obj, EDJE_MESSAGE_INT, 1, &msg);
	}

	if (status->changed_playback) {
		Edje_Message_Int msg;
		msg.val = status->playback_status;
		edje_object_message_send( ((rockon_window*)win)->edje_obj, EDJE_MESSAGE_INT, 2, &msg);
		//printf("Playback Status: %d\n", status->playback_status);
	}

	if (status->changed_playback_volume) {
		printf("Volume: %d\n", status->volume);
	}

	if (status->changed_mediainfo) {
		Edje_Message_String_Set *msg;
		Edje_Message_Int msg_dur;
		Edje_Message_Int msg_id;
		
		msg = calloc(1, sizeof(Edje_Message_String_Set) - sizeof(char*) + (7 * sizeof(char*)));
		msg->count = 7;
		msg->str[0] = (char*) status->media_artist;
		msg->str[1] = (char*) status->media_album;
		msg->str[2] = (char*) status->media_title;
		msg->str[3] = (char*) status->media_url;
		msg->str[4] = (char*) status->media_comment;
		msg->str[5] = (char*) status->media_genre;
		msg->str[6] = (char*) status->media_date;
		edje_object_message_send( ((rockon_window*)win)->edje_obj, EDJE_MESSAGE_STRING_SET, 4, msg);
		free(msg);
		
		msg_dur.val = status->media_duration;
		edje_object_message_send( ((rockon_window*)win)->edje_obj, EDJE_MESSAGE_INT, 5, &msg_dur);
		
		msg_id.val = status->playback_id;
		edje_object_message_send( ((rockon_window*)win)->edje_obj, EDJE_MESSAGE_INT, 6, &msg_id);
	}


	if (status->changed_playlist) {
		printf("Playlist: %s POS: %d\n",status->playlist_name,status->playlist_pos);
	}

	} // end EINA_LIST_FOREACH

	if (status->changed_playtime) status->changed_playtime = 0;
	if (status->changed_playback) status->changed_playback = 0;
	if (status->changed_playback_volume) status->changed_playback_volume = 0;
	if (status->changed_mediainfo) status->changed_mediainfo = 0;
	if (status->changed_playlist) status->changed_playlist = 0;

}

