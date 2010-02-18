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

#ifndef GUI_UPDATE_H
#define GUI_UPDATE_H

#include "server_data.h"

enum Message_id {
	PLAYBACK_PLAYTIME = 1,
	PLAYBACK_STATUS   = 2,
	PLAYBACK_INFO     = 3,
	PLAYBACK_DURATION = 4,
	PLAYBACK_ID       = 5,

	MLIB_STATUS_FILES = 6 // remaining files to process
};

void gui_upd_playback_id       (server_data *sdata);
void gui_upd_playback_status   (server_data *sdata);
void gui_upd_playback_playtime (server_data *sdata);
void gui_upd_playback_info     (server_data *sdata);
void gui_upd_playlist          (server_data *sdata);
void gui_upd_playlist_pos      (server_data *sdata);
void gui_upd_playlist_list     (server_data *sdata);
void gui_upd_collection        (server_data *sdata);
void gui_upd_mlib_reader_status(server_data *sdata, int files);

#endif /* GUI_UPDATE_H */
