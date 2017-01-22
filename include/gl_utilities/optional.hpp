#pragma once

#ifdef __has_include
#if __has_include(<experimental/optional>)
#include <experimental/optional>
#define GL_UTILITIES_HAS_EXPERIMENTAL_OPTIONAL
#endif
#endif


namespace gl_utilities {


	#ifdef GL_UTILITIES_HAS_EXPERIMENTAL_OPTIONAL
	using std::experimental::optional;
	using std::experimental::nullopt_t;
	using std::experimental::nullopt;
	using std::experimental::bad_optional_access;
	using std::experimental::in_place_t;
	using std::experimental::in_place;
	#endif

	#if !defined(GL_UTILITIES_HAS_EXPERIMENTAL_OPTIONAL)
	#error No support for std::optional
	#endif


}
