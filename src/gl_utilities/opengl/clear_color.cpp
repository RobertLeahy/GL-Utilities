#include <gl_utilities/opengl.hpp>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		clear_color_guard::clear_color_guard () : d_(in_place) {
			
			glGetFloatv(GL_COLOR_CLEAR_VALUE,d_->data());
			raise();
			
		}
		
		
		clear_color_guard::clear_color_guard (clear_color_guard && other) noexcept {
			
			std::swap(other.d_,d_);
			
		}
		
		
		clear_color_guard::~clear_color_guard () noexcept {
			
			if (!d_) return;
			
			auto & a=*d_;
			glClearColor(a[0],a[1],a[2],a[3]);
			
		}
		
		
		clear_color_guard clear_color (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
			
			clear_color_guard retr;
			
			glClearColor(red,green,blue,alpha);
			
			return retr;
			
		}
		
		
	}
	
	
}
