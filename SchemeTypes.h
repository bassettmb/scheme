#ifndef SCHEME_TYPES_H
#define SCHEME_TYPES_H

#include <assert.h>
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

enum {

  SCHEME_KIND_UNDEF = 0,

  /* Primitives */
  SCHEME_KIND_BYTE,
  SCHEME_KIND_NAT,
  SCHEME_KIND_INT,
  SCHEME_KIND_CHAR,
  SCHEME_KIND_DOUBLE,

  /* Type-level primitives */
  SCHEME_KIND_CONS,
  SCHEME_KIND_TYPE,
  SCHEME_KIND_PRIM,
  SCHEME_KIND_FN,

  /* Execution-specific heap objects */
  SCHEME_KIND_AP,
  SCHEME_KIND_PAP,

  /* Heap values */
  SCHEME_KIND_VAL,
  SCHEME_KIND_ARR,
  SCHEME_KIND_BYTEARR,
  SCHEME_KIND_VAR,

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
  SchemeCons variants[];
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
  SchemeCode code[]; /* first arity words constitute type sig */
};

/*
struct SchemeString_ {
  SchemeHdr hdr;
  SchemeSize size;
  SchemeByte data[];
};

struct SchemeSymbol_ {
  SchemeHdr hdr;
  SchemeSize hash;
  SchemeSize size;
  SchemeByte data[];
};
*/

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
  //const SchemeString *str;
  //const SchemeSymbol *sym;
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
    //const SchemeString *str;
    //const SchemeSymbol *sym;
    SchemeVal *val;
    SchemePAP *pap;
    SchemeAP *ap;
    SchemeArr *arr;
    SchemeByteArr *byteArr;
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

struct Scheme_ {
  SchemeArity nargs;
  SchemeObj *spBase;
  SchemeObj *sp;
  SchemeObj *spLim;
  SchemeObj *hp;
  SchemeObj *hpLim;
  SchemeObj value;
  SchemeObj stack[SCHEME_STACK_MIN];
};

#endif /* !SCHEME_TYPE_H */
