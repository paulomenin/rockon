
CC = gcc
CFLAGS = -Wall

LIBS = ecore ecore-evas edje esmart_trans_x11 xmms2-client xmms2-client-ecore
HDR = playback_cb.h
SRC = main.c playback_cb.c

.PHONY: clean

all: rockon

rockon: $(SRC) $(HDR)
	$(CC) -o rockon $(CFLAGS) `pkg-config --cflags --libs $(LIBS)` $(SRC)

clean:
	rm rockon
