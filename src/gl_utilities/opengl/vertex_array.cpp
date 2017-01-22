//	We must include in this order because glew.h insists
//	on being included before gl.h
#include <GL/glew.h>
#include <gl_utilities/opengl.hpp>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		void vertex_array::destroy () noexcept {
			
			if (handle_==0) return;
			
			glDeleteVertexArrays(1,&handle_);
			handle_=0;
			
		}
		
		
		vertex_array::vertex_array () {
			
			glGenVertexArrays(1,&handle_);
			raise();
			
		}
		
		
		vertex_array::vertex_array (vertex_array && other) noexcept : handle_(other.handle_) {
			
			other.handle_=0;
			
		}
		
		
		vertex_array & vertex_array::operator = (vertex_array && other) noexcept {
			
			destroy();
			
			std::swap(handle_,other.handle_);
			
			return *this;
			
		}
		
		
		vertex_array::~vertex_array () noexcept {
			
			destroy();
			
		}
		
		
		vertex_array::operator GLuint () const noexcept {
			
			return handle_;
			
		}
		
		
		void vertex_array::guard::destroy () noexcept {
			
			if (!handle_) return;
			
			glBindVertexArray(*handle_);
			raise();
			handle_=nullopt;
			
		}
		
		
		vertex_array::guard::guard () : handle_(in_place) {
			
			//	OpenGL doesn't have a glGet* function for
			//	unsigned integers
			//
			//	I absolve myself of any blame for lossy
			//	conversions and/or undefined behaviour
			GLint old;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING,&old);
			raise();
			
			*handle_=old;
			
		}
		
		
		vertex_array::guard::guard (guard && other) noexcept {
			
			std::swap(handle_,other.handle_);
			
		}
		
		
		vertex_array::guard::~guard () noexcept {
			
			destroy();
			
		}
		
		
		vertex_array::guard vertex_array::bind () const {
			
			guard retr;
			
			glBindVertexArray(handle_);
			raise();
			
			return retr;
			
		}
		
		
	}
	
	
}
