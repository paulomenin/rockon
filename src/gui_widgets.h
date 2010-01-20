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

#ifndef GUI_WIDGETS_H
#define GUI_WIDGETS_H

#include <Elementary.h>
#include "server_data.h"
#include "gui_window.h"

typedef struct {
	Evas_Object   *widget;
	rockon_window *window;
	int update;
} widget;

void clean_widgets(widgets_list *widgets, rockon_window *window);

void seekbar_new (server_data *sdata, const char *emission, rockon_window *window);
const char* seekbar_format_indicator(double val);

#endif /* GUI_WIDGETS_H */