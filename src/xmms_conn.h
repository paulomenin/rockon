#ifndef XMMS_CONN_H
#define XMMS_CONN_H

#include <stdlib.h>
#include <xmmsclient/xmmsclient.h>
#include <xmmsclient/xmmsclient-ecore.h>
#include "error.h"
#include "status.h"

int xmms2_connect (xmmsc_connection_t **xmms_conn);
int check_error (xmmsv_t *value, void *data);

/* FIXME this function is not necessary, change for xmms2_shutdown */
void xmms2_disconnect_cb (void *data);

#endif /* XMMS_CONN_H */
