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

#include "util.h"

void dump_list(xmmsv_t *list) {
	xmmsv_list_iter_t *it;

	xmmsv_get_list_iter(list, &it);
	for (; xmmsv_list_iter_valid (it); xmmsv_list_iter_next (it)) {
		xmmsv_t *list_entry;
		xmmsv_list_iter_entry(it, &list_entry);
		dump_xmms_value(list_entry);
	}
}

void dump_dict(xmmsv_t *dict) {
	xmmsv_dict_foreach (dict, dict_foreach, NULL);
}

void dict_foreach(const char *key, xmmsv_t *value, void *user_data) {
	printf("KEY: %s", key);
	dump_xmms_value(value);
}

void dump_xmms_value(xmmsv_t *value) {
	xmmsv_type_t type;
	int number;
	const char *buf;

	type = xmmsv_get_type(value);
	
	printf("\nTYPE: ");
	switch (type) {
		case XMMSV_TYPE_NONE:
			printf("none\n");
			break;
		case XMMSV_TYPE_ERROR:
			printf("error\n");
			xmmsv_get_error(value, &buf);
			printf("Value: %s\n", buf);
			break;
		case XMMSV_TYPE_INT32:
			printf("int\n");
			xmmsv_get_int(value, &number);
			printf("Value: %d\n", number);
			break;
		case XMMSV_TYPE_STRING:
			printf("string\n");
			xmmsv_get_string(value, &buf);
			printf("Value: %s\n", buf);
			break;
		case XMMSV_TYPE_COLL:
			printf("coll\n");
			break;
		case XMMSV_TYPE_BIN:
			printf("bin\n");
			break;
		case XMMSV_TYPE_LIST:
			printf("list\n--- LIST START ---\n");
			dump_list(value);
			printf("--- LIST END ---\n");
			break;
		case XMMSV_TYPE_DICT:
			printf("dict\n");
			dump_dict(value);
			printf("--- DICT END ---\n");
			break;
		case XMMSV_TYPE_END:
			printf("end\n");
			break;
		default:
			printf("Undefined, this is bad!\n");
	}
	fflush(stdout);
}
