#include <cassert>

#if defined(DISABLE_THROW_ASSERT)
#	define sv_assert(CONDITION, MESSAGE) assert(CONDITION)
#else
#	if __has_include("throw_assert.hpp") and !defined(NDEBUG)
#		include "throw_assert.hpp"
#		define sv_assert(CONDITION, MESSAGE) throw_assert(CONDITION, MESSAGE)
#	else
#		define sv_assert(CONDITION, MESSAGE) assert(CONDITION)
#	endif
#endif
