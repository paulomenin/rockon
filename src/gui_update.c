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

#include "gui_update.h"

#define DBG(...) EINA_LOG_DOM_DBG(gui_upd_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(gui_upd_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(gui_upd_log_dom, __VA_ARGS__)

extern int gui_upd_log_dom;

void gui_upd_playback_id (server_data *sdata) {
	INFO("Playback ID: %d", sdata->playback_id);
}

void gui_upd_playback_status (server_data *sdata) {
	INFO("Playback STATUS: %d", sdata->playback_status);
}

void gui_upd_playback_playtime (server_data *sdata) {
	/*INFO("Playback Playtime: %d:%02d",
			(sdata->playback_playtime / 1000)/60,
			(sdata->playback_playtime / 1000)%60);
	*/
}


