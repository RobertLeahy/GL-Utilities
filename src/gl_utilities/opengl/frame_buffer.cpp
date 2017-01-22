//	This has to be included before GL/gl.h...
#include <GL/glew.h>
#include <gl_utilities/opengl.hpp>
#include <stdexcept>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		void frame_buffer::destroy () noexcept {
			
			if (handle_==0) return;
			
			glDeleteFramebuffers(1,&handle_);
			handle_=0;
			
		}
		
		
		frame_buffer::frame_buffer () {
			
			glGenFramebuffers(1,&handle_);
			opengl::raise();
			
		}
		
		
		frame_buffer::frame_buffer (frame_buffer && other) noexcept : handle_(other.handle_) {
			
			other.handle_=0;
			
		}
		
		
		frame_buffer & frame_buffer::operator = (frame_buffer && other) noexcept {
			
			destroy();
			
			std::swap(handle_,other.handle_);
			
			return *this;
			
		}
		
		
		frame_buffer::~frame_buffer () noexcept {
			
			destroy();
			
		}
		
		
		frame_buffer::operator GLuint () const noexcept {
			
			return handle_;
			
		}
		
		
		void frame_buffer::guard::destroy () noexcept {
			
			if (!d_) return;
			
			if (d_->read) {
				
				glBindFramebuffer(GL_READ_FRAMEBUFFER,*(d_->read));
				//	Hopefully this doesn't throw
				opengl::raise();
				
			}
			
			if (d_->draw) {
				
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER,*(d_->draw));
				//	Again, hopefully this doesn't throw...
				opengl::raise();
				
			}
			
			d_=nullopt;
			
		}
		
		
		static GLuint get (GLenum type) {
			
			//	This should be an unsigned int but
			//	there's no overload for glGet* that
			//	actually retrieves that...genius...
			GLint fb;
			glGetIntegerv(type,&fb);
			opengl::raise();
			
			return GLuint(fb);
			
		}
		
		
		static GLuint get_draw () {
			
			return get(GL_DRAW_FRAMEBUFFER_BINDING);
			
		}
		
		
		static GLuint get_read () {
			
			return get(GL_READ_FRAMEBUFFER_BINDING);
			
		}
		
		
		frame_buffer::guard::guard (GLenum type) : d_(in_place) {
			
			switch (type) {
				
				case GL_DRAW_FRAMEBUFFER:
					d_->draw=get_draw();
					break;
				case GL_READ_FRAMEBUFFER:
					d_->read=get_read();
					break;
				case GL_FRAMEBUFFER:
					d_->draw=get_draw();
					d_->read=get_read();
					break;
				default:
					throw std::logic_error("Unrecognized frame buffer binding");
				
			}
			
		}
		
		
		frame_buffer::guard::guard (guard && other) noexcept {
			
			std::swap(other.d_,d_);
			
		}
		
		
		frame_buffer::guard::~guard () noexcept {
			
			destroy();
			
		}
		
		
		frame_buffer::guard frame_buffer::bind (GLenum type) const {
			
			guard retr(type);
			
			glBindFramebuffer(type,handle_);
			opengl::raise();
			
			return retr;
			
		}
		
		
	}
	
	
}
