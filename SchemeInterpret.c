#include <stdbool.h>
#include "SchemeError.h"
#include "SchemePrimTypes.h"
#include "SchemeTypes.h"

SchemeError
SchemeCheckCall(const Scheme *context,
                SchemeArity arity, const SchemeType *const *sig)
{
  if (context->nargs < arity)
    return SCHEME_ERROR_UNSAT;
  if (context->nargs > arity)
    return SCHEME_ERROR_OVERSAT;

  const SchemeObj *sp = context->sp;
  SCHEME_ASSERT(sp - context->spLim >= arity);
  for (SchemeArity ix = 0; ix < arity; ix += 1)
    if (!SchemeCheckType(sig[ix], sp[ix]))
      return SCHEME_ERROR_TYPE;

  return SCHEME_ERROR_OK;
}

SchemeError
SchemeCheckFn(Scheme *context)
{
  SchemeObj *val = &context->value;
  SchemeKind kind = val->hdr.kind;
  SCHEME_ASSERT(kind == SCHEME_KIND_FN);
  const SchemeFn *fn = val->word.fn;
  SchemeArity arity = val->hdr.arity;
  SCHEME_ASSERT(arity);
  return SchemeCheckCall(context, arity, fn->sig);
}

SchemeError
SchemeCallCons(Scheme *context)
{
  SchemeObj *val = &context->value;
  SchemeKind kind = val->hdr.kind;
  SCHEME_ASSERT(kind == SCHEME_KIND_CONS);
  const SchemeCons *cons = val->word.cons;
  SchemeArity arity = val->hdr.arity;
  SchemeVariant variant = val->hdr.variant;
  const SchemeType *type = cons->type;
  SCHEME_ASSERT(variant < type->hdr.variant);

  if (!arity) {
    val->hdr.kind = SCHEME_KIND_VAL;
    val->hdr.variant = variant;
    val->hdr.size = 0;
    val->word.type = type;
    return SCHEME_ERROR_OK;
  }

  SchemeError error = SchemeCheckCall(context, arity, cons->sig);
  if (error)
    return error;
  SchemeVal *obj = SchemeAlloc(context, sizeof *obj, arity);
  if (!obj)
    return SCHEME_ERROR_ALLOC;

  for (SchemeArity ix = 0; ix < arity; ix += 1)
    val->data[ix] = context->sp[ix];

  context->sp += arity;
  val->hdr.kind = SCHEME_KIND_VAL;
  val->hdr.variant = variant;
  val->hdr.size = arity;
  val->word.val = obj;

  return SCHEME_ERROR_OK;
}

SchemeError
SchemeUnpackPAP(Scheme *context)
{
  SchemeObj *val = &context->value;
  SchemeKind kind = val->hdr.kind;
  SCHEME_ASSERT(kind == SCHEME_KIND_PAP);
  SchemeArity size = val->hdr.size;
  if (size < 
  SchemePAP *pap = val->word.pap;



SchemeError
SchemeInterpret(Scheme *context)
{
  SchemeObj *val = context->value;
  SchemeError err;

  {
pap:
    SchemeArity arity = val->hdr.arity;
    SchemeArity size = val->hdr.size;
    SchemePAP *pap = val->word.pap;
    SCHEME_ASSERT(context->sp - context->spLim >= arity);
    context->sp += arity;
    for (SchemeArity ix = 0; ix < arity; ix += 1)
      context->sp[ix] = pap->sp[ix];
    SchemeObj *obj = pap->fn

  }

  SchemeCode *ip = val->word.fn.code;
  for (;;) {
    case SCHEME_BC_PRIM:

    case SCHEME_BC_CALL:
      err = SchemeCheckCall(context);
      if (err)
        goto err;
      ip = val->word.fn.code;
      break;
    }
    case SCHEME_BC_NAT_IMM:
      val->hdr.kind = SCHEME_TYPE_NAT;
      val->word.nat = ip[1];
      ip += 2;
      break;
    case SCHEME_BC_NAT_ADD:
      SCHEME_ASSERT(context->nargs == 2);
      SCHEME_ASSERT(val->hdr.kind == SCHEME_KIND_NAT);
      SCHEME_ASSERT(sp->hdr.kind == SCHEME_KIND_NAT);
      val->word.nat += sp->word.nat;
      ip += 1;
      break;
    case SCHEME_BC_NAT_SUB:
      SCHEME_ASSERT(context->nargs == 2);
      SCHEME_ASSERT(val->hdr.kind == SCHEME_KIND_NAT);


    case SCHEME_BC_RETFUN:
      context->value = 
      context->sp
  }

err:
  return err;
}
