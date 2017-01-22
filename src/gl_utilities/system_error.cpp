#include <gl_utilities/system_error.hpp>
#include <errno.h>


namespace gl_utilities {
	
	
	std::error_code last_error () noexcept {
		
		return std::error_code(errno,std::system_category());
		
	}
	
	
	void raise () {
		
		throw std::system_error(last_error());
		
	}
	
	
}
