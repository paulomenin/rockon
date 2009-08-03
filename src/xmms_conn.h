#ifndef XMMS_CONN_H
#define XMMS_CONN_H

#include <stdlib.h>
#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include "error.h"
#include "status.h"

int xmms2_connect (xmmsc_connection_t **xmms_conn);
int check_error (xmmsv_t *value, void *data);

int broadcast_playlist_pos_cb (xmmsv_t *value, void *data);
int broadcast_playback_volume_cb (xmmsv_t *value, void *data);
int broadcast_playback_id_cb (xmmsv_t *value, void *data);
//int signal_playback_playtime_cb (xmmsv_t *value, void *data);

/* FIXME xmmsc_disconnect_func_t xmms2_disconnect_cb (void *data); */
void xmms2_disconnect_cb (void *data);

#endif /* XMMS_CONN_H */
