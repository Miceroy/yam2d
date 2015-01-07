#include <slmath/slmath_pp.h>

#ifdef _MSC_VER
#pragma warning( disable : 4127 )
#endif

extern "C" {
#include <lua.h>
}

SLMATH_USING()

#define STATIC_LINKED
#include "slmath_wrap.cxx"
