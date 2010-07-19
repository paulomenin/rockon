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

#ifndef COMMANDS_H
#define COMMANDS_H

#include "rockon_data.h"
#include <xmmsclient/xmmsclient.h>

#define XMMS_CONN_IS_VALID() { \
	if ( rdata->connection == NULL ) \
		if ( xmms2_connect (rdata) == 0 ) \
			return; \
}

void cmd_play  (rockon_data *rdata);
void cmd_pause (rockon_data *rdata);
void cmd_stop  (rockon_data *rdata);
void cmd_next  (rockon_data *rdata);
void cmd_prev  (rockon_data *rdata);
void cmd_seek_ms (rockon_data *rdata, int miliseconds);
void cmd_jump_to (rockon_data *rdata, int pos);
void cmd_jump_and_play  (rockon_data *rdata, int pos);

void cmd_server_launch  (rockon_data *rdata);
void cmd_server_shutdown(rockon_data *rdata);

void cmd_playlist_load  (rockon_data* rdata, const char *playlist);

void cmd_mlib_add_media (rockon_data* rdata, const char *path);

void cmd_volume_change  (rockon_data* rdata, const char *channel, int volume);
void cmd_volume_change_all(rockon_data* rdata, int volume);

void cmd_coll_search(rockon_data* rdata, const char *pattern);
void cmd_coll_load (rockon_data* rdata, xmmsv_coll_t* coll);
void cmd_coll_save (rockon_data* rdata, xmmsv_coll_t* coll, const char *name);
void cmd_coll_add_to_playlist(rockon_data* rdata, xmmsv_coll_t* coll, const char *playlist);

#endif /* COMMANDS_H */
