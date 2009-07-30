#ifndef LIBS_SETUP_H
#define LIBS_SETUP_H

#include <Evas.h>
#include <Ecore.h>
#include <Ecore_Config.h>
#include <Ecore_Evas.h>
#include <Edje.h>
#include <xmmsclient/xmmsclient.h>

int efl_init ();
void efl_shutdown ();
void xmms2_shutdown ();

#endif /* LIBS_SETUP_H */
