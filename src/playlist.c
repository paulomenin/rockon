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

// TODO use media_info reference on playlist_item instead of copy values

#include <assert.h>
#include <string.h>
#include "playlist.h"
#include "xmms_conn.h"
#include "media_info.h"
#include "ui_update.h"
#include "rockon_data.h"
#include "util.h"

#define DBG(...) EINA_LOG_DOM_DBG(playlist_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(playlist_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(playlist_log_dom, __VA_ARGS__)

extern int playlist_log_dom;

struct pls_changed_params {
	playlist  *list;
	int position;
};

int playlist_change_item_add_inner (xmmsv_t *value, void *data);

/* playlist_list */

playlist_list *playlist_list_new() {
	playlist_list *list;

	list = (playlist_list*) malloc (sizeof(playlist_list));

	list->locked = 0;
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
		playlist_del(data);
	}
	EINA_LIST_FOREACH(list->playlists_, l, data) {
		playlist_del(data);
	}
	free(list);
	list = NULL;
}

void playlist_list_wait(playlist_list *list) {
	if (list == NULL) return;
	wait_mutex(&(list->locked));
}

playlist_list* playlist_list_get (xmmsc_connection_t *conn, void *data) {
	playlist_list *list;
	struct pls_fetch_params *params;

	assert(conn);

	list = playlist_list_new();
	up_mutex(&(list->locked));

	params = (struct pls_fetch_params*) malloc(sizeof(struct pls_fetch_params));

	if (params == NULL) {
		EINA_LOG_CRIT("pls_fetch_params malloc failed");
		return NULL;
	}

	params->conn = conn;
	params->list = list;
	params->data = data;

	XMMS_CALLBACK_SET (((rockon_data*)data)->connection,
				xmmsc_playlist_list,
				playlist_list_fetch, params);

	return list;
}

int playlist_list_fetch (xmmsv_t *value, void *params) {
	xmmsv_list_foreach(value, playlist_list_item_add, ((struct pls_fetch_params*)params)->list);
	down_mutex(&(((playlist_list*)((struct pls_fetch_params*)params)->list)->locked));
	ui_upd_playlist_list(((struct pls_fetch_params*)params)->data);
	free((struct pls_fetch_params*)params);
	return 1;
}

void playlist_list_item_add (xmmsv_t *value, void *list) {
	const char *val;
	playlist *pls;
	xmmsv_get_string (value, &val);

	pls = playlist_new();
	pls->name = strdup(val);

	if (val[0] == '_') {
		((playlist_list*)list)->playlists_ = eina_list_append(((playlist_list*)list)->playlists_, pls);
	} else {
		((playlist_list*)list)->playlists = eina_list_append(((playlist_list*)list)->playlists, pls);
	}
}

/* playlist */

playlist *playlist_new() {
	playlist *list;

	list = (playlist*) malloc (sizeof(playlist));

	list->locked = 0;
	list->name  = NULL;
	list->items = NULL;
	list->num_items = -1; // data not fetched from server
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
	list = NULL;
}

int  playlist_is_fetched(playlist *list) {
	assert(list);

	if (list->num_items < 0)
		return 0;
	else
		return 1;
}

void playlist_wait(playlist *pls) {
	if (pls == NULL) return;
	wait_mutex(&(pls->locked));
}

/* playlist_item */

void playlist_clear_items(playlist *pls) {
	void* data;
	Eina_List *l;

	if (!pls) return;

	EINA_LIST_FOREACH(pls->items, l, data) {
		playlist_item_del(data);
	}

	pls->items = NULL;
	pls->num_items = -1;
	pls->current_pos = 0;
}

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

playlist* playlist_find(playlist_list *pls_list, const char *pls_name) {
	void* list;
	Eina_List *l;

	assert(pls_name);

	if (pls_name[0] == '_') {
		EINA_LIST_FOREACH( pls_list->playlists_, l, list) {
			if (strcmp( ((playlist*)list)->name, pls_name) == 0 ) {
				return list;
			}
		}
	} else {
		EINA_LIST_FOREACH( pls_list->playlists, l, list) {
			if (strcmp( ((playlist*)list)->name, pls_name) == 0 ) {
				return list;
			}
		}
	}

	return NULL;
}

playlist* playlist_get_by_name(xmmsc_connection_t *conn, const char *pls_name, void *data) {
	playlist *pls = NULL;

	assert(conn);
	assert(pls_name);

	DBG("PLS GET BY NAME: %s", pls_name);

	pls = playlist_find(((rockon_data*)data)->playlists, pls_name);
	if (pls)
		playlist_get(conn, pls, data);

	return pls;
}

playlist *playlist_get (xmmsc_connection_t *conn, playlist *pls, void *data) {
	xmmsc_result_t *result;
	struct pls_fetch_params *params;

	DBG("PLS GET: %s", pls->name);

	assert(conn);

	params = (struct pls_fetch_params*) malloc(sizeof(struct pls_fetch_params));

	if (params == NULL) {
		EINA_LOG_CRIT("pls_fetch_params malloc failed");
		return NULL;
	}

	params->conn = conn;
	params->list = pls;
	params->data = data;

	up_mutex(&(pls->locked));

	if ( playlist_is_fetched(pls) ) {
		playlist_clear_items(pls);
	}

	result = xmmsc_playlist_list_entries(conn, pls->name);
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

	/* reset position counter */
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
	down_mutex(&(((playlist*)(data->list))->locked));
	ui_upd_playlist((rockon_data*)(data->data), (playlist*)(data->list));
	free(data);
}

