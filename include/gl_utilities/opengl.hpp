/**
 *	\file
 */


#pragma once


#include "optional.hpp"
#include <array>
#include <istream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


namespace gl_utilities {
	
	
	/**
	 *	Contains utilities for interoperating with OpenGL.
	 */
	namespace opengl {
		
		
		/**
		 *	A base class for all OpenGL exceptions.
		 */
		class error : public std::runtime_error {
			
			
			public:
			
			
				using std::runtime_error::runtime_error;
				error ();
			
			
		};
		
		
		/**
		 *	Encapsulates an OpenGL error represented by
		 *	one or more GLenum values.
		 */
		class basic_error : public error {
			
			
			private:
			
			
				std::unordered_set<GLenum> codes_;
				std::string msg_;
			
			
			public:
			
			
				basic_error ();
				
				
				void add (GLenum);
				
				
				virtual const char * what () const noexcept override;
			
			
		};
		
		
		/**
		 *	Encapsulates an error encountered compiling an OpenGL
		 *	shader.
		 */
		class shader_compilation_error : public error {
			
			
			public:
			
			
				using error::error;
			
			
		};
		
		
		/**
		 *	Encapsulates an error encountered linking an OpenGL
		 *	shader program.
		 */
		class program_linking_error : public error {
			
			
			public:
			
			
				using error::error;
			
			
		};
		
		
		/**
		 *	Encapsulates an error encountered attempting to find
		 *	the location of an attribute variable.
		 */
		class attribute_not_found_error : public error {
			
			
			public:
			
			
				using error::error;
			
			
		};
		
		
		/**
		 *	Encapsulates an error encountered attempting to find
		 *	the location of a uniform variable in a program.
		 */
		class uniform_not_found_error : public error {
			
			
			public:
			
			
				using error::error;
			
			
		};
		
		
		/**
		 *	Retrieves a basic_error exception object representing
		 *	the last OpenGL error, if any.
		 *
		 *	\return
		 *		An engaged optional containing a basic_error exception
		 *		object if one or more OpenGL errors occurred,
		 *		a disengaged optional otherwise.
		 */
		optional<basic_error> last_error ();
		/**
		 *	Raises any OpenGL errors as an exception.
		 */
		void raise ();
		
		
		/**
		 *	Encapsulates an OpenGL shader.
		 */
		class shader {
			
			
			private:
			
			
				class inner {
					
					
					private:
					
					
						GLuint handle_;
						
						
						void destroy () noexcept;
						
						
					public:
					
					
						inner () = delete;
						inner (const inner &) = delete;
						inner & operator = (const inner &) = delete;
						
						
						inner (GLenum);
						inner (inner &&) noexcept;
						inner & operator = (inner &&) noexcept;
						
						
						~inner () noexcept;
						
						
						operator GLuint () const noexcept;
					
					
				};
				
				
				inner handle_;
				
				
			public:
			
			
				/**
				 *	Compiles a shader from a file.
				 *
				 *	\param [in] type
				 *		The type of shader to compile.
				 *	\param [in] filename
				 *		The name of the file.
				 *
				 *	\return
				 *		A shader object.
				 */
				static shader from_file (GLenum type, const std::string & filename);
				
				
				shader () = default;
				shader (const shader &) = delete;
				shader (shader &&) = default;
				shader & operator = (const shader &) = delete;
				shader & operator = (shader &&) = default;
				
				
				/**
				 *	Creates and compiles a shader.
				 *
				 *	\param [in] type
				 *		The type of shader to create.  The acceptable
				 *		values for this parameter are defined by OpenGL.
				 *	\param [in] is
				 *		An input stream which yields the source of the
				 *		shader.
				 */
				shader (GLenum type, std::istream & is);
				
				
				/**
				 *	Retrieves a handle which may be passed to OpenGL
				 *	C functions to reference this shader.
				 *
				 *	\return
				 *		An integer.
				 */
				operator GLuint () const noexcept;
			
			
		};
		
		
		/**
		 *	Encapsulates an OpenGL shader program.
		 */
		class program {
			
			
			private:
			
			
				GLuint handle_;
				
				
				void destroy () noexcept;
				
				
			public:
			
			
				program (const program &) = delete;
				program & operator = (const program &) = delete;
				
				
				program ();
				program (program && other) noexcept;
				program & operator = (program && other) noexcept;
				
				
				~program () noexcept;
				
				
				/**
				 *	Retrieves a handle which may be passed to OpenGL
				 *	C functions to reference this program.
				 *
				 *	\return
				 *		An integer.
				 */
				operator GLuint () const noexcept;
				
				
				/**
				 *	Attaches a shader to this program.
				 *
				 *	\param [in] s
				 *		The s to attach.
				 */
				void attach (const shader & s);
				
				
				/**
				 *	Links the program.
				 */
				void link ();
				
				
				/**
				 *	A scope guard which uninstalls the current program
				 *	when destroyed.
				 */
				class guard {
					
					
					private:
					
					
						optional<GLuint> old_;
						
						
						void destroy () noexcept;
						void move (guard &&) noexcept;
						
						
					public:
					
					
						guard (const guard &) = delete;
						guard & operator = (const guard &) = delete;
						guard & operator = (guard &&) = delete;
						
						
						/**
						 *	Saves the current program.
						 */
						guard ();
						/**
						 *	Moves from a guard object.  After this constructor
						 *	completes the newly constructed guard will guard the
						 *	current scope and the moved from guard will no longer
						 *	be engaged (i.e. the moved from scope will no longer
						 *	be guarded).
						 *
						 *	\param [in] other
						 *		The guard to move from.
						 */
						guard (guard && other) noexcept;
						
						
						/**
						 *	Restores the saved program.
						 */
						~guard () noexcept;
					
					
				};
				
				
				/**
				 *	Sets this program to be the current program.
				 *
				 *	The program which was current when this function
				 *	was invoked will be saved and a guard which will
				 *	restore it when it goes out of scope is returned.
				 *
				 *	Be sure to save the return value of this function
				 *	in a local variable or the program which was current
				 *	when this function was invoked will be restored
				 *	immediately.
				 *
				 *	\return
				 *		A guard which will restore the previously
				 *		current program when it goes out of scope.
				 */
				guard use () const;
				
				
				/**
				 *	Obtains a handle which may be passed to OpenGL to
				 *	refer to the location of an attribute within this
				 *	program.
				 *
				 *	\param [in] name
				 *		A string giving the name of the attribute.
				 *
				 *	\return
				 *		An integer.
				 */
				GLint attribute_location (const char * name) const;
				GLint attribute_location (const std::string & name) const;
				
				
				/**
				 *	Obtains a handle which may be passed to OpenGL to refer
				 *	to the location of a uniform variable within this
				 *	program.
				 *
				 *	\param [in] name
				 *		A string giving the name of the uniform variable.
				 *
				 *	\return
				 *		An integer.
				 */
				GLint uniform_location (const char * name) const;
				GLint uniform_location (const std::string & name) const;
			
			
		};
		
		
		/**
		 *	Encapsulates an OpenGL buffer object.
		 */
		class buffer {
			
			
			private:
			
			
				GLuint handle_;
				
				
				void destroy () noexcept;
				
				
			public:
			
			
				buffer (const buffer &) = delete;
				buffer & operator = (const buffer &) = delete;
				
				
				buffer ();
				buffer (buffer &&) noexcept;
				buffer & operator = (buffer &&) noexcept;
				
				
				~buffer () noexcept;
				
				
				/**
				 *	Retrieves a handle which may be used to refer
				 *	to this buffer when calling OpenGL C functions.
				 *
				 *	\return
				 *		An integer.
				 */
				operator GLuint () const noexcept;
				
				
				class guard {
					
					
					private:
					
					
						class details {
							
							
							public:
							
							
								GLuint handle;
								GLenum type;
							
							
						};
						
						
						optional<details> d_;
						
						
						void destroy () noexcept;
						
						
					public:
					
					
						guard () = delete;
						guard (const guard &) = delete;
						guard & operator = (const guard &) = delete;
						guard & operator = (guard &&) = delete;
						
						
						explicit guard (GLenum);
						guard (guard &&) noexcept;
						
						
						~guard () noexcept;
					
					
				};
				
				
				/**
				 *	Binds this buffer object.
				 *
				 *	Care must be taken to store the return value of this
				 *	function or the binding will be immediately reverted.
				 *
				 *	\param [in] type
				 *		The target to which this buffer shall be
				 *		bound.
				 *
				 *	\return
				 *		An object which shall revert this binding (restoring
				 *		the previous binding) when it goes out of scope.
				 */
				guard bind (GLenum type) const;
			
			
		};
		
		
		/**
		 *	Encapsulates an OpenGL texture name.
		 */
		class texture {
			
			
			private:
			
			
				GLuint handle_;
				
				
				void destroy () noexcept;
				
				
			public:
			
			
				texture (const texture &) = delete;
				texture & operator = (const texture &) = delete;
				
				
				texture ();
				texture (texture &&) noexcept;
				texture & operator = (texture &&) noexcept;
				
				
				~texture () noexcept;
				
				
				/**
				 *	Returns a handle which may be used to refer
				 *	to this texture when calling OpenGL C functions.
				 *
				 *	\return
				 *		An integer.
				 */
				operator GLuint () const noexcept;
				
				
				class guard {
					
					
					private:
					
					
						class details {
							
							
							public:
							
							
								GLuint handle;
								GLenum type;
							
							
						};
						
						
						optional<details> d_;
						
						
						void destroy () noexcept;
						
						
					public:
					
					
						guard () = delete;
						guard (const guard &) = delete;
						guard & operator = (const guard &) = delete;
						guard & operator = (guard &&) = delete;
						
						
						explicit guard (GLenum);
						guard (guard &&) noexcept;
						
						
						~guard () noexcept;
					
					
				};
				
				
				/**
				 *	Binds this texture.
				 *
				 *	Care must be taken to store the return value of this
				 *	function or the binding will immediately be reverted.
				 *
				 *	\param [in] type
				 *		The type of texture as which to bind this texture.
				 *
				 *	\return
				 *		A guard object which will revert the binding of type
				 *		\em type to its previous value when it goes out of scope.
				 */
				guard bind (GLenum type) const;
			
			
		};
		
		
		/**
		 *	Encapsulates an OpenGL vertex array name.
		 */
		class vertex_array {
			
			
			private:
			
			
				GLuint handle_;
				
				
				void destroy () noexcept;
				
				
			public:
			
			
				vertex_array (const vertex_array &) = delete;
				vertex_array & operator = (const vertex_array &) = delete;
				
				
				vertex_array ();
				vertex_array (vertex_array &&) noexcept;
				vertex_array & operator = (vertex_array &&) noexcept;
				
				
				~vertex_array () noexcept;
				
				
				/**
				 *	Retrieves a handle which may be used to represent
				 *	this vertex array when calling OpenGL C functions.
				 *
				 *	\return
				 *		An integer.
				 */
				operator GLuint () const noexcept;
				
				
				class guard {
					
					
					private:
					
					
						optional<GLuint> handle_;
						
						
						void destroy () noexcept;
						
						
					public:
					
					
						guard (const guard &) = delete;
						guard & operator = (const guard &) = delete;
						guard & operator = (guard &&) = delete;
						
						
						guard ();
						guard (guard &&) noexcept;
						
						
						~guard () noexcept;
					
					
				};
				
				
				/**
				 *	Binds this vertex array.
				 *
				 *	Be sure to save the return value of this method in
				 *	a variable or the binding will be immediately undone.
				 *
				 *	\return
				 *		An object which will reverse the binding (restoring
				 *		the binding which was current when this method was
				 *		invoked) when it goes out of scope.
				 */
				guard bind () const;
			
			
		};
		
		
		class active_texture_guard {
			
			
			private:
			
			
				optional<GLenum> handle_;
				
				
				void destroy () noexcept;
				
				
			public:
			
			
				active_texture_guard (const active_texture_guard &) = delete;
				active_texture_guard & operator = (const active_texture_guard &) = delete;
				
				
				active_texture_guard ();
				active_texture_guard (active_texture_guard &&) noexcept;
				
				
				~active_texture_guard () noexcept;
			
			
		};
		
		
		/**
		 *	Selects an active texture unit.
		 *
		 *	\param [in] num
		 *		The number of the active texture unit to
		 *		activate.  The number of such units is
		 *		implementation defined.
		 *
		 *	\return
		 *		An object which will restore the texture unit
		 *		active when this function was called when it goes
		 *		out of scope.
		 */
		active_texture_guard active_texture (unsigned num);
		
		
		/**
		 *	Encapsulates an OpenGL frame buffer object name.
		 */
		class frame_buffer {
			
			
			private:
			
			
				GLuint handle_;
				
				
				void destroy () noexcept;
				
				
			public:
			
			
				frame_buffer (const frame_buffer &) = delete;
				frame_buffer & operator = (const frame_buffer &) = delete;
				
				
				frame_buffer ();
				frame_buffer (frame_buffer &&) noexcept;
				frame_buffer & operator = (frame_buffer &&) noexcept;
				
				
				~frame_buffer () noexcept;
				
				
				/**
				 *	Retrieves a handle which may be passed to OpenGL
				 *	C functions to refer to this frame buffer.
				 *
				 *	\return
				 *		An integer.
				 */
				operator GLuint () const noexcept;
				
				
				class guard {
					
					
					private:
					
					
						class details {
							
							
							public:
							
							
								optional<GLuint> read;
								optional<GLuint> draw;
							
							
						};
						
						
						optional<details> d_;
						
						
						void destroy () noexcept;
						
						
					public:
					
					
						guard () = delete;
						guard (const guard &) = delete;
						guard & operator = (const guard &) = delete;
						guard & operator = (guard &&) = delete;
						
						
						explicit guard (GLenum);
						guard (guard &&) noexcept;
						
						
						~guard () noexcept;
					
					
				};
				
				
				/**
				 *	Makes this frame buffer the currently active
				 *	frame buffer on a certain frame buffer binding.
				 *
				 *	Be sure to store the returned object in a variable
				 *	as the binding performed by this method will be
				 *	reversed as soon as that object goes out of scope.
				 *
				 *	\param [in] type
				 *		The frame buffer binding to which this frame
				 *		buffer shall be bound.
				 *
				 *	\return
				 *		An object which will revert the binding when it
				 *		goes out of scope.
				 */
				guard bind (GLenum type) const;
			
			
		};
		
		
		/**
		 *	Encapsulates an OpenGL render buffer name.
		 */
		class render_buffer {
			
			
			private:
			
			
				GLuint handle_;
				
				
				void destroy () noexcept;
				
				
			public:
			
			
				render_buffer (const render_buffer &) = delete;
				render_buffer & operator = (const render_buffer &) = delete;
				
				
				render_buffer ();
				render_buffer (render_buffer &&) noexcept;
				render_buffer & operator = (render_buffer &&) noexcept;
				
				
				~render_buffer () noexcept;
				
				
				/**
				 *	Retrieves a handle which may be pasesd to
				 *	OpenGL C functions to refer to this render
				 *	buffer.
				 *
				 *	\return
				 *		An integer.
				 */
				operator GLuint () const noexcept;
				
				
				class guard {
					
					
					private:
					
					
						optional<GLuint> handle_;
						
						
						void destroy () noexcept;
						
						
					public:
					
					
						guard (const guard &) = delete;
						guard & operator = (const guard &) = delete;
						guard & operator = (guard &&) = delete;
						
						
						guard ();
						guard (guard &&) noexcept;
						
						
						~guard () noexcept;
					
					
				};
				
				
				/**
				 *	Binds this render buffer.
				 *
				 *	Be careful to store the returned value in a variable otherwise
				 *	it will immediately go out of scope thereby restoring the
				 *	previous binding and undoing all of the work done by this
				 *	method.
				 *
				 *	\return
				 *		An object which will restore the render buffer binding
				 *		which was current when this method was invoked when
				 *		it goes out of scope.
				 */
				guard bind () const;
			
			
		};
		
		
		class primitive_restart_index_guard {
			
			
			private:
			
			
				optional<GLuint> index_;
				
				
			public:
			
			
				primitive_restart_index_guard (const primitive_restart_index_guard &) = delete;
				primitive_restart_index_guard & operator = (const primitive_restart_index_guard &) = delete;
				primitive_restart_index_guard & operator = (primitive_restart_index_guard &&) = delete;
				
				
				primitive_restart_index_guard ();
				primitive_restart_index_guard (primitive_restart_index_guard &&) noexcept;
				
				
				~primitive_restart_index_guard () noexcept;
			
			
		};
		
		
		/**
		 *	Sets the primitive restart index by calling glPrimitiveRestartIndex.
		 *
		 *	\param [in] index
		 *		The index which will signal that a new primitive is to
		 *		begin.
		 *
		 *	\return
		 *		An object which will restore the previous primitive restart index
		 *		when it goes out of scope.
		 */
		primitive_restart_index_guard primitive_restart_index (GLuint index);
		
		
		class polygon_mode_guard {
			
			
			private:
			
			
				optional<GLenum> m_;
				
				
			public:
			
			
				polygon_mode_guard (const polygon_mode_guard &) = delete;
				polygon_mode_guard & operator = (const polygon_mode_guard &) = delete;
				polygon_mode_guard & operator = (polygon_mode_guard &&) = delete;
				
				
				polygon_mode_guard ();
				polygon_mode_guard (polygon_mode_guard &&) noexcept;
				
				
				~polygon_mode_guard () noexcept;
			
			
		};
		
		
		/**
		 *	Sets the polygon mode by calling glPolygonMode.
		 *
		 *	This function does not support the \em face parameter to
		 *	glPolygonMode as all values except GL_FRONT_AND_BACK (which this
		 *	function uses) have been deprecated as of OpenGL 3.2.
		 *
		 *	\param [in] mode
		 *		The way affected polygons will be rasterized.  See documentation
		 *		for glPolygonMode.
		 *
		 *	\return
		 *		A scope guard object which will undo the actions of this
		 *		function when its lifetime ends.
		 */
		polygon_mode_guard polygon_mode (GLenum mode);
		
		
		class enable_guard {
			
			
			private:
			
			
				class state {
					
					
					private:
					
					
						GLenum cap_;
						bool e_;
						
						
					public:
					
					
						state () = delete;
						state (const state &) = delete;
						state (state &&) = default;
						state & operator = (const state &) = delete;
						state & operator = (state &&) = delete;
						
						
						state (GLenum);
						
						
						void restore ();
					
					
				};
				
				
				optional<state> s_;
				
				
			public:
			
			
				enable_guard () = delete;
				enable_guard (const enable_guard &) = delete;
				enable_guard & operator = (const enable_guard &) = delete;
				enable_guard & operator = (enable_guard &&) = delete;
				
				
				enable_guard (GLenum);
				enable_guard (enable_guard &&) noexcept;
				
				
				~enable_guard () noexcept;
			
			
		};
		
		
		/**
		 *	Enables a certain OpenGL capability.
		 *
		 *	\param [in] cap
		 *		An OpenGL enumeration value representing the
		 *		capability to enable.
		 *
		 *	\return
		 *		A scope guard which will undo the action of this
		 *		function when its lifetime ends.
		 */
		enable_guard enable (GLenum cap);
		/**
		 *	Disables a certain OpenGL capability.
		 *
		 *	\param [in] cap
		 *		An OpenGL enumeration value representing the
		 *		capability to disable.
		 *
		 *	\return
		 *		A scope guard which will undo the action of this
		 *		function when its lifetime ends.
		 */
		enable_guard disable (GLenum cap);
		
		
		class clear_color_guard {
			
			
			private:
			
			
				optional<std::array<GLfloat,4>> d_;
				
				
			public:
			
			
				clear_color_guard (const clear_color_guard &) = delete;
				clear_color_guard & operator = (const clear_color_guard &) = delete;
				clear_color_guard & operator = (clear_color_guard &&) = delete;
				
				
				clear_color_guard ();
				clear_color_guard (clear_color_guard &&) noexcept;
				
				
				~clear_color_guard () noexcept;
			
			
		};
		
		
		/**
		 *	Sets the clear color.
		 *
		 *	\param [in] red
		 *		The red component of the clear color.
		 *	\param [in] green
		 *		The green component of the clear color.
		 *	\param [in] blue
		 *		The blue component of the clear color.
		 *	\param [in] alpha
		 *		The alpha component of the clear color.
		 *
		 *	\return
		 *		A scope guard which shall undo the actions of
		 *		this function when its lifetime ends.
		 */
		clear_color_guard clear_color (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
		
		
		class viewport_guard {
			
			
			private:
			
			
				optional<std::array<GLuint,4>> d_;
				
				
			public:
			
			
				viewport_guard (const viewport_guard &) = delete;
				viewport_guard & operator = (const viewport_guard &) = delete;
				viewport_guard & operator = (viewport_guard &&) = delete;
				
				
				viewport_guard ();
				viewport_guard (viewport_guard &&) noexcept;
				
				
				~viewport_guard () noexcept;
			
			
		};
		
		
		/**
		 *	Calls glViewport.
		 *
		 *	\param [in] x
		 *		The \em x parameter to glViewport.
		 *	\param [in] y
		 *		The \em y parameter to glViewport.
		 *	\param [in] width
		 *		The \em width parameter to glViewport.
		 *	\param [in] height
		 *		The \em height parameter to glViewport.
		 *
		 *	\return
		 *		A scope guard which shall undo the actions of
		 *		this function when its lifetime ends.
		 */
		viewport_guard viewport (GLint x, GLint y, GLsizei width, GLsizei height);
		
		
	}
	
	
}
