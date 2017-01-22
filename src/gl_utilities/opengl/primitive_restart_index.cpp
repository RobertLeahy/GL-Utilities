#include <GL/glew.h>
#include <gl_utilities/opengl.hpp>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		primitive_restart_index_guard::primitive_restart_index_guard () : index_(in_place) {
			
			//	This should really be unsigned (as the primitive restart
			//	index itself is), but OpenGL has extremely poorly designed
			//	APIs across the board so the only way can retrieve the
			//	current global (LOL!) state is by getting an integer
			GLint curr;
			glGetIntegerv(GL_PRIMITIVE_RESTART_INDEX,&curr);
			raise();
			
			*index_=curr;
			
		}
		
		
		primitive_restart_index_guard::primitive_restart_index_guard (primitive_restart_index_guard && other) noexcept {
			
			std::swap(other.index_,index_);
			
		}
		
		
		primitive_restart_index_guard::~primitive_restart_index_guard () noexcept {
			
			if (!index_) return;
			
			glPrimitiveRestartIndex(*index_);
			//	No error checking: According to the documentation
			//	glPrimitiveRestartIndex cannot fail
			
		}
		
		
		primitive_restart_index_guard primitive_restart_index (GLuint index) {
			
			primitive_restart_index_guard retr;
			
			glPrimitiveRestartIndex(index);
			
			return retr;
			
		}
		
		
	}
	
	
}
