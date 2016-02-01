#include "SchemeBasicTypes.h"

const SchemeType SchemeBool = {
  { /* metadata */
    SCHEME_KIND_TYPE,
    2 /* number of variants */
  },
  { /* constructors */
    { /* true */
      { /* metadata */
        SCHEME_KIND_CONS,
        0, /* variant */
        0  /* arity */
      },
      &SchemeBool /* type */
    },
    { /* false */
      { /* metadata */
        SCHEME_KIND_CONS,
        1, /* variant */
        0  /* arity */
      },
      &SchemeBool /* type */
    }
  }
};

const SchemeCons *const SchemeBoolTrue = SchemeBool.variants;
const SchemeCons *const SchemeBoolFalse = SchemeBool.variants + 1;

const SchemeType SchemeTuple = {
  { /* metadata */
    SCHEME_KIND_TYPE,
    1 /* variants */
  },
  { /* constructors */
    { /* tuple */
      {
        SCHEME_KIND_CONS,
        1, /* variant */
        2  /* arity */
      },
      &SchemeTuple, /* "return" type */
      { SCHEME_TYPE_ANY, SCHEME_TYPE_ANY } /* argument types */
    }
  }
};

const SchemeCons *const SchemeTupleTuple = SchemeTuple.variants;

const SchemeType SchemeList = {
  { /* metadata */
    SCHEME_KIND_TYPE,
    2 /* variants */
  },
  { /* constructors */
    { /* empty */
      { /* metadata */
        SCHEME_KIND_CONS,
        0, /* variant */
        0  /* arity */
      }
      &SchemeList, /* type */
    },
    { /* cons */
      { /* metadata */
        SCHEME_KIND_CONS,
        0, /* variant */
        2  /* arity */
      },
      &SchemeList, /* type */
      { SCHEME_TYPE_ANY, &SchemeTypeList }
    }
  }
};

const SchemeCons *const SchemeListEmpty = SchemeList.variants;
const SchemeCons *const SchemeListCons = SchemeList.variants + 1;

const SchemeType SchemeOption = {
  { /* metadata */
    SCHEME_KIND_TYPE,
    2 /* variants */
  },
  { /* constructors */
    { /* none */
      { /* metadata */
        SCHEME_KIND_CONS,
        1, /* variant */
        0  /* arity */
      },
      &SchemeOption, /* type */
    },
    { /* some */
      { /* metadata */
        SCHEME_KIND_CONS,
        0, /* variant */
        1  /* arity */
      },
      &SchemeOption, /* type */
      { SCHEME_TYPE_ANY } /* argument types */
    }
  }
};

const SchemeCons *const SchemeOptionNone = SchemeOption.variants;
const SchemeCons *const SchemeOptionSome = SchemeOption.variants + 1;

const SchemeType SchemeEither = {
  { /* metadata */
    SCHEME_KIND_TYPE,
    2 /* variants */
  },
  { /* constructors */
    { /* left */
      { /* metadata */
        SCHEME_KIND_CONS,
        0, /* variant */
        1  /* arity */
      },
      &SchemeOption, /* type */
      { SCHEME_TYPE_ANY } /* argument type */
    },
    { /* right */
      { /* metadata */
        SCHEME_KIND_CONS,
        0, /* variant */
        1  /* arity */
      },
      &SchemeOption, /* type */
      { SCHEME_TYPE_ANY } /* argument type */
    }
  }
};

const SchemeCons *const SchemeEitherLeft = SchemeEither.variants;
const SchemeCons *const SchemeEitherRight = SchemeEither.variants + 1;
