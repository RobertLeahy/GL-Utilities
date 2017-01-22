//	This has to be included first
#include <GL/glew.h>
#include <gl_utilities/opengl.hpp>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		void render_buffer::destroy () noexcept {
			
			if (handle_==0) return;
			
			glDeleteRenderbuffers(1,&handle_);
			handle_=0;
			
		}
		
		
		render_buffer::render_buffer () {
			
			glGenRenderbuffers(1,&handle_);
			opengl::raise();
			
		}
		
		
		render_buffer::render_buffer (render_buffer && other) noexcept : handle_(other.handle_) {
			
			other.handle_=0;
			
		}
		
		
		render_buffer & render_buffer::operator = (render_buffer && other) noexcept {
			
			destroy();
			
			std::swap(other.handle_,handle_);
			
			return *this;
			
		}
		
		
		render_buffer::~render_buffer () noexcept {
			
			destroy();
			
		}
		
		
		render_buffer::operator GLuint () const noexcept {
			
			return handle_;
			
		}
		
		
		void render_buffer::guard::destroy () noexcept {
			
			if (!handle_) return;
			
			glBindRenderbuffer(GL_RENDERBUFFER,*handle_);
			//	We hope this never throws...
			opengl::raise();
			handle_=nullopt;
			
		}
		
		
		render_buffer::guard::guard () : handle_(in_place) {
			
			//	Have to use a signed integer even
			//	though the conversion is possibly
			//	lossy because OpenGL doesn't provide
			//	a glGet* for unsigned integers
			GLint old;
			glGetIntegerv(GL_RENDERBUFFER_BINDING,&old);
			opengl::raise();
			
			*handle_=GLuint(old);
			
		}
		
		
		render_buffer::guard::guard (guard && other) noexcept {
			
			std::swap(other.handle_,handle_);
			
		}
		
		
		render_buffer::guard::~guard () noexcept {
			
			destroy();
			
		}
		
		
		render_buffer::guard render_buffer::bind () const {
			
			guard retr;
			
			glBindRenderbuffer(GL_RENDERBUFFER,handle_);
			opengl::raise();
			
			return retr;
			
		}
		
		
	}
	
	
}
