#include <GL/glew.h>
#include <gl_utilities/opengl.hpp>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		polygon_mode_guard::polygon_mode_guard () : m_(in_place) {
			
			//	Historically glGet* with GL_POLYGON_MODE returned
			//	two values:
			//
			//	http://lmb.informatik.uni-freiburg.de/people/reisert/opengl/doc/glGet.html
			//
			//	However as of OpenGL 3.2 you can no longer set separate
			//	values for front- and back-facing polygons:
			//
			//	http://stackoverflow.com/a/19672297/1007504
			//
			//	glGet* with GL_POLYGON_MODE now only seems to return
			//	one value (observed stepping through this code with the
			//	debugger)
			//
			//	To avoid undefined behaviour if the OpenGL version happens
			//	to be less than 3.2 an array of two integers is still passed
			//	although only the first value is regarded
			GLint arr [2];
			glGetIntegerv(GL_POLYGON_MODE,arr);
			raise();
			
			*m_=arr[0];
			
		}
		
		
		polygon_mode_guard::polygon_mode_guard (polygon_mode_guard && other) noexcept {
			
			std::swap(m_,other.m_);
			
		}
		
		
		polygon_mode_guard::~polygon_mode_guard () noexcept {
			
			if (!m_) return;
			
			glPolygonMode(GL_FRONT_AND_BACK,*m_);
			raise();
			
		}
		
		
		polygon_mode_guard polygon_mode (GLenum mode) {
			
			polygon_mode_guard retr;
			
			glPolygonMode(GL_FRONT_AND_BACK,mode);
			raise();
			
			return retr;
			
		}
		
		
	}
	
	
}
