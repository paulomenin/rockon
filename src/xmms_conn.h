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
#include "rockon_data.h"
#include "rockon_config.h"

int  xmms2_connect (rockon_data *rdata);
int  xmms2_reconnect_cb (rockon_data *rdata);
void xmms2_disconnect_cb (rockon_data *rdata);
void xmms2_shutdown (rockon_data *rdata);

void xmms2_get_status (rockon_data *rdata);

int  check_error (xmmsv_t *value, void *data);

#endif /* XMMS_CONN_H */
