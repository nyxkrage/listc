#include <stdarg.h>
#include <stdio.h>

#ifndef eprintf
#define eprintf(...) fprintf (stderr, __VA_ARGS__)

#endif // EPRINTF_H_
