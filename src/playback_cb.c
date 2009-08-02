#include "playback_cb.h"

extern xmms_status app_status;

void cmd_play_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (xmms_conn_async, xmmsc_playback_start,
					check_error, NULL);
}

void cmd_pause_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (xmms_conn_async, xmmsc_playback_pause,
					check_error, NULL);
}

void cmd_stop_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	XMMS_CONN_IS_VALID();
	XMMS_CALLBACK_SET (xmms_conn_async, xmmsc_playback_stop,
					check_error, NULL);
}

void cmd_next_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	XMMS_CONN_IS_VALID();
	xmmsc_result_t *result;
	result = xmmsc_playlist_set_next_rel (xmms_conn_async, 1);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (xmms_conn_async, xmmsc_playback_tickle,
					check_error, NULL);
}

void cmd_prev_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	XMMS_CONN_IS_VALID();
	xmmsc_result_t *result;
	result = xmmsc_playlist_set_next_rel (xmms_conn_async, -1);
	xmmsc_result_notifier_set (result, check_error, NULL);
	xmmsc_result_unref (result);
	XMMS_CALLBACK_SET (xmms_conn_async, xmmsc_playback_tickle, check_error, NULL);
}
