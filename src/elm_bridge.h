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

#ifndef ELM_BRIDGE_H
#define ELM_BRIDGE_H

#include "commands.h"
#include "edje_bridge.h"
#include "playlist.h"

void elm_cb_set (void *data, Evas_Object *eo, const char *emission, const char *source);

void elm_cb_play(void *data, Evas_Object *obj, void *event_info);
void elm_cb_pls_click(void *data, Evas_Object *obj, void *event_info);

void win_del_cb (void *data, Evas_Object *obj, void *event_info);
void win_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source);
void app_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source);

#endif /* ELM_BRIDGE_H */
