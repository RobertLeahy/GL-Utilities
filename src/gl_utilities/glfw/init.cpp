#include <gl_utilities/glfw.hpp>
#include <gl_utilities/system_error.hpp>
#include <GL/glfw.h>
#include <system_error>


#ifdef __APPLE__
#include <sys/param.h>
#include <unistd.h>
#endif


namespace gl_utilities {
	
	
	namespace glfw {
		
		
		init::init () {
			
			//	As per documentation:
			//
			//	http://www.glfw.org/docs/latest/group__init.html#ga317aac130a235ab08c6db0834907d85e
			//
			//	On OSX there is a possibility that calling
			//	glfwInit changes the application's current
			//	working directory
			//
			//	Therefore we save the working directory before
			//	the call and restore it thereafter
			#ifdef __APPLE__
			char buffer [MAXPATHLEN];
			if (getcwd(buffer,MAXPATHLEN)==nullptr) raise();
			#endif
			
			if (glfwInit()==GL_FALSE) throw error("glfwInit failed");
			
			#ifdef __APPLE__
			if (chdir(buffer)==-1) raise();
			#endif
			
			
		}
		
		
		init::~init () noexcept {
			
			glfwTerminate();
			
		}
		
		
	}
	
	
}