void playlist_change_item_del (playlist_list *pls_list, const char* name, int position) {
	Eina_List *item;
	playlist *pls;

	pls = playlist_find(pls_list, name);

	if (pls && playlist_is_fetched(pls)) {
		Eina_List *l;
		void *data;
		item = eina_list_nth_list(pls->items, position);

		EINA_LIST_FOREACH(item , l, data) {
			((playlist_item*)data)->pos--;
		}

		playlist_item_del((playlist_item*)eina_list_data_get(item));
		pls->items = eina_list_remove_list(pls->items, item);
		pls->num_items--;
		if (pls->num_items == 0) {
			DBG("Clear all itens");
			playlist_clear_items(pls);
		}
	}

}

void playlist_change_item_add (void *rck_data, const char* name, int position, int id) {
	xmmsc_result_t *result;
	struct pls_changed_params *params;
	playlist *pls;
	rockon_data *rdata = (rockon_data*)rck_data;
	pls = playlist_find(rdata->playlists, name);
	if (pls) {
		params = (struct pls_changed_params*) malloc(sizeof(struct pls_changed_params));
		if (params == NULL) {
			EINA_LOG_CRIT("pls_changed_params malloc failed");
			return;
		}
		up_mutex(&(pls->locked));
		params->list = pls;
		params->position = position;

		result = xmmsc_medialib_get_info(rdata->connection, id);
		xmmsc_result_notifier_set (result, playlist_change_item_add_inner, params);
		xmmsc_result_unref (result);

		playlist_wait(pls);
	}
}

int playlist_change_item_add_inner (xmmsv_t *value, void *data) {
	playlist_item *pi;
	media_info *info;
	Eina_List *insert_pos, *l;
	void *item;
	struct pls_changed_params *params = (struct pls_changed_params*)data;

	if (! check_error(value, NULL)) {

		info = media_info_new();
		media_info_get(value, info);

		pi = playlist_item_new();

		pi->title = strdup(info->title);
		pi->artist = strdup(info->artist);
		pi->album = strdup(info->album);
		pi->id  = info->id;
		pi->tracknr  = info->tracknr;
		pi->pos = params->position;

		media_info_del(info);

		insert_pos = eina_list_nth_list(params->list->items, params->position);
		if (insert_pos) {
			EINA_LIST_FOREACH(insert_pos , l, item) {
				((playlist_item*)item)->pos++;
			}
			params->list->items = eina_list_prepend_relative_list(params->list->items, pi, insert_pos);
		} else {
			params->list->items = eina_list_append(params->list->items, pi);
		}
		if (params->list->num_items < 0) {
			params->list->num_items = 1;
		}else {
			params->list->num_items++;
		}
		down_mutex(&(params->list->locked));
		free(params);
		return 1;
	}
	free(params);
	return 0;
}

void playlist_change_item_moved (playlist_list *pls_list, const char* name, int position, int newposition) {
	Eina_List *item_on_newpos, *item_on_oldpos, *l;
	void *list;
	playlist *pls;

	if (position == newposition)
		return;

	pls = playlist_find(pls_list, name);

	if (pls && playlist_is_fetched(pls)) {
		int pos;
		if (newposition < position) {
			pos = 0;
			EINA_LIST_FOREACH(pls->items, l, list) {
				if (pos == position) break;
				if (pos >= newposition)
					((playlist_item*)list)->pos++;
				pos++;
			}
		} else {
			pos = 0;
			EINA_LIST_FOREACH(pls->items, l, list) {
				if (pos > position)
					((playlist_item*)list)->pos--;
				if (pos == newposition) break;
				pos++;
			}
		}

		item_on_newpos = eina_list_nth_list(pls->items, newposition);
		item_on_oldpos = eina_list_nth_list(pls->items, position);
		((playlist_item*)eina_list_data_get(item_on_oldpos))->pos = newposition;
		if (newposition < position) {
			pls->items = eina_list_prepend_relative_list(pls->items,
						 eina_list_data_get(item_on_oldpos), item_on_newpos);
		} else {
			pls->items = eina_list_append_relative_list(pls->items,
						 eina_list_data_get(item_on_oldpos), item_on_newpos);
		}
		pls->items = eina_list_remove_list(pls->items, item_on_oldpos);
	}
}

int playlist_load_current_cb (xmmsv_t *value, void *data) {
	rockon_data *rdata = (rockon_data*)data;
	const char *pls_name;

	if (! check_error(value, NULL)) {
		xmmsv_get_string(value, &pls_name);

		rdata->current_playlist = playlist_get_by_name(rdata->connection, pls_name, rdata);
		playlist_wait(rdata->current_playlist);
	}
	down_mutex(&(rdata->mutex_playlist));
	return 1;
}
