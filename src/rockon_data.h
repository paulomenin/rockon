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

#ifndef ROCKON_DATA_H
#define ROCKON_DATA_H

#include <Ecore.h>
#include <Eina.h>
#include <Elementary.h>
#include <xmmsclient/xmmsclient.h>
#include "rockon_config.h"
#include "media_info.h"
#include "playlist.h"
#include "collection.h"

typedef struct {
	Evas_Object *win;
	Evas_Object *layout;
	Evas_Object *edje;

	Evas_Object *seekbar;
	int seekbar_update;
	Evas_Object *volumebar;
	int volumebar_update;
	Evas_Object *playlist;
	Evas_Object *playlists;
	Evas_Object *coll_list;
	Evas_Object *coll_result;
	Evas_Object *coll_entry;
	Evas_Object *coll_name;
} elm_widgets;

typedef struct {
	char *name;
	int  value;
} volume_channel;

typedef struct {
	void               *ecore_fdh;
	xmmsc_connection_t *connection;
	Ecore_Timer        *reconn_timer;
	rockon_config      *config;

	int playback_status;
	int playback_id;
	int playback_playtime;
	Eina_List *volume;
	media_info *playback_info;
	
	playlist_list *playlists;
	playlist *current_playlist;
	int mutex_playlist;;
	
	collection_list *collections;
	Eina_List *coll_queried;

	elm_widgets widgets;
} rockon_data;

rockon_data* rockon_data_new();
void rockon_data_del(rockon_data *rdata);
void rockon_data_clean(rockon_data *rdata);
void rockon_data_coll_queried_clean(rockon_data *rdata);

volume_channel* volume_channel_new();
void volume_channel_del(volume_channel* channel);
void volume_del(Eina_List *volume);

#endif /* ROCKON_DATA_H */
