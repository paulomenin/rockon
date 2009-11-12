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

#ifndef STATUS_H
#define STATUS_H

#include <Evas.h>
#include <Edje.h>
#include <Elementary.h>
#include <xmmsclient/xmmsclient.h>
#include "config.h"

typedef struct {
	char 	    *name;
	Evas_Object *elm_win;
	Evas_Object *elm_layout;
	Evas_Object *edje_obj;
	Evas_Object *playlist;
} rockon_window;

typedef struct {
	int connected;
	xmmsc_connection_t *connection;
	rockon_config *config;
	Eina_List *windows;	
	
	int changed_playback;
	int changed_playback_volume;
	int changed_playtime;
	int changed_mediainfo;
	int changed_playlist;
	int changed_playlist_pos;

	Eina_List *playlist;
	char *playlist_name;
	int playlist_pos;
	Elm_Genlist_Item_Class pls_item_class;

	int playback_status;
	int volume;
	int playtime;
	int playback_id;
	
	/* Be careful these pointers are not valid all the time */
	const char *media_artist;
	const char *media_album;
	const char *media_title;
	const char *media_url;
	const char *media_comment;
	const char *media_genre;
	const char *media_date;
	int  media_bitrate;
	int  media_duration;

} rockon_status;

void status_free(rockon_status *status);
void status_fetch(rockon_status *status);
void status_gui_update(rockon_status *status);
void* status_find_window_by_win(const Eina_List *list, const void *data);
void* status_find_window_by_edje(const Eina_List *list, const void *data);

#endif /* STATUS_H */
