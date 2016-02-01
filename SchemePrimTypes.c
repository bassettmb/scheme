#include "SchemePrimTypes.h"

const SchemeType SchemeTypePrimByte = {
  {
    SCHEME_KIND_TYPE,
    1,
  },
  {
    {
      {
        SCHEME_KIND_NAT,
        0,
        0,
        0
      },
      &SchemeTypePrimByte
    }
  }
};

const SchemeType *const SchemeConsPrimByte = SchemeTypePrimByte.variants;

const SchemeType SchemeTypePrimNat = {
  {
    SCHEME_KIND_TYPE
    1,
  },
  {
    {
      {
        SCHEME_KIND_NAT,
        0,
        0,
        0
      },
      &SchemeTypePrimNat
    }
  }
};

const SchemeType *const SchemeConsPrimNat = SchemeTypePrimNat.variants;

const SchemeType SchemeTypePrimInt = {
  {
    SCHEME_KIND_TYPE,
    1
  },
  {
    {
      {
        SCHEME_KIND_INT,
        0,
        0,
        0
      },
      &SchemeTypePrimInt
    }
  }
};

const SchemeType *const SchemeConsPrimInt = SchemeTypePrimInt.variants;

const SchemeType SchemeTypePrimDouble = {
  {
    SCHEME_KIND_TYPE,
    1
  },
  {
    {
      {
        SCHEME_KIND_DOUBLE,
        0,
        0,
        0
      },
      &SchemeTypePrimDouble
    }
  }
};

const SchemeType *const SchemeConsPrimDouble = SchemeTypePrimDouble.variants;

const SchemeType SchemeTypePrimChar = {
  {
    SCHEME_KIND_TYPE,
    1
  },
  {
    {
      {
        SCHEME_KIND_CHAR,
        0,
        0,
        0
      },
      &SchemeTypePrimChar
    }
  }
};

const SchemeType *const SchemeConsPrimChar = SchemeTypePrimChar.variants;

const SchemeType SchemeTypePrimArr = {
  {
    SCHEME_KIND_TYPE,
    1
  },
  {
    {
      {
        SCHEME_KIND_ARR,
        0,
        0,
        0
      },
      &SchemeTypePrimArr
    }
  }
};

const SchemeType *const SchemeConsPrimArr = SchemeTypePrimArr.variants;

const SchemeType SchemeTypePrimByteArr = {
  {
    SCHEME_KIND_TYPE,
    1
  },
  {
    {
      {
        SCHEME_KIND_BYTEARR,
        0,
        0,
        0
      },
      &SchemeTypePrimByteArr
    }
  }
};

const SchemeType *const SchemeConsPrimByteArr = SchemeTypePrimByteArr.variants;

const SchemeType SchemeTypePrimVar = {
  {
    SCHEME_KIND_TYPE,
    1
  },
  {
    {
      {
        SCHEME_KIND_VAR,
        0,
        0,
        0
      },
      &SchemeTypePrimVar
    }
  }
};

const SchemeType *const SchemeConsPrimVar = SchemeTypePrimVar.variants;
