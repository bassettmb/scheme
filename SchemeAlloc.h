#ifndef SCHEME_ALLOC_H
#define SCHEME_ALLOC_H

#include "SchemeAssert.h"
#include "SchemeTypes.h"

#include <stddef.h>
#include <stdint.h>

#define SCHEME_ALLOC_MAX (SCHEME_SIZE_MAX - sizeof(SchemeMem))
#define SCHEME_MEM(ptr) \
  ((SchemeMem *)((char *)(ptr) - offsetof(SchemeMem, obj)))
#define SCHEME_HEAP(ctx) (&ctx->heap)

typedef uint16_t SchemeGCState;
typedef struct SchemeMem_ SchemeMem;
typedef struct SchemeHeap_ SchemeHeap;

typedef void *(*SchemeAllocFn)(void *ptr, size_t oldsize, size_t newsize);

enum {

  SCHEME_GC_WHITE      = 0,
  SCHEME_GC_GREY       = 1,
  SCHEME_GC_BLACK      = 2,

  SCHEME_GC_COLOR_MASK = 3,

  SCHEME_GC_RELOC      = 4,
  SCHEME_GC_LARGE      = 8,
  SCHEME_GC_STATIC     = 16,

  SCHEME_GC_LOC_MASK   = 28,

  SCHEME_GC_MUT        = 32,
  SCHEME_GC_DIRTY      = 64,

  SCHEME_GC_MUT_MASK   = 96

};

struct SchemeMem_ {
  SchemeGCState state;
  SchemeMem *queue;
  SchemeMem *link;
  SchemeHdr obj; /* for convenience, padding, etc. */
};

struct SchemeHeap_ {
  SchemeMem *root;
  SchemeMem *black;
  SchemeMem *grey;
  SchemeMem *dirty;
  SchemeAllocFn allocFn;
  void *allocCtx;
};

void *
SchemeAllocRaw(Scheme *context, SchemeSize nbytes)
{
  SCHEME_ASSERT(nbytes >= sizeof (SchemeHdr));
  SchemeMem *mem;
  if (SCHEME_SIZE_MAX - (sizeof *mem - (sizeof mem->obj)) < nbytes) {
    context->err = SCHEME_ERROR_OVERFLOW;
    return 0;
  }
  mem = context->allocFn(context->allocCtx, 0, 0, sizeof *mem + nbytes);
  if (!mem) {
    context->err = SCHEME_ERROR_ALLOC;
    return 0;
  }
  mem->state = SCHEME_GC_WHITE;
  mem->link = context->root;
  context->root = mem;
  return &mem->obj;
}

void *
SchemeAllocWords(Scheme *context, SchemeSize nbytes, SchemeSize nwords)
{
  if ((SCHEME_SIZE_MAX - nbytes) / SCHEME_WORD_SIZE < nwords) {
    context->err = SCHEME_ERROR_OVERFLOW;
    return 0;
  }
  return SchemeAllocRaw(context, nalloc + SCHEME_WORD_SIZE * nobjs);
}

void *
SchemeAllocObjs(Scheme *context, SchemeSize nbytes, SchemeSize nobjs)
{
  if ((SCHEME_SIZE_MAX - nbytes) / SCHEME_OBJ_SIZE < nobjs) {
    context->err = SCHEME_ERROR_OVERFLOW;
    return 0;
  }
  return SchemeAllocRaw(context, nbytes + SCHEME_OBJ_SIZE * nobjs);
}

SchemePAP *
SchemePAPAlloc(Scheme *context, SchemeArity arity, SchemeArity size)
{
  SchemePAP *pap =
    SchemeAllocObjs(context, sizeof *pap, (SchemeSize)arity + size);
  if (!pap)
    return 0;
  pap->hdr.kind = SCHEME_KIND_PAP;
  pap->hdr.arity = arity;
  pap->hdr.size = size;
  return pap;
}

