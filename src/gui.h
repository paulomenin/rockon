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

#ifndef GUI_H
#define GUI_H

#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Edje.h>

#include "libs_setup.h"
#include "error.h"
#include "playback_cb.h"
#include "config.h"

int gui_setup(rockon_config *config, xmms_status *status);

void win_del_cb (Ecore_Evas *ee);
void win_resize_cb (Ecore_Evas *ee);
void win_move_cb (Ecore_Evas *ee);

void app_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source);

#endif /* GUI_H */
