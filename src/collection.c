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
#include <string.h>
#include "collection.h"
#include "xmms_conn.h"
#include "media_info.h"
#include "gui_update.h"

#define DBG(...) EINA_LOG_DOM_DBG(collection_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(collection_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(collection_log_dom, __VA_ARGS__)

extern int collection_log_dom;

/* collection */

collection *collection_new() {
	collection *coll;

	coll = (collection*) malloc (sizeof(collection));

	coll->locked = 0;
	coll->items = NULL;
	coll->num_items = -1;

	return coll;
}

void collection_del (collection *coll) {
	void* data;
	Eina_List *l;

	if (!coll) return;

	EINA_LIST_FOREACH(coll->items, l, data) {
		collection_item_del(data);
	}

	free(coll);
}

void collection_wait(collection *coll) {
	if (coll == NULL) return;

	while (coll->locked == 1) {
		ecore_main_loop_iterate();
	}
}

/* collection_item */

collection_item *collection_item_new() {
	collection_item *item;

	item = (collection_item*) malloc (sizeof(collection_item));

	item->id  = 0;
	item->tracknr = 0;
	item->title  = NULL;
	item->artist = NULL;
	item->album  = NULL;

	return item;
}

void collection_item_del (collection_item *item) {
	if (!item) return;

	free(item->title);
	free(item->artist);
	free(item->album);

	free(item);
}

/* collection get */

collection *collection_get (xmmsc_connection_t *conn, const char *pattern, void *data) {
	xmmsc_result_t *result;
	collection *coll;
	xmmsv_coll_t *coll_query;
	struct coll_fetch_params *params;

	DBG("COLL GET: %s", pattern);

	assert(conn);
	//assert(pattern);

	coll = collection_new();

	params = (struct coll_fetch_params*) malloc(sizeof(struct coll_fetch_params));

	if (params == NULL) {
		EINA_LOG_CRIT("coll_fetch_params malloc failed");
		return NULL;
	}

	xmmsc_coll_parse(pattern, &coll_query);

	params->conn = conn;
	params->coll = coll_query;
	params->list = coll;
	params->data = data;

	coll->locked = 1;
	result = xmmsc_coll_query_ids(conn, coll_query, NULL, 0, 0);
	xmmsc_result_notifier_set (result, collection_fetch, params);
	xmmsc_result_unref (result);

	DBG("PLS GET END");
	return coll;
}

int collection_fetch(xmmsv_t *value, void *data) {
	xmmsv_list_iter_t *it;
	xmmsc_result_t *result;

	DBG("COLL FETCH START");

	if (! check_error(value, NULL)) {
		if (!xmmsv_get_list_iter (value, &it)) {
			ERR("get list iterator failed!");
			return 0;
		}

		((collection*)(((struct coll_fetch_params*)data)->list))->num_items = xmmsv_list_get_size(value);

		for (; xmmsv_list_iter_valid (it); xmmsv_list_iter_next (it)) {
			int id;

			xmmsv_t *list_entry;

			xmmsv_list_iter_entry (it, &list_entry);
			xmmsv_get_int (list_entry, &id);

			result = xmmsc_medialib_get_info( ((struct coll_fetch_params*)data)->conn, id);
			xmmsc_result_notifier_set (result, collection_item_add, data);
			xmmsc_result_unref (result);
		}
		DBG("PLS FETCH END");
		return 1;
	}
	free(data);
	return 0;
}

int collection_item_add(xmmsv_t *value, void *data) {
	collection_item *ci;
	media_info *info;
	struct coll_fetch_params *params = (struct coll_fetch_params*)data;

	assert(value);
	assert(data);

	if (! check_error(value, NULL)) {

		info = media_info_new();
		media_info_get(value, info);

		ci = collection_item_new();

		ci->title = strdup(info->title);
		ci->artist = strdup(info->artist);
		ci->album = strdup(info->album);
		ci->id  = info->id;
		ci->tracknr  = info->tracknr;

		media_info_del(info);

		((collection*)(params->list))->items = eina_list_append(((collection*)(params->list))->items, ci);

		if (collection_is_ready((collection*)(params->list))) {
			collection_fetched(params);
		}

		return 1;
	}
	return 0;
}

int  collection_is_ready (collection *list) {
	if (eina_list_count(list->items) == list->num_items) {
		return 1;
	}
	return 0;
}

void collection_fetched(struct coll_fetch_params* data) {
	DBG("COLL fetched");
	if (((collection*)(data->list)) == ((server_data*)(data->data))->collection_current) {
		gui_upd_collection((server_data*)(data->data));
		((collection*)(data->list))->locked = 0;
	}
	xmmsc_coll_unref(data->coll);
	free(data);
}
