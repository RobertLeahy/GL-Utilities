#include <gl_utilities/opengl.hpp>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		basic_error::basic_error () : error(std::string{}) {	}
		
		
		void basic_error::add (GLenum code) {
			
			auto pair=codes_.insert(code);
			//	Code already present, no need to add
			//	the error message for it to the aggregate
			//	string again
			if (!pair.second) return;
			
			auto str=gluErrorString(code);
			//	The provided error code is invalid
			if (str==nullptr) {
				
				codes_.erase(pair.first);
				throw std::logic_error("Invalid GLenum value");
				
			}
			
			if (!msg_.empty()) msg_+="; ";
			msg_+=reinterpret_cast<const char *>(str);
			
		}
		
		
		const char * basic_error::what () const noexcept {
			
			return msg_.c_str();
			
		}
		
		
		optional<basic_error> last_error () {
			
			optional<basic_error> retr;
			
			GLenum c;
			while ((c=glGetError())!=GL_NO_ERROR) {
				
				if (!retr) retr.emplace();
				retr->add(c);
				
			}
			
			return retr;
			
		}
		
		
		void raise () {
			
			auto ex=last_error();
			if (ex) throw *ex;
			
		}
		
		
	}
	
	
}
