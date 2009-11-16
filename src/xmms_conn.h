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

#ifndef XMMS_CONN_H
#define XMMS_CONN_H

#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include <string.h>
#include "error.h"
#include "status.h"

int  xmms2_connect (rockon_status *status);
void xmms2_shutdown ();

int  check_error (xmmsv_t *value, void *data);

int  broadcast_playlist_pos_cb (xmmsv_t *value, void *data);
int  broadcast_playlist_changed_cb (xmmsv_t *value, void *data);
int  broadcast_playlist_loaded_cb (xmmsv_t *value, void *data);
int  broadcast_playback_volume_cb (xmmsv_t *value, void *data);
int  broadcast_playback_id_cb (xmmsv_t *value, void *data);
int  broadcast_playback_status_cb (xmmsv_t *value, void *data);
int  signal_playback_playtime_cb (xmmsv_t *value, void *data);

/* FIXME xmmsc_disconnect_func_t xmms2_disconnect_cb (void *data); */
void xmms2_disconnect_cb (void *data);

#endif /* XMMS_CONN_H */
