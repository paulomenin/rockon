#include "error.h"

void print_error (const char *msg, Error_type type) {
	switch (type) {
		case ERR_NORMAL:
				fprintf (stderr, "ERROR: %s\n", msg);
				break;
		case ERR_CRITICAL:
				fprintf (stderr, "CRITICAL ERROR: %s\nExiting...\n", msg);
				exit (EXIT_FAILURE);
				break;
		case ERR_WARNING:
				fprintf (stdout, "WARNING: %s\n", msg);
				break;
	}
}

