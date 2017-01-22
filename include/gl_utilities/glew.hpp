/**
 *	\file
 */


#pragma once


#include <stdexcept>
#include <GL/glew.h>


namespace gl_utilities {
	
	
	/**
	 *	Contains utilities for interoperating with GLEW.
	 */
	namespace glew {
		
		
		/**
		 *	Represents an error originating from GLEW.
		 */
		class error : public std::runtime_error {
			
			
			private:
			
			
				GLenum code_;
			
			
			public:
			
			
				error (GLenum code);
			
			
		};
		
		
		/**
		 *	Calls glewInit when constructed.
		 *
		 *	GLEW does not seem to have a function that's the
		 *	dual of glewInit, but if I discover such a function
		 *	at some point this class' destructor will then call
		 *	that.
		 */
		class init {
			
			
			public:
			
			
				/**
				 *	Calls glewInit.
				 */
				init ();
			
			
		};
		
		
	}
	
	
}
