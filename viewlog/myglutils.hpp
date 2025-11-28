
void glCheckError() {
    // Check for errors immediately after the draw call
    GLenum error = glGetError(); 
    
    if (error != GL_NO_ERROR) {
      // An error occurred!
      std::cerr << "OpenGL Error: " << error << std::endl;
      // You can convert the error code (like 1282) to its symbolic name (GL_INVALID_OPERATION)
      // using a lookup table or debugging utility.
    }
}



int  checkShaderError(GLuint vs) { 
  // After glCompileShader check for errors.
  int params = -1;
  glGetShaderiv( vs, GL_COMPILE_STATUS, &params );
  
  // On error, capture the log and print it.
  if ( GL_TRUE != params ) {
    int max_length = 2048;
    int actual_length = 0;
    char slog[2048];
    glGetShaderInfoLog( vs, max_length, &actual_length, slog );
    fprintf( stderr, "ERROR: Shader index %u did not compile.\n%s\n", vs, slog );
    return 1;
  }
  return 0;
}
