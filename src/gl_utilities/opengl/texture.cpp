//	We must include in this order because glew.h insists
//	on being included before gl.h
#include <GL/glew.h>
#include <gl_utilities/opengl.hpp>
#include <stdexcept>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		void texture::destroy () noexcept {
			
			if (handle_==0) return;
			
			glDeleteTextures(1,&handle_);
			handle_=0;
			
		}
		
		
		texture::texture () {
			
			glGenTextures(1,&handle_);
			raise();
			
		}
		
		
		texture::texture (texture && other) noexcept : handle_(other.handle_) {
			
			other.handle_=0;
			
		}
		
		
		texture & texture::operator = (texture && other) noexcept {
			
			destroy();
			
			std::swap(other.handle_,handle_);
			
			return *this;
			
		}
		
		
		texture::~texture () noexcept {
			
			destroy();
			
		}
		
		
		texture::operator GLuint () const noexcept {
			
			return handle_;
			
		}
		
		
		void texture::guard::destroy () noexcept {
			
			if (!d_) return;
			
			glBindTexture(d_->type,d_->handle);
			raise();
			d_=nullopt;
			
		}
		
		
		static GLenum to_get (GLenum type) {
			
			switch (type) {
				
				case GL_TEXTURE_1D:
					return GL_TEXTURE_BINDING_1D;
				case GL_TEXTURE_1D_ARRAY:
					return GL_TEXTURE_BINDING_1D_ARRAY;
				case GL_TEXTURE_2D:
					return GL_TEXTURE_BINDING_2D;
				case GL_TEXTURE_2D_ARRAY:
					return GL_TEXTURE_BINDING_2D_ARRAY;
				case GL_TEXTURE_2D_MULTISAMPLE:
					return GL_TEXTURE_BINDING_2D_MULTISAMPLE;
				case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
					return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
				case GL_TEXTURE_3D:
					return GL_TEXTURE_BINDING_3D;
				case GL_TEXTURE_BUFFER:
					return GL_TEXTURE_BINDING_BUFFER;
				case GL_TEXTURE_CUBE_MAP:
					return GL_TEXTURE_BINDING_CUBE_MAP;
				case GL_TEXTURE_RECTANGLE:
					return GL_TEXTURE_BINDING_RECTANGLE;
				default:
					throw std::logic_error("Unknown texture type");
				
			}
			
		}
		
		
		texture::guard::guard (GLenum type) : d_(in_place) {
			
			//	Texture names are technically unsigned integers
			//	but there's no glGet* for unsigned integers...
			GLint handle;
			glGetIntegerv(to_get(type),&handle);
			raise();
			
			d_->handle=handle;
			d_->type=type;
			
		}
		
		
		texture::guard::guard (guard && other) noexcept : d_(other.d_) {
			
			other.d_=nullopt;
			
		}
		
		
		texture::guard::~guard () noexcept {
			
			destroy();
			
		}
		
		
		texture::guard texture::bind (GLenum type) const {
			
			guard retr(type);
			
			glBindTexture(type,handle_);
			raise();
			
			return retr;
			
		}
		
		
	}
	
	
}
