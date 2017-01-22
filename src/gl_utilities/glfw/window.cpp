#include <gl_utilities/glfw.hpp>
#include <GL/glfw.h>


namespace gl_utilities {
	
	
	namespace glfw {
		
		
		window::window (int width, int height, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits, int mode) {
			
			if (glfwOpenWindow(width,height,redbits,greenbits,bluebits,alphabits,depthbits,stencilbits,mode)==GL_FALSE) throw error("glfwOpenWindow failed");
			
		}
		
		
		window::~window () noexcept {
			
			glfwCloseWindow();
			
		}
		
		
	}
	
	
}
