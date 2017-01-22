#include <gl_utilities/glew.hpp>
#include <string>


namespace gl_utilities {
	
	
	namespace glew {
		
		
		static std::string get_message (GLenum code) {
			
			auto cstr=glewGetErrorString(code);
			return std::string(reinterpret_cast<const char *>(cstr));
			
		}
		
		
		error::error (GLenum code) : std::runtime_error(get_message(code)), code_(code) {	}
		
		
	}
	
	
}
