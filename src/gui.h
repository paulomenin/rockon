#ifndef GUI_H
#define GUI_H

#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Evas.h>
#include <Edje.h>
#include <Esmart/Esmart_Trans_X11.h>

#include "libs_setup.h"
#include "error.h"
#include "playback_cb.h"
#include "config.h"

int gui_setup(rockon_config *config, xmms_status *status);

void win_del_cb (Ecore_Evas *ee);
void win_resize_cb (Ecore_Evas *ee);
void win_move_cb (Ecore_Evas *ee);

void app_close_cb (void *data, Evas_Object *eo, const char *emission, const char *source);

#endif /* GUI_H */
