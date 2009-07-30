#include "playback_cb.h"

void cmd_play_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	xmmsc_result_t *result;
	result = xmmsc_playback_start (xmms_conn_async);
	xmmsc_result_unref (result);
}

void cmd_pause_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	xmmsc_result_t *result;
	result = xmmsc_playback_pause (xmms_conn_async);
	xmmsc_result_unref (result);
}

void cmd_stop_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	xmmsc_result_t *result;
	result = xmmsc_playback_stop (xmms_conn_async);
	xmmsc_result_unref (result);
}

void cmd_next_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	xmmsc_result_t *result;
	result = xmmsc_playlist_set_next_rel (xmms_conn_async, 1);
	xmmsc_result_unref (result);
	result = xmmsc_playback_tickle (xmms_conn_async);
	xmmsc_result_unref (result);
}

void cmd_prev_cb (void *data, Evas_Object *eo, const char *emission, const char *source) {
	xmmsc_result_t *result;
	result = xmmsc_playlist_set_next_rel (xmms_conn_async, -1);
	xmmsc_result_unref (result);
	result = xmmsc_playback_tickle (xmms_conn_async);
	xmmsc_result_unref (result);
}
