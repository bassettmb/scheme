#ifndef SCHEME_PRIM_TYPES_H
#define SCHEME_PRIM_TYPES_H

#include <stdbool.h>
#include "SchemeAssert.h"
#include "SchemeTypes.h"

/* XXX: what should the type of types be? also just a type? and since we
 * have arity built into the object header, maybe we get type-level
 * functions for cheap?
 *
 * .. though I guess the point of all of this is to allow you to do things
 * that aren't necessarily typeable (or easily typeable in this system)
 */

extern const SchemeType SchemeTypePrimByte;
extern const SchemeType SchemeTypePrimNat;
extern const SchemeType SchemeTypePrimInt;
extern const SchemeType SchemeTypePrimDouble;
extern const SchemeType SchemeTypePrimChar;
extern const SchemeType SchemeTypePrimArr;
extern const SchemeType SchemeTypePrimByteArr;
extern const SchemeType SchemeTypePrimVar;

extern const SchemeCons *const SchemeConsPrimByte;
extern const SchemeCons *const SchemeConsPrimNat;
extern const SchemeCons *const SchemeConsPrimInt;
extern const SchemeCons *const SchemeConsPrimDouble;
extern const SchemeCons *const SchemeConsPrimChar;
extern const SchemeCons *const SchemeConsPrimArr;
extern const SchemeCons *const SchemeConsPrimByteArr;
extern const SchemeCons *const SchemeConsPrimVar;

#define SCHEME_TYPE_ANY ((const SchemeType *)0)
#define SCHEME_TYPE_BYTE (&SchemeTypePrimByte)
#define SCHEME_TYPE_NAT (&SchemeTypePrimNat)
#define SCHEME_TYPE_INT (&SchemeTypePrimInt)
#define SCHEME_TYPE_DOUBLE (&SchemeTypePrimDouble)
#define SCHEME_TYPE_CHAR (&SchemeTypePrimChar)
#define SCHEME_TYPE_ARR (&SchemeTypePrimArr)
#define SCHEME_TYPE_BYTEARR (&SchemeTypePrimByteArr)
#define SCHEME_TYPE_VAR (&SchemeTypePrimVar)

#define SCHEME_CONS_ANY ((const SchemeCons *)0)
#define SCHEME_CONS_BYTE (SchemeConsPrimByte)
#define SCHEME_CONS_NAT (SchemeConsPrimNat)
#define SCHEME_CONS_INT (SchemeConsPrimInt)
#define SCHEME_CONS_DOUBLE (SchemeConsPrimDouble)
#define SCHEME_CONS_CHAR (SchemeConsPrimChar)
#define SCHEME_CONS_ARR (&SchemeConsPrimArr)
#define SCHEME_CONS_BYTEARR (&SchemeConsPrimByteArr)
#define SCHEME_CONS_VAR (&SchemeConsPrimVar)

static const SchemeType *
SchemeObjType(const SchemeObj *obj)
{
  switch (obj->hdr.kind) {
    case SCHEME_KIND_BYTE:
      return SCHEME_TYPE_BYTE;
    case SCHEME_KIND_NAT:
      return SCHEME_TYPE_NAT;
    case SCHEME_KIND_INT:
      return SCHEME_TYPE_INT;
    case SCHEME_KIND_DOUBLE:
      return SCHEME_TYPE_DOUBLE;
    case SCHEME_KIND_CHAR:
      return SCHEME_TYPE_CHAR;
    case SCHEME_KIND_CONS:
    case SCHEME_KIND_FN:
    case SCHEME_KIND_PRIM:
    case SCHEME_KIND_PAP:
    case SCHEME_KIND_AP:
      /* We should verify higher-level features before we ever reach this
       * point. We'll ensure safety by ensuring prim-ops cannot do
       * (unchecked) unsafe things. We may still reach a point where we
       * cannot ensure progress, so a program cannot be guaranteed to be
       * well-typed, but we can ensure preservation.
       */
      return SCHEME_TYPE_ANY;
    case SCHEME_KIND_VAL:
      /* values built by constructors of arity 0 are simply pointers to
       * their respective type .. the variant can be recovered from the
       * header in order to get the constructor
       */
      return val->hdr.arity ? val->word.cons->type : val->word.type;
    case SCHEME_KIND_ARR:
      return SCHEME_TYPE_ARR;
    case SCHEME_KIND_BYTEARR:
      return SCHEME_TYPE_BYTEARR;
    case SCHEME_KIND_VAR:
      return SCHEME_TYPE_VAR;
    case SCHEME_KIND_TYPE:
    default:
      SCHEME_UNIMPLEMENTED();
      return SCHEME_TYPE_ANY;
  }
}

static const SchemeCons *
SchemeObjCons(const SchemeObj *obj)
{
  switch (obj->hdr.kind) {
    case SCHEME_KIND_BYTE:
      return SCHEME_CONS_BYTE;
    case SCHEME_KIND_NAT:
      return SCHEME_CONS_NAT;
    case SCHEME_KIND_INT:
      return SCHEME_CONS_INT;
    case SCHEME_KIND_DOUBLE:
      return SCHEME_CONS_DOUBLE;
    case SCHEME_KIND_CHAR:
      return SCHEME_CONS_CHAR;
    case SCHEME_KIND_CONS:
    case SCHEME_KIND_FN:
    case SCHEME_KIND_PRIM:
    case SCHEME_KIND_PAP:
    case SCHEME_KIND_AP:
      return SCHEME_CONS_ANY;
    case SCHEME_KIND_VAL:
      if (val->hdr.arity)
        return val->word.cons;
      SCHEME_ASSERT(val->word.type->hdr.variant > val->hdr.variant);
      return val->word.type->variants + val->hdr.variant;
    case SCHEME_KIND_ARR:
      return SCHEME_CONS_ARR;
    case SCHEME_KIND_BYTEARR:
      return SCHEME_CONS_BYTEARR;
    case SCHEME_KIND_VAR:
      return SCHEME_CONS_VAR;
    case SCHEME_KIND_TYPE:
    default:
      SCHEME_UNIMPLEMENTED();
      return SCHEME_TYPE_ANY;
  }
}

bool
SchemeCheckType(const SchemeType *type, const SchemeObj *obj)
{
  return type == SCHEME_TYPE_ANY || SchemeObjType(obj) == type;
}

bool
SchemeCheckCase(const SchemeCons *cons, const SchemeObj *obj)
{
  return cons == SCHEME_CONS_ANY || SchemeObjCons(obj) == cons;
}

bool
SchemeCheckPrimMatch(const SchemeObj *expr, const SchemeObj *obj)
{
  if (expr->hdr.kind == SCHEME_KIND_ANY)
    return true;
  if (expr->hdr.kind != obj->hdr.kind)
    return false;
  switch (expr->hdr.kind) {
    case SCHEME_KIND_BYTE:
      return expr->word.byte == obj->word.byte;
    case SCHEME_KIND_NAT:
      return expr->word.nat == obj->word.nat;
    case SCHEME_KIND_INT:
      return expr->word.num == obj->word.num;
    case SCHEME_KIND_DOUBLE:
      return expr->word.dbl == obj->word.dbl;
    case SCHEME_KIND_CHAR:
      return expr->word.ch == obj->word.ch;
    default:
      return false;
  }
}

#endif /* !SCHEME_PRIM_TYPES_H */
