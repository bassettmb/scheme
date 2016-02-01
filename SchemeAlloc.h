#ifndef SCHEME_ALLOC_H
#define SCHEME_ALLOC_H

#include "SchemeTypes.h"

void *SchemeAlloc(Scheme *context, SchemeSize nbytes, SchemeSize nwords);
SchemeError SchemeGrowStack(Scheme *context, SchemeSize nwords);

#endif /* !SCHEME_ALLOC_H */
