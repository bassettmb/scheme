#ifndef SCHEME_BASIC_TYPES_H
#define SCHEME_BASIC_TYPES_H

#include "SchemeType.h"

extern const SchemeType SchemeBool;
extern const SchemeCons *const SchemeBoolTrue;
extern const SchemeCons *const SchemeBoolFalse;
extern const SchemeType SchemeTuple;
extern const SchemeCons *const SchemeTupleTuple;
extern const SchemeType SchemeList;
extern const SchemeCons *const SchemeListCons;
extern const SchemeCons *const SchemeListEmpty;
extern const SchemeType SchemeOption;
extern const SchemeCons *const SchemeOptionNone;
extern const SchemeCons *const SchemeOptionSome;
extern const SchemeType SchemeEither;

static void
SchemeMarshalBool(Scheme *context, bool value)
{
  SchemeObj *val = &context->value;
  val->hdr.kind = SCHEME_KIND_VAL;
  val->hdr.variant = value ?
    SchemeBoolTrue->hdr.variant : SchemeBoolFalse->hdr.variant;
  val->hdr.size = 0;
  val->word.type = &SchemeBool;
}


#endif /* !SCHEME_BASIC_TYPES_H */
