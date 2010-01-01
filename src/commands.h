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

#include "server_data.h"

#define XMMS_CONN_IS_VALID() { \
	if ( sdata->connection == NULL ) \
		if ( xmms2_connect (sdata) == 0 ) \
			return; \
}

void cmd_play  (server_data *sdata);
void cmd_pause (server_data *sdata);
void cmd_stop  (server_data *sdata);
void cmd_next  (server_data *sdata);
void cmd_prev  (server_data *sdata);
void cmd_jump_to (server_data *sdata, int pos);
void cmd_jump_and_play (server_data *sdata, int pos);
void cmd_server_launch(server_data *sdata);
void cmd_server_shutdown(server_data *sdata);

#endif /* COMMANDS_H */
