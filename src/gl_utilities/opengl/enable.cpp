#include <gl_utilities/opengl.hpp>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		enable_guard::state::state (GLenum cap) : cap_(cap) {
			
			e_=glIsEnabled(cap)==GL_TRUE;
			raise();
			
		}
		
		
		void enable_guard::state::restore () {
			
			if (e_) glEnable(cap_);
			else glDisable(cap_);
			raise();
			
		}
		
		
		enable_guard::enable_guard (GLenum cap) : s_(in_place,cap) {	}
		
		
		enable_guard::enable_guard (enable_guard && other) noexcept : s_(std::move(other.s_)) {
			
			other.s_=nullopt;
			
		}
		
		
		enable_guard::~enable_guard () noexcept {
			
			if (s_) s_->restore();
			
		}
		
		
		enable_guard enable (GLenum cap) {
			
			enable_guard retr(cap);
			
			glEnable(cap);
			raise();
			
			return retr;
			
		}
		
		
		enable_guard disable (GLenum cap) {
			
			enable_guard retr(cap);
			
			glDisable(cap);
			raise();
			
			return retr;
			
		}
		
		
	}
	
	
}
