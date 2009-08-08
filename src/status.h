#ifndef STATUS_H
#define STATUS_H

#include <xmmsclient/xmmsclient.h>

typedef struct {
	int connected;
	xmmsc_connection_t *connection;

	Evas_Object *edje_gui;

	char *playlist_name;
	int playlist_pos;

	int playback_id;
	int volume_left;
	int volume_right;
	
	int playtime;

} xmms_status;

void status_gui_update(xmms_status *status);

#endif /* STATUS_H */
/*
char pos[5];
snprintf(pos, 5, "%d", app_status.playback_id);
edje_object_signal_emit(edje_o , pos, "rockon_playback_id");
*/
