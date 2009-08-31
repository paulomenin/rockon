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

#ifndef COMMANDS_H
#define COMMANDS_H

#include <xmmsclient/xmmsclient.h>
#include "xmms_conn.h"
#include "status.h"

void cmd_play  (xmms_status *status, void *data);
void cmd_pause (xmms_status *status, void *data);
void cmd_stop  (xmms_status *status, void *data);
void cmd_next  (xmms_status *status, void *data);
void cmd_prev  (xmms_status *status, void *data);


/* Macros */
#define XMMS_CONN_IS_VALID() { \
	if (! status->connected) \
		if (! xmms2_connect (status)) \
			return; \
}

#endif /* COMMANDS_H */
