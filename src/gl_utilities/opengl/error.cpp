#include <gl_utilities/opengl.hpp>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		//	Dummy error message which allows error to
		//	be default constructible despite the fact
		//	std::runtime_error is not
		error::error () : std::runtime_error("OpenGL error occurred") {	}
		
		
	}
	
	
}
