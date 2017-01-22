//	We must include in this order because glew.h insists
//	on being included before gl.h
#include <GL/glew.h>
#include <gl_utilities/opengl.hpp>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <memory>
#include <new>
#include <sstream>
#include <utility>


namespace gl_utilities {
	
	
	namespace opengl {
		
		
		void shader::inner::destroy () noexcept {
			
			if (handle_==0) return;
			
			glDeleteShader(handle_);
			handle_=0;
			
		}
		
		
		shader::inner::inner (GLenum type) : handle_(glCreateShader(type)) {
			
			if (handle_==0) raise();
			
		}
		
		
		shader::inner::inner (inner && other) noexcept : handle_(other.handle_) {
			
			other.handle_=0;
			
		}
		
		
		shader::inner & shader::inner::operator = (inner && other) noexcept {
			
			destroy();
			
			std::swap(other.handle_,handle_);
			
			return *this;
			
		}
		
		
		shader::inner::~inner () noexcept {
			
			destroy();
			
		}
		
		
		shader::inner::operator GLuint () const noexcept {
			
			return handle_;
			
		}
		
		
		shader shader::from_file (GLenum type, const std::string & filename) {
			
			std::ifstream is(filename.c_str());
			if (!is) {
				
				std::ostringstream ss;
				ss << "Could not open " << filename;
				throw shader_compilation_error(ss.str());
				
			}
			
			return shader(type,is);
			
		}
		
		
		shader::shader (GLenum type, std::istream & is) : handle_(type) {
			
			if (handle_==0) raise();
			
			//	Read the entire source code stream
			typedef std::istreambuf_iterator<char> iterator;
			std::string src(iterator(is),iterator{});
			
			//	Compile
			auto cstr=src.c_str();
			//	We assume that this conversion is not lossy
			//
			//	NOTE: This may be a very bad assumption, depending
			//	on the lengths involved
			GLint len(src.size());
			glShaderSource(handle_,1,&cstr,&len);
			glCompileShader(handle_);
			//	glCompileShader may fail if a shader compiler
			//	is not supported, and therefore we should
			//	check if it failed and throw if it did
			//	(glShaderSource only fails if handle_ is
			//	invalid, but we know it's valid since we
			//	just got it from glCreateShader
			raise();
			
			//	Check compilation result
			GLint success;
			glGetShaderiv(handle_,GL_COMPILE_STATUS,&success);
			raise();
			if (success==GL_TRUE) return;
			
			//	Compilation failed
			GLint msglen;
			glGetShaderiv(handle_,GL_INFO_LOG_LENGTH,&msglen);
			raise();
			//	There's an error but there's no log
			if (msglen==0) throw shader_compilation_error{};
			std::unique_ptr<char,void (*) (void *)> msg(reinterpret_cast<char *>(std::malloc(msglen)),&std::free);
			if (!msg) throw std::bad_alloc{};
			auto buffer=msg.get();
			glGetShaderInfoLog(handle_,msglen,nullptr,buffer);
			raise();
			throw shader_compilation_error(std::string(buffer,buffer+(msglen-1)));
			
		}
		
		
		shader::operator GLuint () const noexcept {
			
			return handle_;
			
		}
		
		
	}
	
	
}
