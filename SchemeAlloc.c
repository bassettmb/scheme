#include "SchemeAlloc.h"
#include "SchemeTypes.h"
#include <stdlib.h>

static void *
SchemeAllocRaw(Scheme *context, SchemeSize nbytes, SchemeSize nwords)
{
  if ((SCHEME_SIZE_MAX - nbytes) / SCHEME_WORD_SIZE < nwords)
    return 0;
  return malloc(nbytes + nwords * SCHEME_WORD_SIZE);
}

static void
SchemeDeallocRaw(Scheme *context, void *ptr)
{
  free(ptr);
}


void *
SchemeAlloc(Scheme *context, SchemeSize nbytes, SchemeSize nwords)
{
  return SchemeAllocRaw(context, nbytes, nwords);
}

SchemeError
SchemeGrowStack(Scheme *context, SchemeSize nobjs)
{
  SchemeSize oldsize = context->sp - context->spLim;
  if (SCHEME_SIZE_MAX - oldsize < nobjs)
    return SCHEME_ERROR_ALLOC;
  SchemeSize newsize = oldsize;
  SchemeSize minsize = oldsize + nobjs;
  if (SCHEME_SIZE_MAX / 4 < minsize)
  while (newsize <= minsize)
    newsize *= 2;
  if (SCHEME_SIZE_MAX / SCHEME_OBJ_SIZE < size)
    return SCHEME_ERROR_ALLOC;
  SchemeObj *spBase = SchemeAllocRaw(context, nobjs * SCHEME_OBJ_SIZE, 0);
  if (!spBase)
    return SCHEME_ERROR_ALLOC;
  SchemeObj *sp = spBase + (newsize - oldsize);
  for (SchemeSize ix = 0; ix < oldsize; ix += 1)
    sp[ix] = context->sp[ix];
  SchemeDeallocRaw(context->spBase);
  contet->spBase = spBase;
  context->sp = sp;
  context->spLim = spBase + newsize;
  return SCHEME_ERROR_OK;
}
