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
#include "playlist.h"
#include "xmms_conn.h"
#include "media_info.h"
#include "gui_update.h"

#define DBG(...) EINA_LOG_DOM_DBG(playlist_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(playlist_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(playlist_log_dom, __VA_ARGS__)

extern int playlist_log_dom;

/* playlist_list */

playlist_list *playlist_list_new() {
	playlist_list *list;

	list = (playlist_list*) malloc (sizeof(playlist_list));

	list->playlists  = NULL;
	list->playlists_ = NULL;

	return list;
}

void playlist_list_del(playlist_list *list) {
	void *data;
	Eina_List *l;

	if (!list) return;
	assert(list);

	EINA_LIST_FOREACH(list->playlists, l, data) {
		free(data);
	}
	EINA_LIST_FOREACH(list->playlists_, l, data) {
		free(data);
	}
	free(list);
}

playlist_list* playlist_list_get (xmmsc_connection_t *conn, void *data) {
	playlist_list *list;
	struct pls_fetch_params *params;

	assert(conn);

	list = playlist_list_new();

	params = (struct pls_fetch_params*) malloc(sizeof(struct pls_fetch_params));

	if (params == NULL) {
		EINA_LOG_CRIT("pls_fetch_params malloc failed");
		return NULL;
	}

	params->conn = conn;
	params->list = list;
	params->data = data;

	XMMS_CALLBACK_SET (((server_data*)data)->connection,
				xmmsc_playlist_list,
				playlist_list_fetch, params);

	return list;
}

int playlist_list_fetch (xmmsv_t *value, void *params) {
	xmmsv_list_foreach(value, playlist_list_item_add, ((struct pls_fetch_params*)params)->list);
	gui_upd_playlist_list(((struct pls_fetch_params*)params)->data);
	return 1;
}

void playlist_list_item_add (xmmsv_t *value, void *list) {
	const char *val;
	xmmsv_get_string (value, &val);
	if (val[0] == '_') {
		((playlist_list*)list)->playlists_ = eina_list_append(((playlist_list*)list)->playlists_, strdup(val));
	} else {
		((playlist_list*)list)->playlists = eina_list_append(((playlist_list*)list)->playlists, strdup(val));
	}
}

/* playlist */

playlist *playlist_new() {
	playlist *list;

	list = (playlist*) malloc (sizeof(playlist));

	list->locked = 0;
	list->name  = NULL;
	list->items = NULL;
	list->num_items = -1;
	list->current_pos = 0;

	return list;
}

void playlist_del (playlist *list) {
	void* data;
	Eina_List *l;

	if (!list) return;

	free(list->name);
	EINA_LIST_FOREACH(list->items, l, data) {
		playlist_item_del(data);
	}

	free(list);
}

void playlist_wait(playlist *pls) {
	if (pls == NULL) return;

	while (pls->locked == 1) {
		ecore_main_loop_iterate();
	}
}

/* playlist_item */

playlist_item *playlist_item_new() {
	playlist_item *item;

	item = (playlist_item*) malloc (sizeof(playlist_item));

	item->id  = 0;
	item->pos = 0;
	item->tracknr = 0;
	item->title  = NULL;
	item->artist = NULL;
	item->album  = NULL;

	return item;
}

void playlist_item_del (playlist_item *item) {
	if (!item) return;

	free(item->title);
	free(item->artist);
	free(item->album);

	free(item);
}

/* playlist get */

playlist *playlist_get (xmmsc_connection_t *conn, const char *pls_name, void *data) {
	xmmsc_result_t *result;
	playlist *pls;
	struct pls_fetch_params *params;

	DBG("PLS GET: %s", pls_name);

	assert(conn);

	pls = playlist_new();
	pls->name = strdup(pls_name);

	params = (struct pls_fetch_params*) malloc(sizeof(struct pls_fetch_params));

	if (params == NULL) {
		EINA_LOG_CRIT("pls_fetch_params malloc failed");
		return NULL;
	}

	params->conn = conn;
	params->list = pls;
	params->data = data;

	pls->locked = 1;
	result = xmmsc_playlist_list_entries(conn, pls_name);
	xmmsc_result_notifier_set (result, playlist_fetch, params);
	xmmsc_result_unref (result);

	DBG("PLS GET END");
	return pls;
}

int playlist_fetch(xmmsv_t *value, void *data) {
	xmmsv_list_iter_t *it;
	xmmsc_result_t *result;

	DBG("PLS FETCH START");

	if (! check_error(value, NULL)) {
		if (!xmmsv_get_list_iter (value, &it)) {
			ERR("get list iterator failed!");
			return 0;
		}

		playlist_item_add(NULL, NULL); // restart pos counter

		((playlist*)(((struct pls_fetch_params*)data)->list))->num_items = xmmsv_list_get_size(value);

		for (; xmmsv_list_iter_valid (it); xmmsv_list_iter_next (it)) {
			int id;

			xmmsv_t *list_entry;

			xmmsv_list_iter_entry (it, &list_entry);
			xmmsv_get_int (list_entry, &id);

			result = xmmsc_medialib_get_info( ((struct pls_fetch_params*)data)->conn, id);
			xmmsc_result_notifier_set (result, playlist_item_add, data);
			xmmsc_result_unref (result);
		}
		DBG("PLS FETCH END");
		return 1;
	}
	free(data);
	return 0;
}

int playlist_item_add(xmmsv_t *value, void *data) {
	static int pos = 0;
	playlist_item *pi;
	media_info *info;
	struct pls_fetch_params *params = (struct pls_fetch_params*)data;

	if (data == NULL) {
		pos = 0;
		return 1;
	}

	assert(value);

	if (! check_error(value, NULL)) {

		info = media_info_new();
		media_info_get(value, info);

		pi = playlist_item_new();

		pi->title = strdup(info->title);
		pi->artist = strdup(info->artist);
		pi->album = strdup(info->album);
		pi->id  = info->id;
		pi->tracknr  = info->tracknr;
		pi->pos = pos;
		pos++;

		media_info_del(info);

		((playlist*)(params->list))->items = eina_list_append(((playlist*)(params->list))->items, pi);

		if (playlist_is_ready((playlist*)(params->list))) {
			playlist_fetched(params);
		}

		return 1;
	}
	return 0;
}

int  playlist_is_ready (playlist *list) {
	if (eina_list_count(list->items) == list->num_items) {
		return 1;
	}
	return 0;
}

void playlist_fetched(struct pls_fetch_params* data) {
	DBG("PLS fetched");
	if (((playlist*)(data->list)) == ((server_data*)(data->data))->playlist_current) {
		gui_upd_playlist((server_data*)(data->data));
		((playlist*)(data->list))->locked = 0;
	}

	free(data);
}
