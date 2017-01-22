//	We must include in this order because glew.h insists
//	on being included before gl.h
#include <GL/glew.h>
#include <gl_utilities/opengl.hpp>
#include <cstdlib>
#include <memory>
#include <new>
#include <sstream>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		void program::destroy () noexcept {
			
			if (handle_==0) return;
			
			glDeleteProgram(handle_);
			handle_=0;
			
		}
		
		
		program::program () : handle_(glCreateProgram()) {
			
			if (handle_==0) raise();
			
		}
		
		
		program::program (program && other) noexcept : handle_(other.handle_) {
			
			other.handle_=0;
			
		}
		
		
		program & program::operator = (program && other) noexcept {
			
			destroy();
			
			std::swap(other.handle_,handle_);
			
			return *this;
			
		}
		
		
		program::~program () noexcept {
			
			destroy();
			
		}
		
		
		program::operator GLuint () const noexcept {
			
			return handle_;
			
		}
		
		
		void program::attach (const shader & s) {
			
			glAttachShader(handle_,s);
			raise();
			
		}
		
		
		void program::link () {
			
			glLinkProgram(handle_);
			raise();
			
			GLint success;
			glGetProgramiv(handle_,GL_LINK_STATUS,&success);
			raise();
			if (success==GL_TRUE) return;
			
			//	Link failed
			GLint msglen;
			glGetProgramiv(handle_,GL_INFO_LOG_LENGTH,&msglen);
			raise();
			//	There's no info log but linking failed
			if (msglen==0) throw program_linking_error{};
			std::unique_ptr<char,void (*) (void *)> msg(reinterpret_cast<char *>(std::malloc(msglen)),&std::free);
			if (!msg) throw std::bad_alloc{};
			auto buffer=msg.get();
			glGetProgramInfoLog(handle_,msglen,nullptr,buffer);
			raise();
			throw program_linking_error(std::string(buffer,buffer+(msglen-1)));
			
		}
		
		
		void program::guard::destroy () noexcept {
			
			if (!old_) return;
			
			glUseProgram(*old_);
			//	We hope that this never throws, but if it
			//	does it seems (based on the documentation of
			//	glUseProgram) that it represents a logic
			//	error, in which case throwing into noexcept
			//	and calling std::terminate is a reasonable thing
			//	to do
			raise();
			old_=nullopt;
			
		}
		
		
		void program::guard::move (guard && other) noexcept {
			
			if (!other.old_) return;
			
			old_.emplace(*other.old_);
			other.old_=nullopt;
			
		}
		
		
		program::guard::guard () {
			
			//	Program handles are unsigned integers but
			//	OpenGL does not seem to have a way to retrieve
			//	unsigned integers throught the glGetX family
			//	of functions
			GLint old;
			glGetIntegerv(GL_CURRENT_PROGRAM,&old);
			raise();
			
			old_=old;
			
		}
		
		
		program::guard::guard (guard && other) noexcept {
			
			move(std::move(other));
			
		}
		
		
		program::guard::~guard () noexcept {
			
			destroy();
			
		}
		
		
		program::guard program::use () const {
			
			guard retr;
			
			glUseProgram(handle_);
			raise();
			
			return retr;
			
		}
		
		
		GLint program::attribute_location (const char * name) const {
			
			auto retr=glGetAttribLocation(handle_,name);
			raise();
			//	If the attribute isn't found OpenGL elects
			//	to not report this as an error through the
			//	usual channel and instead use a reserved value
			//	return code
			//
			//	Wonderful design, this is PHP levels of greatness
			if (retr!=-1) return retr;
			
			std::ostringstream ss;
			ss << "Could not find attribute \"" << name << "\"";
			throw attribute_not_found_error(ss.str());
			
		}
		
		
		GLint program::attribute_location (const std::string & name) const {
			
			return attribute_location(name.c_str());
			
		}
		
		
		GLint program::uniform_location (const char * name) const {
			
			auto retr=glGetUniformLocation(handle_,name);
			raise();
			//	See rant above in attribute_location
			if (retr!=-1) return retr;
			
			std::ostringstream ss;
			ss << "Could not find uniform \"" << name << "\"";
			throw uniform_not_found_error(ss.str());
			
		}
		
		
		GLint program::uniform_location (const std::string & name) const {
			
			return uniform_location(name.c_str());
			
		}
		
		
	}
	
	
}
