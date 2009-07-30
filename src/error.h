#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include "libs_setup.h"

typedef enum {
	ERR_NORMAL,
	ERR_CRITICAL,
	ERR_WARNING
} Error_type;

void print_error(const char *msg, Error_type type);

#endif /* ERROR_H */
