
CC = gcc
CFLAGS = -Wall

LIBS = ecore ecore-config ecore-evas edje esmart_trans_x11 xmms2-client xmms2-client-ecore
HDR = error.h libs_setup.h gui.h config.h xmms_conn.h playback_cb.h
SRC = main.c error.c gui.c libs_setup.c config.c xmms_conn.c playback_cb.c

.PHONY: clean

all: rockon

rockon: $(SRC) $(HDR)
	$(CC) -o rockon $(CFLAGS) `pkg-config --cflags --libs $(LIBS)` $(SRC)

clean:
	rm rockon