SchemeAP *
SchemeAPAlloc(Scheme *context, SchemeArity size)
{
  SchemeAP *ap = SchemeAllocObjs(context, sizeof *ap, (SchemeSize)size);
  if (!ap)
    return 0;
  ap->hdr.kind = SCHEME_KIND_AP;
  ap->hdr.arity = 1;
  ap->hdr.size = size;
  return ap;
}

SchemeArr *
SchemeArrAlloc(Scheme *context, SchemeSize size, bool mut)
{
  SchemeArr *arr;
  if ((SCHEME_SIZE_MAX - sizeof *arr) / SCHEME_OBJ_SIZE < size) {
    context->err = SCHEME_ERROR_OVERFLOW;
    return 0;
  }
  arr = SchemeAllocRaw(context, sizeof *arr + SCHEME_OBJ_SIZE * size);
  if (!arr)
    return 0;
  arr->hdr.kind = SCHEME_KIND_ARR;
  arr->hdr.variant = mut;
  arr->hdr.nelems = size >= SCHEME_HDR_MAX ? SCHEME_HDR_MAX : size;
  return arr;
}

SchemeByteArr *
SchemeByteArrAlloc(Scheme *context, SchemeSize size, bool mut)
{
  SchemeByteArr *bytearr;
  if (SCHEME_SIZE_MAX - sizeof *bytearr < size) {
    context->err = SCHEME_ERROR_OVERFLOW;
    return 0;
  }
  bytearr = SchemeAllocRaw(context, sizeof *bytearr + size);
  if (!bytearr)
    return 0;
  bytearr->hdr.kind = SCHEME_KIND_BYTEARR;
  bytearr->hdr.variant = mut;
  bytearr->hdr.nelems = size >= SCHEME_HDR_MAX ? SCHEME_HDR_MAX : size;
  return bytearr;
}

SchemePAP *
SchemePAPNew(Scheme *context, const SchemeFn *fn, SchemeArity size)
{
  if (context->sp - context->spLim + 1 < size) {
    context->err = SCHEME_ERROR_ARITY;
    return 0;
  }
  const SchemeType *const *sig = (const SchemeType *const *)fn->code;
  for (SchemeSize ix = 0; ix < size; ix += 1) {
    if (!SchemeCheckType(sig + ix, context->sp + ix)) {
      context->err = SCHEME_ERROR_TYPE;
      return SCHEME_ERROR_TYPE;
    }
  }
  SchemePAP *pap = SchemePAPAlloc(context, fn->hdr.arity - size, size);
  if (!pap)
    return 0;
  pap->fn = fn;
  for (SchemeSize ix = 0; ix < size; ix += 1)
    pap->data[ix] = context->sp[ix];
  context-sp += size;
  return pap;
}

SchemePAP *
SchemePAPApply(Scheme *context, const SchemePAP *oldpap, SchemeArity size)
{
  /* Strictly, we need at least one more than size since we need a
   * continuation to invoke after we complete the current function.
   */
  if (context->sp - context->spLim < size) {
    context->err = SCHEME_ERROR_ARITY;
    return 0;
  }
  const SchemeType *const *sig = SchemePAPSig(oldpap);
  for (SchemeSize ix = 0; ix < size; ix += 1) {
    if (!SchemeCheckType(sig + ix, context->sp + ix)) {
      context->err = SCHEME_ERROR_TYPE;
      return 0;
    }
  }
  SchemeSize oldsize = pap->size;
  SchemeArity arity = pap->arity - size;
  SchemeArity newsize = oldsize + size;
  SchemePAP *pap = SchemePAPAlloc(context, arity, newsize);
  if (!pap)
    return 0;
  pap->fn = oldpap->fn;
  SchemeObj *data = pap->data;
  for (SchemeSize ix = 0; ix < oldsize; ix += 1)
    data[ix] = oldpap->data[ix];
  data += oldsize;
  for (SchemeSize ix = 0; ix < size; ix += 1)
    data[ix] = context->sp[ix];
  context->sp += ix;
  return pap;
}

