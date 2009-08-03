#ifndef STATUS_H
#define STATUS_H

typedef struct {
	int connected;

	char *playlist_name;
	int playlist_pos;

	int playback_id;
	int volume_left;
	int volume_right;
	
	int playtime;

} xmms_status;

#endif /* STATUS_H */
