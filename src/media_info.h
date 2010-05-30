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

#ifndef MEDIA_INFO_H
#define MEDIA_INFO_H

#include <xmmsclient/xmmsclient.h>

typedef struct {
	int id;
	char *artist;
	char *album;
	char *title;
	char *url;
	char *comment;
	char *genre;
	char *date;
	int bitrate;
	int duration;
	int tracknr;
} media_info;

media_info* media_info_new();
void media_info_del(media_info* info);
void media_info_get (xmmsv_t *value, media_info* info);

#endif /* MEDIA_INFO_H */
