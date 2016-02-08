#ifndef SCHEME_TYPES_H
#define SCHEME_TYPES_H

#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Scheme_ Scheme;
typedef void (*SchemeNativeFn)(Scheme *context);
typedef uint64_t SchemeByteCode;
typedef unsigned char SchemeByte;
typedef size_t SchemeSize;
typedef uint32_t SchemeHdrSize;
typedef unsigned char *SchemePtr;
typedef uint16_t SchemeArity;
typedef uint16_t SchemeVariant;
typedef uint16_t SchemeKind;
typedef uint64_t SchemeNat;
typedef int64_t SchemeInt;
typedef uint32_t SchemeChar;
typedef double SchemeDouble;
typedef unsigned char SchemeRaw;
typedef SchemeNat SchemeBC;
typedef SchemeInt SchemeError;

enum {

  SCHEME_KIND_UNDEF = 0,  /* undefined */

  /* Primitives */

  SCHEME_KIND_BYTE,       /* unsigned byte */
  SCHEME_KIND_NAT,        /* native unsigned int */
  SCHEME_KIND_INT,        /* native signed int */
  SCHEME_KIND_CHAR,       /* unicode codepoint */
  SCHEME_KIND_DOUBLE,     /* IEEE 754 double */

  /* Type-level primitives */

  SCHEME_KIND_CONS,       /* data constructor */
  SCHEME_KIND_TYPE,       /* type constructor */
  SCHEME_KIND_PRIM,       /* primitive (C) function */
  SCHEME_KIND_FN,         /* bytecode function */

  /* Execution-specific heap objects */

  SCHEME_KIND_AP,         /* trampoline continuation */
  SCHEME_KIND_PAP,        /* partially-applied function */

  /* Heap values */

  SCHEME_KIND_VAL,        /* saturated data constructor */
  SCHEME_KIND_ARR,        /* array of objects */
  SCHEME_KIND_BYTEARR,    /* array of bytes */
  SCHEME_KIND_STRING,     /* string */

  SCHEME_KIND_VAR,        /* mutable reference */

  /* XXX: mutable kinds share struct definition with immutable kinds */

  SCHEME_KIND_MUTARR,     /* mutable array */
  SCHEME_KIND_MUTBYTEARR  /* mutable array of bytes */

};

enum {

  SCHEME_ERROR_OK = 0,
  SCHEME_ERROR_ALLOC,
  SCHEME_ERROR_OVERFLOW,

  SCHEME_ERROR_ARITY,
  SCHEME_ERROR_TYPE,

};

typedef union SchemeRaw_ SchemeRaw;
typedef struct SchemeWord_ SchemeWord;
typedef struct SchemeHdr_ SchemeHdr;
typedef struct SchemeObj_ SchemeObj;

typedef struct SchemePrim_ SchemePrim;
typedef struct SchemeFn_ SchemeFn;
typedef struct SchemeCons_ SchemeCons;
typedef struct SchemePAP_ SchemePAP;
typedef struct SchemeAP_ SchemeAP;

typedef struct SchemeType_ SchemeType;
typedef struct SchemeVal_ SchemeVal;
typedef struct SchemeByteArr_ SchemeByteArr;
typedef struct SchemeArr_ SchemeArr;
typedef struct SchemeVar_ SchemeVar;
typedef struct SchemeString_ SchemeString;

/* Model of lambda calculus
 *
 * Variable: Fn, Prim, Cons, Val, Primitive Types
 * Abstraction: Fn, Prim, Cons 
 * Application: (Abstraction Variable)
 */

#define SCHEME_SIZE_MAX (SIZE_MAX)
#define SCHEME_HDR_MAX (UINT32_MAX)
#define SCHEME_WORD_SIZE (sizeof (SchemeWord))
#define SCHEME_OBJ_SIZE (sizeof (SchemeObj))
#define SCHEME_ARR_MAX (SCHEME_SIZE_MAX / SCHEME_WORD_SIZE)

#define SCHEME_TYPE_ANY ((const SchemeType *)(0))
#define SCHEME_SIZEOF_ARRAY(arr) (sizeof (arr) / sizeof *(arr))
#define SCHEME_STACK_MIN (4096ul)

#define SCHEME_BYTE_SIZE (sizeof (SchemeByte)) /* should equal 1 */
#define SCHEME_BYTE_BITS (SCHEME_BYTE_SIZE * CHAR_BIT)
#define SCHEME_NAT_SIZE (sizeof (SchemeNat))
#define SCHEME_NAT_BITS (SCHEME_NAT_SIZE * CHAR_BIT)
#define SCHEME_INT_SIZE (sizeof (SchemeInt))
#define SCHEME_INT_BITS (SCHEME_INT_SIZE * CHAR_BIT)

