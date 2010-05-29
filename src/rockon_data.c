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
#include <Eina.h>
#include "rockon_data.h"

rockon_data* rockon_data_new() {
	rockon_data *rdata;

	rdata = (rockon_data*) malloc(sizeof(rockon_data));
	if (rdata == NULL) {
		EINA_LOG_CRIT("malloc rockon_data failed");
	}

	rdata->config = config_new();

	return rdata;
}

void rockon_data_del(rockon_data *rdata) {
	assert(rdata);

	if (rdata->config != NULL) {
		config_save(rdata->config);
		config_del(rdata->config);
	}

	free(rdata);
}
