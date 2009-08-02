#ifndef PLAYBACK_CB_H
#define PLAYBACK_CB_H

#include <stdio.h>
#include <Evas.h>
#include <xmmsclient/xmmsclient.h>
#include "xmms_conn.h"
#include "status.h"

extern xmmsc_connection_t *xmms_conn_async;

void cmd_play_cb (void *data, Evas_Object *eo, const char *emission, const char *source);
void cmd_pause_cb (void *data, Evas_Object *eo, const char *emission, const char *source);
void cmd_stop_cb (void *data, Evas_Object *eo, const char *emission, const char *source);
void cmd_next_cb (void *data, Evas_Object *eo, const char *emission, const char *source);
void cmd_prev_cb (void *data, Evas_Object *eo, const char *emission, const char *source);


/* Macros */

#define XMMS_CONN_IS_VALID() { \
	if (! app_status.connected) \
		if (! xmms2_connect (&xmms_conn_async)) \
			return; \
}

#endif /* PLAYBACK_CB_H */
