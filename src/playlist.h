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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <Eina.h>
#include <xmmsclient/xmmsclient.h>

typedef struct {
	int locked;
	Eina_List *playlists;
	Eina_List *playlists_; // playlists starting with _ are not visible to user
} playlist_list;

typedef struct {
	int locked;
	char *name;
	Eina_List *items;
	int num_items;
	int current_pos;
} playlist;

typedef struct {
	int id;
	int pos;
	int tracknr;
	char *title;
	char *artist;
	char *album;
} playlist_item;

struct pls_fetch_params {
	xmmsc_connection_t *conn;
	void *list;
	void *data;
};

playlist_list *playlist_list_new();
void playlist_list_del      (playlist_list *list);
void playlist_list_wait     (playlist_list *list);

playlist_list* playlist_list_get (xmmsc_connection_t *conn, void *data);
int  playlist_list_fetch    (xmmsv_t *value, void *list);
void playlist_list_item_add (xmmsv_t *value, void *list);

playlist *playlist_new();
void playlist_del (playlist *list);

playlist_item *playlist_item_new();
void playlist_item_del (playlist_item *item);
void playlist_clear_items(playlist *pls);

playlist* playlist_get (xmmsc_connection_t *conn, playlist *pls, void *data);
playlist* playlist_get_by_name (xmmsc_connection_t *conn, const char *pls_name, void *data);
void playlist_wait     (playlist *pls);
int  playlist_fetch    (xmmsv_t *value, void *data);
int  playlist_item_add (xmmsv_t *value, void *data);
int  playlist_is_ready (playlist *list);
void playlist_fetched  (struct pls_fetch_params* data);
int  playlist_is_fetched(playlist *list);
playlist* playlist_find(playlist_list *pls_list, const char *pls_name);

void playlist_change_item_del (playlist_list *pls_list, const char* name, int position);
void playlist_change_item_add (void *rck_data, const char* name, int position, int id);
void playlist_change_item_moved (playlist_list *pls_list, const char* name, int position, int newposition);

#endif /* PLAYLIST_H */
