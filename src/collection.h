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

#include <Eina.h>
#include <xmmsclient/xmmsclient.h>

typedef struct {
	int locked;
	Eina_List *Collections;
} collection_list;

typedef struct {
	char *name;
	xmmsv_coll_t *coll;
} collection;

struct coll_fetch_params {
	xmmsc_connection_t *conn;
	void *list;
	void *data;
};

collection_list* coll_list_new();
void coll_list_del(collection_list *list);
void coll_list_wait(collection_list *list);
collection_list* coll_list_get (xmmsc_connection_t *conn, void *data);
int coll_list_fetch (xmmsv_t *value, void *params);
void coll_list_item_add (xmmsv_t *value, void *list);

collection* coll_new();
void coll_del(collection* coll);

#endif /* COLLECTION_H */
