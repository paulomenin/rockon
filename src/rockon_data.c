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
#include "rockon_data.h"
#include "xmms_conn.h"

rockon_data* rockon_data_new() {
	rockon_data *rdata;

	rdata = (rockon_data*) malloc(sizeof(rockon_data));
	if (rdata == NULL) {
		EINA_LOG_CRIT("malloc rockon_data failed");
	}

	rdata->ecore_fdh = NULL;
	rdata->connection = NULL;
	rdata->reconn_timer = NULL;
	rdata->config = config_new();

	rdata->playback_status = 0;
	rdata->playback_id = 0;
	rdata->playback_playtime = 0;
	rdata->playback_info = NULL;
	rdata->volume = NULL;

	rdata->playlists = NULL;
	rdata->current_playlist = NULL;

	return rdata;
}

void rockon_data_del(rockon_data* rdata) {
	assert(rdata);

	rockon_data_clean(rdata);

	if (rdata->config != NULL) {
		config_save(rdata->config);
		config_del(rdata->config);
	}

	free(rdata);
}

void rockon_data_clean(rockon_data *rdata) {
	assert(rdata);

	if (rdata->playback_info != NULL) {
		media_info_del(rdata->playback_info);
	}
	if (rdata->playlists != NULL) {
		playlist_list_del(rdata->playlists);
	}
	if (rdata->volume != NULL) {
		volume_del(rdata->volume);
	}
	
	xmms2_shutdown(rdata);
}

volume_channel* volume_channel_new() {
	volume_channel *vol = NULL;

	vol = (volume_channel*) malloc(sizeof(volume_channel));
	if (vol) {
		vol->name = NULL;
		vol->value = 0;
	}
	return vol;
}

void volume_channel_del(volume_channel* channel) {
	if (channel == NULL) return;

	if (channel->name) {
		free(channel->name);
	}
	free(channel);
}

void volume_del(Eina_List *volume) {
	volume_channel *channel;

	if (volume == NULL) return;

	EINA_LIST_FREE(volume, channel) {
		volume_channel_del(channel);
	}
}
