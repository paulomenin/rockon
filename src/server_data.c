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
#include <Eina.h>
#include "server_data.h"
#include "xmms_conn.h"
#include "gui_window.h"

server_data* server_data_new() {
	server_data *sdata;

	sdata = (server_data*) malloc(sizeof(server_data));
	if (sdata == NULL) {
		EINA_LOG_CRIT("malloc server_data failed");
	}

	sdata->widgets = (widgets_list*) malloc(sizeof(widgets_list));
	if (sdata->widgets == NULL) {
		EINA_LOG_CRIT("malloc server_data widgets failed");
	}

	sdata->ecore_fdh = NULL;
	sdata->config = config_new();
	sdata->connection = NULL;
	sdata->reconn_timer = NULL;

	sdata->windows = NULL;
	sdata->widgets->seekbars = NULL;

	sdata->playback_status = 0;
	sdata->playback_playtime = 0;
	sdata->playback_id = 0;
	sdata->playback_info = NULL;

	sdata->playlists = NULL;
	sdata->playlist_current = NULL;

	return sdata;
}

void server_data_del(server_data *sdata) {
	rockon_window *data;
	Eina_List *l;

	assert(sdata);

	config_save(sdata->config);
	config_del(sdata->config);
	media_info_del(sdata->playback_info);
	playlist_list_del(sdata->playlists);
	playlist_del(sdata->playlist_current);
	xmms2_shutdown(sdata);

	if (sdata->windows) {
		EINA_LIST_FOREACH(sdata->windows, l, data) {
			free(data->name);
			free(data);
		}
	}

	if (sdata->widgets) {
		EINA_LIST_FOREACH(sdata->widgets->seekbars, l, data) {
			free(data);
		}

		free(sdata->widgets);
	}

	free(sdata);
}
