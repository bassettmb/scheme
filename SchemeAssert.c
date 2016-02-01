#include "SchemeAssert.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SCHEME_STRINGIFY_(s) #s
#define SCHEME_STRINGIFY(s) SCHEME_STRINGIFY_(s)

#define SCHEME_MSG(kind, ...) \
  SchemeLog(__FILE__, __func__, __LINE__, kind, __VA_ARGS__)
#define SCHEME_LOG(...) SCHEME_MSG("LOG", __VA_ARGS__)
#define SCHEME_PING() SchemeLog(__FILE__, __func__, __LINE__, "PING", 0)
#define SCHEME_PANIC(...) \
  SchemePanic(__FILE__, __func__, __LINE__, __VA_ARGS__)
#define SCHEME_UNIMPLEMENTED() SCHEME_PANIC("%s", "unimplemented")
#define SCHEME_UNREACHABLE() SCHEME_PANIC("%s", "unreachable")
#define SCHEME_ASSERT(expr) ((void)((expr) ? (void)0 : \
  (SchemeAssertFail(__FILE__, __func__, __LINE__, SCHEME_STRINGIFY(expr)))))

void
SchemeAbort(void)
{
  abort();
  for (;;)
    exit(EXIT_FAILURE);
}

void
SchemeLogMeta(const char *file, const char *fn, int lineno, const char *kind)
{
  fprintf(stderr, "%s:%s:%d: (%s)", file, fn, lineno, kind);
}

void
SchemeLogV(const char *file, const char *fn, int lineno,
           const char *kind, const char *fmt, va_list argv)
{
  fflush(stdout);
  SchemeLogMeta(file, fn, lineno, kind);
  vfprintf(stderr, fmt, argv);
  putc('\n', stderr);
  fflush(stderr);
}

void
SchemeLog(const char *file, const char *fn, int lineno,
          const char *kind, const char *fmt, ...)
{
  fflush(stdout);
  SchemeLogMeta(file, fn, lineno, kind);
  if (fmt) {
    va_list argv;
    va_start(argv, fmt);
    vfprintf(stderr, fmt, argv);
    va_end(argv);
  }
  putc('\n', stderr);
}

void
SchemePanic(const char *file, const char *fn,
            int lineno, const char *fmt, ...)
{
  va_list argv;
  va_start(argv, fmt);
  SchemeLogV(file, fn, lineno, "PANIC", fmt, argv);
  va_end(argv);
  SchemeAbort();
}

void
SchemeAssertFail(const char *file, const char *fn,
                 int lineno, const char *expr)
{
  SchemeLog(file, fn, lineno, "ASSERT", "%s", expr);
  SchemeAbort();
}
