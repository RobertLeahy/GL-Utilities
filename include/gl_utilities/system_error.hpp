/**
 *	\file
 */


#pragma once


#include <system_error>


namespace gl_utilities {
	
	
	/**
	 *	Retrieves a std::error_code representing
	 *	the last operating system error.
	 */
	std::error_code last_error () noexcept;
	/**
	 *	Raises the last encountered operating
	 *	system error as a std::system_error.
	 */
	[[noreturn]]
	void raise ();
	
	
}
