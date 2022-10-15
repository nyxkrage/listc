#ifndef TEST_H
#define TEST_H
#include <stdarg.h>
#include <stdio.h>
#define ASSERT(lhs, rhs, emsg, ...) if (lhs!=rhs) {fprintf(stderr, emsg, lhs, rhs, ##__VA_ARGS__);exit(1); }

#endif // TEST_H
