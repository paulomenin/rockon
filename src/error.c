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

#include <assert.h>
#include "error.h"

/** 
 * print error message
 * 
 * @Param msg message to print
 * @Param type type of error
 */
void print_error (const char *msg, Error_type type) {
	assert(msg);
	switch (type) {
		case ERR_NORMAL:
				fprintf (stderr, "ERROR: %s\n", msg);
				break;
		case ERR_CRITICAL:
				fprintf (stderr, "CRITICAL ERROR: %s\nExiting...\n", msg);
				exit (1);
				break;
		case ERR_WARNING:
				fprintf (stdout, "WARNING: %s\n", msg);
				break;
		default:
				fprintf (stderr, "ERROR with invalid type!\nExiting...\n");
				exit (1);
	}
}

