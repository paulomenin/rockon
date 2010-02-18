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

#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include <Ecore.h>
#include <Eina.h>
#include <xmmsclient/xmmsclient.h>
#include "rockon_config.h"
#include "media_info.h"
#include "playlist.h"
#include "collection.h"

typedef struct {
	Eina_List *seekbars;
	Eina_List *playlist_lists;
	Eina_List *playlists;
	Eina_List *file_selectors;
} widgets_list;

typedef struct {
	void               *ecore_fdh;
	rockon_config      *config;
	xmmsc_connection_t *connection;
	Ecore_Timer        *reconn_timer;

	Eina_List *windows;
	widgets_list *widgets;

	int playback_status;
	int playback_playtime;
	int playback_id;
	media_info *playback_info;

	playlist_list *playlists;
	playlist      *playlist_current;
	
	collection    *collection_current; // used in search

} server_data;

server_data* server_data_new();
void server_data_del(server_data *sdata);

#endif /* SERVER_DATA_H */
