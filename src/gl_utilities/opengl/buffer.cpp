//	We must include in this order because glew.h insists
//	on being included before gl.h
#include <GL/glew.h>
#include <gl_utilities/opengl.hpp>
#include <stdexcept>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		void buffer::destroy () noexcept {
			
			if (handle_==0) return;
			
			glDeleteBuffers(1,&handle_);
			handle_=0;
			
		}
		
		
		buffer::buffer () {
			
			glGenBuffers(1,&handle_);
			raise();
			
		}
		
		
		buffer::buffer (buffer && other) noexcept : handle_(other.handle_) {
			
			other.handle_=0;
			
		}
		
		
		buffer & buffer::operator = (buffer && other) noexcept {
			
			destroy();
			
			std::swap(other.handle_,handle_);
			
			return *this;
			
		}
		
		
		buffer::~buffer () noexcept {
			
			destroy();
			
		}
		
		
		buffer::operator GLuint () const noexcept {
			
			return handle_;
			
		}
		
		
		void buffer::guard::destroy () noexcept {
			
			if (!d_) return;
			
			glBindBuffer(d_->type,d_->handle);
			//	This should never throw, but just in
			//	case...
			raise();
			d_=nullopt;
			
		}
		
		
		static GLenum to_get (GLenum type) {
			
			switch (type) {
				
				case GL_ARRAY_BUFFER:
					return GL_ARRAY_BUFFER_BINDING;
				case GL_ATOMIC_COUNTER_BUFFER:
					return GL_ATOMIC_COUNTER_BUFFER_BINDING;
				case GL_COPY_READ_BUFFER:
					return GL_COPY_READ_BUFFER_BINDING;
				case GL_COPY_WRITE_BUFFER:
					return GL_COPY_WRITE_BUFFER_BINDING;
				case GL_DRAW_INDIRECT_BUFFER:
					return GL_DRAW_INDIRECT_BUFFER_BINDING;
				case GL_DISPATCH_INDIRECT_BUFFER:
					return GL_DRAW_INDIRECT_BUFFER_BINDING;
				case GL_ELEMENT_ARRAY_BUFFER:
					return GL_ELEMENT_ARRAY_BUFFER_BINDING;
				case GL_PIXEL_PACK_BUFFER:
					return GL_PIXEL_PACK_BUFFER_BINDING;
				case GL_PIXEL_UNPACK_BUFFER:
					return GL_PIXEL_UNPACK_BUFFER_BINDING;
				case GL_SHADER_STORAGE_BUFFER:
					return GL_SHADER_STORAGE_BUFFER_BINDING;
				case GL_TRANSFORM_FEEDBACK_BUFFER:
					return GL_TRANSFORM_FEEDBACK_BUFFER_BINDING;
				case GL_UNIFORM_BUFFER:
					return GL_UNIFORM_BUFFER_BINDING;
				default:
					throw std::logic_error("Unknown buffer type");
				
			}
			
		}
		
		
		buffer::guard::guard (GLenum type) : d_(in_place) {
			
			//	Buffer handles are unsigned integers but
			//	there's no glGet* for unsigned integers...
			GLint handle;
			glGetIntegerv(to_get(type),&handle);
			raise();
			
			d_->handle=handle;
			d_->type=type;
			
		}
		
		
		buffer::guard::guard (guard && other) noexcept {
			
			std::swap(other.d_,d_);
			
		}
		
		
		buffer::guard::~guard () noexcept {
			
			destroy();
			
		}
		
		
		buffer::guard buffer::bind (GLenum type) const {
			
			guard retr(type);
			
			glBindBuffer(type,handle_);
			raise();
			
			return retr;
			
		}
		
		
	}
	
	
}