#define SCHEME_CHAR_MAX (0x10fffful) /* largest unicode codepoint */
#define SCHEME_CHAR_INVAL (0xfffdul) /* unicode replacement codepoint */

struct SchemeHdr_ {
  SchemeKind kind;
  SchemeVariant variant;
  union {
    struct {
      SchemeArity arity;
      SchemeArity size;
    }
    SchemeHdrSize nelems;
  };
};

struct SchemeCons_ {
  SchemeHdr hdr;
  const SchemeType *type;
  const SchemeType *sig[];
};

struct SchemeType_ {
  SchemeHdr hdr;
  SchemeCons *variants[];
};

struct SchemePrim_ {
  SchemeHdr hdr;
  const SchemeType *type;
  const SchemeType *sig[];
  SchemeNativeFn entry;
};

struct SchemeFn_ {
  SchemeHdr hdr;
  const SchemeType *type; /* return type */
  SchemeSize nobjs;       /* slots reserved for objects (including type sig) */
  SchemeWord code[];      /* block of raw code */
  /* The first nobjs elements of code are statically-bound objects. The
   * first hdr.arity of those objects form the function's type signature.
   * The remaining elements are other statically-bound objects or types.
   *
   * The remainder of the array after the first nobjs is bytecode.
   */
};

union SchemeRaw_ {
  SchemeNat nat;
  SchemeInt num;
  SchemeChar ch;
  SchemeDouble dbl;
  const SchemeHdr *obj;
  const SchemeFn *fn;
  const SchemeCons *cons;
  const SchemePrim *prim;
  const SchemeType *type;
  const SchemeString *str;
  SchemeVal *val;
  SchemePAP *pap;
  SchemeAP *ap;
  SchemeArr *arr;
  SchemeByteArr *byteArr;
};

struct SchemeWord_ {
  union {
    SchemeRaw align_;
    SchemeNat nat;
    SchemeInt num;
    SchemeChar ch;
    SchemeDouble dbl;
    const SchemeHdr *obj;
    const SchemeFn *fn;
    const SchemeCons *cons;
    const SchemePrim *prim;
    const SchemeType *type;
    const SchemeString *str;
    SchemeVal *val;
    SchemePAP *pap;
    SchemeAP *ap;
    SchemeArr *arr;
    SchemeByteArr *byteArr;
    SchemeBC instr;
    SchemeByte mem[SCHEME_WORD_SIZE];
  };
};

struct SchemeObj_ {
  SchemeHdr hdr;
  SchemeWord word;
};

struct SchemeVal_ {
  SchemeHdr hdr;
  const SchemeType *type;
  SchemeObj data[];
};

struct SchemePAP_ {
  SchemeHdr hdr;
  const SchemeHdr *fn;
  SchemeObj data[];
};

struct SchemeAP_ {
  SchemeHdr hdr;
  SchemeObj data[];
};

struct SchemeArr_ {
  SchemeHdr hdr;
  SchemeSize size;
  SchemeObj data[];
};

struct SchemeByteArr_ {
  SchemeHdr hdr;
  SchemeSize size;
  SchemeByte data[];
};

struct SchemeVar_ {
  SchemeHdr hdr;
  SchemeVar *link;
  SchemeObj value;
};

struct SchemeString_ {
  SchemeHdr hdr;
  SchemeSize nchars;
  SchemeSize size;
  SchemeByte data[];
};

struct Scheme_ {
  SchemeError error;
  SchemeArity nargs;
  SchemeObj *spBase;
  SchemeObj *sp;
  SchemeObj *spLim;
  SchemeHeap heap;
  SchemeObj value;
  SchemeObj stack[SCHEME_STACK_MIN];
};

/* Primitive type descriptors. */

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

/* Retrieve the type of a given object. */
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

/* Retrieve the constructor of an object. */
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

static bool
SchemeCheckType(const SchemeType *type, const SchemeObj *obj)
{
  return type == SCHEME_TYPE_ANY || SchemeObjType(obj) == type;
}

static bool
SchemeCheckCase(const SchemeCons *cons, const SchemeObj *obj)
{
  return cons == SCHEME_CONS_ANY || SchemeObjCons(obj) == cons;
}

static bool
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

const SchemeType *const *
SchemeFnSig(const SchemeFn *fn)
{
  return (const SchemeType *const *)fn->code;
}

const SchemeType *const *
SchemePAPSig(const SchemePAP *pap)
{
  return (const SchemeType *const *)pap->fn->code + pap->hdr.size;
}

#endif /* !SCHEME_TYPE_H */
