#ifndef SCHEME_ERROR_H
#define SCHEME_ERROR_H

typedef int SchemeError;

enum {
  SCHEME_ERROR_OK = 0,      /* no error */
  SCHEME_ERROR_TYPE,        /* type error */
  SCHEME_ERROR_CASE,        /* unmatched case */
  SCHEME_ERROR_ALLOC,       /* allocation failure */
  SCHEME_ERROR_OVERFLOW,    /* size overflow */
  SCHEME_ERROR_UNSAT_EVAL,  /* entered unsaturated function */
  SCHEME_ERROR_OVERSAT_EVAL /* entered oversaturated function */
};

#endif /* !SCHEME_ERROR_H */
