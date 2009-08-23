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

#include "libs_setup.h"

extern xmmsc_connection_t *xmms_conn_async;
extern xmms_status app_status;

int efl_init () {
	ecore_init ();
	ecore_config_init ("Rockon");
	evas_init ();
	ecore_evas_init ();
	edje_init();
	ecore_file_init();
	return TRUE;
}

void efl_shutdown () {
	edje_shutdown ();
	ecore_evas_shutdown ();
	evas_shutdown ();
	ecore_config_shutdown ();
	ecore_shutdown ();
	ecore_file_shutdown();

	printf ("DEBUG: shutdown\n");
}

/* FIXME maybe move to xmms_conn.c ? */
void xmms2_shutdown (xmms_status* status) {
	if ((status) && (status->connection)) {
		xmmsc_unref (status->connection);
		status->connection = NULL;
		status->connected = 0;
	}
}
