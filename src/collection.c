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

#include "collection.h"

#define DBG(...) EINA_LOG_DOM_DBG(collection_log_dom, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(collection_log_dom, __VA_ARGS__)
#define INFO(...) EINA_LOG_DOM_INFO(collection_log_dom, __VA_ARGS__)
#define WARN(...) EINA_LOG_DOM_WARN(collection_log_dom, __VA_ARGS__)

extern int collection_log_dom;

