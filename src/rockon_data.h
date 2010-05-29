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

#ifndef ROCKON_DATA_H
#define ROCKON_DATA_H

#include <Ecore.h>
#include <Eina.h>
#include <xmmsclient/xmmsclient.h>
#include "rockon_config.h"

typedef struct {
	rockon_config      *config;

} rockon_data;

rockon_data* rockon_data_new();
void rockon_data_del(rockon_data *rdata);

#endif /* ROCKON_DATA_H */
