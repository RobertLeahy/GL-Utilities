#include <gl_utilities/opengl.hpp>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		viewport_guard::viewport_guard () : d_(in_place) {
			
			//	Two of the parameters are signed integers which
			//	can never be negative, and the other two are
			//	GLsizei (judging from the parameters glViewport
			//	takes)
			//
			//	This is typical wonderful OpenGL design, we
			//	carefully decide on types:
			//
			//	-	Signed integer which can never be negative
			//	-	Size type
			//
			//	And then completely disregard the differences/distinction
			//	when you want to fetch them back
			glGetIntegerv(GL_VIEWPORT,reinterpret_cast<GLint *>(d_->data()));
			opengl::raise();
			
		}
		
		
		viewport_guard::viewport_guard (viewport_guard && other) noexcept {
			
			std::swap(other.d_,d_);
			
		}
		
		
		viewport_guard::~viewport_guard () noexcept {
			
			if (!d_) return;
			
			auto & a=*d_;
			glViewport(a[0],a[1],a[2],a[3]);
			//	Hopefully this never actually throws...
			opengl::raise();
			
		}
		
		
		viewport_guard viewport (GLint x, GLint y, GLsizei width, GLsizei height) {
			
			viewport_guard retr;
			
			glViewport(x,y,width,height);
			opengl::raise();
			
			return retr;
			
		}
		
		
	}
	
	
}
