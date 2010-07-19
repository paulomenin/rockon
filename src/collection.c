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

#include <assert.h>
#include "collection.h"
#include "rockon_data.h"
#include "ui_update.h"
#include "util.h"

#define DBG(...) EINA_LOG_DOM_DBG(collection_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(collection_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(collection_log_dom, __VA_ARGS__)
#define WARN(...) EINA_LOG_DOM_WARN(collection_log_dom, __VA_ARGS__)

extern int collection_log_dom;

int coll_list_item_add_inner (xmmsv_t *value, void *data);

collection_list* coll_list_new() {
	collection_list *list;

	list = (collection_list*) malloc (sizeof(collection_list));

	list->locked = 0;
	list->Collections = NULL;

	return list;
}

void coll_list_del(collection_list *list) {
	void *data;

	if (!list) return;
	assert(list);

	EINA_LIST_FREE(list->Collections, data) {
		coll_del((collection*)data);
	}
	list = NULL;
}

collection* coll_new() {
	collection *coll;

	coll = (collection*) malloc (sizeof(collection));

	coll->name = NULL;
	coll->coll = NULL;

	return coll;
}

void coll_del(collection* coll) {
	free(coll->name);
	if (coll->coll)
		xmmsv_coll_unref(coll->coll);
	
	free(coll);
	coll = NULL;
}

void coll_list_wait(collection_list *list) {
	if (list == NULL) return;
	wait_mutex(&(list->locked));
}

collection_list* coll_list_get (xmmsc_connection_t *conn, void *data) {
	collection_list *list;
	xmmsc_result_t *res;

	struct coll_fetch_params *params;

	assert(conn);

	list = coll_list_new();
	up_mutex(&(list->locked));

	params = (struct coll_fetch_params*) malloc(sizeof(struct coll_fetch_params));

	if (params == NULL) {
		EINA_LOG_CRIT("coll_fetch_params malloc failed");
		return NULL;
	}

	params->conn = ((rockon_data*)data)->connection;
	params->list = list;
	params->data = data;
	
	res = xmmsc_coll_list(((rockon_data*)data)->connection, XMMS_COLLECTION_NS_COLLECTIONS);
	xmmsc_result_notifier_set_full(res, coll_list_fetch, params, NULL);
	xmmsc_result_unref(res);

	return list;
}

int coll_list_fetch (xmmsv_t *value, void *params) {
	xmmsv_list_foreach(value, coll_list_item_add, params);
	down_mutex(&(((collection_list*)((struct coll_fetch_params*)params)->list)->locked));
	ui_upd_coll_list(((struct coll_fetch_params*)params)->data);
	free((struct coll_fetch_params*)params);
	return 1;
}

void coll_list_item_add (xmmsv_t *value, void *params) {
	const char *name;
	collection *coll;
	xmmsc_result_t *result;
	collection_list *cl = (collection_list*)(((struct coll_fetch_params*)params)->list);

	xmmsv_get_string (value, &name);

	coll = coll_new();
	coll->name = strdup(name);
	coll->coll = NULL;

	result = xmmsc_coll_get(((struct coll_fetch_params*)params)->conn, name, XMMS_COLLECTION_NS_COLLECTIONS);
	xmmsc_result_notifier_set(result, coll_list_item_add_inner, coll);
	xmmsc_result_unref(result);

	cl->Collections = eina_list_append(cl->Collections, coll);
}

int coll_list_item_add_inner (xmmsv_t *value, void *data) {
	xmmsv_get_coll(value, &(((collection*)data)->coll));
	xmmsv_coll_ref(((collection*)data)->coll);
	return 1;
}
