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
#include <string.h>
#include "media_info.h"
#include "xmms_conn.h"


media_info* media_info_new() {
	media_info* info = NULL;
	
	info = (media_info*) malloc(sizeof(media_info));
	if (! info)
		print_error ("Couldn't allocate memory for media info.", ERR_CRITICAL);

	info->artist = NULL;
	info->album = NULL;
	info->title = NULL;
	info->url = NULL;
	info->comment = NULL;
	info->genre = NULL;
	info->date = NULL;
	info->bitrate = 0;
	info->duration = 0;

	return info;
}

void media_info_del (media_info* info) {
	assert(info);

	free(info->artist);
	free(info->album);
	free(info->title);
	free(info->url);
	free(info->comment);
	free(info->genre);
	free(info->date);

	free(info);
}

void media_info_get (xmmsv_t *value, media_info *info) {
	xmmsv_t *infos, *dict_entry;
	const char *artist, *album, *title, *url, *comment, *genre, *date;

	assert(value);
	assert(info);

	infos = xmmsv_propdict_to_dict(value, NULL);

	if (!xmmsv_dict_get (infos, "artist", &dict_entry) ||
	    !xmmsv_get_string (dict_entry, &artist)) {
		artist = "[Unknown Artist]";
	}
	if (!xmmsv_dict_get (infos, "album", &dict_entry) ||
	    !xmmsv_get_string (dict_entry, &album)) {
		album = "[Unknown Album]";
	}
	if (!xmmsv_dict_get (infos, "title", &dict_entry) ||
	    !xmmsv_get_string (dict_entry, &title)) {
		title = "[Unknown Title]";
	}
	if (!xmmsv_dict_get (infos, "url", &dict_entry) ||
	    !xmmsv_get_string (dict_entry, &url)) {
		url = "[Unknown URL]";
	}
	if (!xmmsv_dict_get (infos, "comment", &dict_entry) ||
	    !xmmsv_get_string (dict_entry, &comment)) {
		comment = "";
	}
	if (!xmmsv_dict_get (infos, "genre", &dict_entry) ||
	    !xmmsv_get_string (dict_entry, &genre)) {
		genre = "[Unknown Genre]";
	}
	if (!xmmsv_dict_get (infos, "date", &dict_entry) ||
	    !xmmsv_get_string (dict_entry, &date)) {
		date = "";
	}
	
	if (!xmmsv_dict_get (infos, "duration", &dict_entry) ||
	    !xmmsv_get_int (dict_entry, &info->duration)) {
		info->duration = 0;
	}

	info->artist = strdup(artist);
	info->album = strdup(album);
	info->title = strdup(title);
	info->url = strdup(url);
	info->comment = strdup(comment);
	info->genre = strdup(genre);
	info->date = strdup(date);

	xmmsv_unref(infos);
}
