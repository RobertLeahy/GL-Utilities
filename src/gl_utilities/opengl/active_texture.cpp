//	We must include in this order because glew.h insists
//	on being included before gl.h
#include <GL/glew.h>
#include <gl_utilities/opengl.hpp>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		void active_texture_guard::destroy () noexcept {
			
			if (!handle_) return;
			
			glActiveTexture(*handle_);
			raise();
			handle_=nullopt;
			
		}
		
		
		active_texture_guard::active_texture_guard () : handle_(in_place) {
			
			//	Signed to unsigned conversion obviously but
			//	glGet* doesn't leave us much choice by not
			//	having an unsigned version
			GLint old;
			glGetIntegerv(GL_ACTIVE_TEXTURE,&old);
			raise();
			
			*handle_=old;
			
		}
		
		
		active_texture_guard::active_texture_guard (active_texture_guard && other) noexcept {
			
			std::swap(handle_,other.handle_);
			
		}
		
		
		active_texture_guard::~active_texture_guard () noexcept {
			
			destroy();
			
		}
		
		
		active_texture_guard active_texture (unsigned num) {
			
			active_texture_guard retr;
			
			glActiveTexture(GL_TEXTURE0+static_cast<GLenum>(num));
			raise();
			
			return retr;
			
		}
		
		
	}
	
	
}
