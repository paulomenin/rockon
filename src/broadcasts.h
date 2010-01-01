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

#ifndef BROADCASTS_H
#define BROADCASTS_H

#include <xmmsclient/xmmsclient.h>

int  broadcast_playback_status_cb  (xmmsv_t *value, void *data);
int  broadcast_playback_id_cb      (xmmsv_t *value, void *data);
int  signal_playback_playtime_cb   (xmmsv_t *value, void *data);

#endif /* BROADCASTS_H */
