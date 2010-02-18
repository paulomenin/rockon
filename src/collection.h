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

#ifndef COLLECTION_H
#define COLLECTION_H

#include <Elementary.h>
#include <xmmsclient/xmmsclient.h>

typedef struct {
	int locked;
	Eina_List *items;
	int num_items;
} collection;

typedef struct {
	int id;
	int tracknr;
	char *title;
	char *artist;
	char *album;
} collection_item;

struct coll_fetch_params {
	xmmsc_connection_t *conn;
	xmmsv_coll_t *coll;
	void *list;
	void *data;
};

collection *collection_new();
void collection_del (collection *coll);

collection_item *collection_item_new();
void collection_item_del (collection_item *item);

collection* collection_get (xmmsc_connection_t *conn, const char *pattern, void *data);
void collection_wait     (collection *coll);
int  collection_fetch    (xmmsv_t *value, void *data);
int  collection_item_add (xmmsv_t *value, void *data);
int  collection_is_ready (collection *coll);
void collection_fetched  (struct coll_fetch_params* data);

#endif /* COLLECTION_H */
