#include <gl_utilities/glew.hpp>
#include <gl_utilities/opengl.hpp>
#include <gl_utilities/optional.hpp>


namespace gl_utilities {
	
	
	namespace glew {
		
		
		init::init () {
			
			glewExperimental=true;
			auto c=glewInit();
			if (c!=GLEW_OK) throw error(c);
			
			//	Apparently GLEW causes GL_INVALID_ENUM:
			//
			//	http://stackoverflow.com/a/20035078/1007504
			optional<opengl::basic_error> e;
			while ((c=glGetError())!=GL_NO_ERROR) {
				
				if (c==GL_INVALID_ENUM) continue;
				
				if (!e) e.emplace();
				e->add(c);
				
			}
			
			if (e) throw *e;
			
		}
		
		
	}
	
	
}
