#include <stdlib.h>

static const int buf_size = 512;
static const int port = 4444;

void err_write(char *s) {
	perror(s);
	exit(1);
}
