#ifndef SCHEME_BC_H
#define SCHEME_BC_H

#include <stddef.h>
#include <stdint.h>

typedef uint64_t SchemeBC;

enum {

  SCHEME_BC_HALT = 0,

  SCHEME_BC_EVAL,
  SCHEME_BC_PAP,
  SCHEME_BC_AP,
  SCHEME_BC_APPLY,
  SCHEME_BC_RETFUN,

  SCHEME_BC_NAT_IMM,

  SCHEME_BC_NAT_NEG,
  SCHEME_BC_NAT_ADD,
  SCHEME_BC_NAT_SUB,
  SCHEME_BC_NAT_MUL,
  SCHEME_BC_NAT_DIV,
  SCHEME_BC_NAT_REM,

  SCHEME_BC_NAT_EQ,
  SCHEME_BC_NAT_NE,

  SCHEME_BC_NAT_LT,
  SCHEME_BC_NAT_LE,
  SCHEME_BC_NAT_GT,
  SCHEME_BC_NAT_GE,

  SCHEME_BC_NAT_AND,
  SCHEME_BC_NAT_OR,
  SCHEME_BC_NAT_XOR,
  SCHEME_BC_NAT_NOT,
  SCHEME_BC_NAT_SHL,
  SCHEME_BC_NAT_SHR,

  SCHEME_BC_NAT_TOBYTE,
  SCHEME_BC_NAT_TOINT,
  SCHEME_BC_NAT_TODBL,
  SCHEME_BC_NAT_TOCHAR,

};


int
SchemeUnwind(Scheme *context, SchemeType type, SchemeWord word)
{

void
SchemeInterpret(Scheme *context)
{
  switch (context->value.hdr.type) {
    case SCHEME_TYPE_EXN: {
      SchemeSize nframes = 0;
      while (context->sp != context->spLim) {
        SchemeObj *frame = context->sp;
        if (frame->hdr.type == SCHEME_TYPE_CATCH) {
          SchemeObj *handler = frame->word.handler.fn;
          switch (handler->type) {
            case SCHEME_TYPE_FN: {

          if (frame->



      for (;;) {
        if (context->sp == context->spLim)
          return;
    case SCHEME_TYPE_PRIM: {
      
      for (SchemeSize ix = 0;
    }
    case SCHEME_TYPE_FN: {
      

}

#endif /* !SCHEME_BC_H */
