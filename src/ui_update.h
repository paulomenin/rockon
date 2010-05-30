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

#ifndef UI_UPDATE_H
#define UI_UPDATE_H

#include "rockon_data.h"
#include "playlist.h"

void ui_upd_playback_id       (rockon_data *rdata);
void ui_upd_playback_status   (rockon_data *rdata);
void ui_upd_playback_playtime (rockon_data *rdata);
void ui_upd_playback_info     (rockon_data *rdata);
void ui_upd_playlist          (rockon_data *rdata, playlist* pls);
void ui_upd_playlist_pos      (rockon_data *rdata);
void ui_upd_playlist_list     (rockon_data *rdata);
void ui_upd_mlib_reader_status(rockon_data *rdata, int files);

#endif /* UI_UPDATE_H */
