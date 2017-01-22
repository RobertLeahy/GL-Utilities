/**
 *	\file
 */


#pragma once


#include <stdexcept>


namespace gl_utilities {
	
	
	/**
	 *	Contains C++ utilities for interoperating
	 *	with GLFW.
	 */
	namespace glfw {
		
		
		/**
		 *	Represents an error encountered invoking
		 *	a GLFW function.
		 */
		class error : public std::runtime_error {
		
			
			public:
			
			
				using std::runtime_error::runtime_error;
		
		
		};
		
		
		/**
		 *	An RAII wrapper which performs calls to
		 *	glfwInit when constructed and glfwTerminate
		 *	when destroyed.
		 */
		class init {
			
			
			public:
			
			
				init (const init &) = delete;
				init (init &&) = delete;
				init & operator = (const init &) = delete;
				init & operator = (init &&) = delete;
				
				
				/**
				 *	Calls glfwInit.
				 */
				init ();
				
				
				/**
				 *	Calls glfwTerminate.
				 */
				~init () noexcept;
			
			
		};
		
		
		class window {
			
			
			public:
			
			
				/**
				 *	Calls glfwOpenWindow.
				 *
				 *	\param [in] width
				 *		Passed as the first parameter of glfwOpenWindow.
				 *	\param [in] height
				 *		Passed as the second parameter of glfwOpenWindow.
				 *	\param [in] redbits
				 *		Passed as the third parameter of glfwOpenWindow.
				 *	\param [in] greenbits
				 *		Passed as the fourth parameter of glfwOpenWindow.
				 *	\param [in] bluebits
				 *		Passed as the fifth parameter of glfwOpenWindow.
				 *	\param [in] alphabits
				 *		Passed as the sixth parameter of glfwOpenWindow.
				 *	\param [in] depthbits
				 *		Passed as the seventh parameter of glfwOpenWindow.
				 *	\param [in] stencilbits
				 *		Passed as the eighth parameter of glfwOpenWindow.
				 *	\param [in] mode
				 *		Passed as the ninth parameter of glfwOpenWindow.
				 */
				window (int width, int height, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits, int mode);
				
				
				/**
				 *	Calls glfwCloseWindow.
				 */
				~window () noexcept;
			
			
		};
		
		
	}
	
	
}