SchemeAP *
SchemeAPNew(Scheme *context, SchemeArity size)
{
  if (context->sp - context->spLim + 1 < size) {

  SchemeAP *ap = SchemeAPAlloc(context, size);
  if (!ap)
    return 0;
  for (SchemeSize ix = 0; ix < size; ix += 1)
    data[ix] = context->sp[ix];
  context->sp += ix;
}

SchemeArr *
SchemeArrNew(Scheme *context, SchemeSize size, SchemeObj val, bool mut)
{
  SchemeArr *arr = SchemeArrAlloc(context, size, mut);
  if (!arr)
    return 0;
  arr->size = size;
  for (SchemeSize ix = 0; ix < size; ix += 1)
    arr->data[ix] = val;
  return arr;
}

SchemeByteArr *
SchemeByteArrNew(Scheme *context, SchemeSize size, SchemeByte val, bool mut)
{
  SchemeByteArr *bytearr = SchemeByteArrAlloc(context, size, mut);
  if (!bytearr)
    return 0;
  bytearr->size = size;
  for (SchemeSize ix = 0; ix < size; ix += 1)
    arr->data[ix] = val;
  return bytearr;
}

SchemeError
SchemeShift(Scheme *context, SchemeSize nelem, SchemeSize count)
{
  SchemeSize navail = context->sp - context->spLim;
  if (navail < nelem || navail - nelem < count) {
    context->err = SCHEME_ERROR_ARITY;
    return SCHEME_ERROR_ARITY;
  }
  SchemeObj *dest = context->sp + count;
  for (SchemeSize ix = nelem; ix--; )
    dest[ix] = context->sp[ix];
  context->sp += count;
  return SCHEME_ERROR_OK;
}

SchemeString *
SchemeAllocString(Scheme *context, SchemeSize nchars, SchemeSize nbytes)
{
  SchemeString *str;
  if (SCHEME_SIZE_MAX - sizeof *str < size) {
    errno = ENOMEM;
    return 0;
  }
  str = SchemeAllocRaw(context, sizeof *str + size, 0);
  if (!str)
    return 0;
  str->hdr.kind = SCHEME_KIND_STRING;
  str->hdr.nelems = nchars >= SCHEME_HDR_MAX ? SCHEME_HDR_MAX : nchars;
  return str;
}

SchemeType *
SchemeAllocType(Scheme *context, SchemeVariant nvariants)
{
  SchemeType *type;
  SchemeAllocRaw(context, sizeof *type, );
}

SchemeFn *
SchemeAllocFn(Scheme *context, SchemeArity arity,
              SchemeSize nobjs, SchemeSize codelen)
{
  SchemeFn *fn;
  if (SCHEME_SIZE_MAX - sizeof *fn / SCHEME_WORD_SIZE < codelen) {
    errno = ENOMEM;
    return 0;
  }
  SchemeSize nbytes = sizeof *fn + codelen * SCHEME_WORD_SIZE;
  SchemeFn *fn = SchemeAllocRaw(context, nbytes, nobjs);
  if (!fn)
    return 0;
  fn->hdr.kind = SCHEME_KIND_FN;
  fn->hdr.arity = arity;
  return fn;
}

SchemeVal *
SchemeAllocVal(Scheme *context, const SchemeCons *cons)
{
  /* We shouldn't allow allocation of primitive types or constructors of
   * null arity on the heap.
   */
  SCHEME_ASSERT(cons->hdr.kind == SCHEME_KIND_CONS);
  SCHEME_ASSERT(cons->hdr.arity > 0);
  SchemeVal *val = SchemeAllocRaw(context, sizeof *val, cons->hdr.arity);
  if (!val)
    return 0;
  val->hdr.kind = SCHEME_KIND_VAL;
  val->hdr.variant = cons->hdr.variant;
  val->hdr.size = cons->hdr.arity;
  return val;
}


