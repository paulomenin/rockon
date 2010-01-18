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

#include "elm_callbacks.h"
#include "gui_window.h"

void elm_cb_set (void *data, Evas_Object *eo, const char *emission, const char *source) {
	server_data *sdata = (server_data*)data;

	if (strcmp(source, "elm_set,win") == 0) {
		window_new(sdata, emission);
	}
}
