#ifndef MYGLOBJECTPOINTS
#define MYGLOBJECTPOINTS


class GL2DPointsBasic : public GLObjectBase {

  GLuint getVertexShader3() {
    const char* vertex_shader =
      "#version 410 core\n"
      "in vec3 vp;"
      "void main() {"
      "  gl_Position = vec4( vp, 1.0 );"
      "}";
    
    GLuint vs = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vs, 1, &vertex_shader, NULL );
    glCompileShader( vs );
    return vs;
  }
  
  GLuint getFragmentShader3() {
    const char* fragment_shader =
      "#version 410 core\n"
      "out vec4 frag_colour;"
      "void main() {"
      "  frag_colour = vec4( 0.3, 0.3, 1.0, 1.0 );"
      "}";
    
    GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fs, 1, &fragment_shader, NULL );
    glCompileShader( fs );
    return fs;
  }

  
  GLuint shader_program3;
  GLuint vao_points;
  GLuint nbpoints;
public:
  GL2DPointsBasic(float* data, size_t nbpoints) {
    this->nbpoints = nbpoints;
    GLuint vbo_points = 0;
    glGenBuffers( 1, &vbo_points );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_points );
    glBufferData( GL_ARRAY_BUFFER, 2* nbpoints * sizeof( float ), data, GL_STATIC_DRAW );
    
    vao_points = 0;
    glGenVertexArrays( 1, &vao_points );
    glBindVertexArray( vao_points );
    
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_points );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );

    auto vs3 = getVertexShader3();
    auto fs3 = getFragmentShader3();

    this->shader_program3 = glCreateProgram();
    glAttachShader( shader_program3, fs3 );
    glAttachShader( shader_program3, vs3 );
    glLinkProgram( shader_program3 );

  
  }


  virtual void render() {
    //draw again with other shader
    glUseProgram( shader_program3 );
    glBindVertexArray( vao_points );
	
    glPointSize(10.);
    glDrawArrays( GL_POINTS, 0,  nbpoints);

  }
  
  virtual ~GL2DPointsBasic() {
    //this gotta leak like crazy
  }
  
};


//The points get mapped to [0:1], val
class GL1DPointsBasic : public GLObjectBase {
  //maps Y axis to the parameter passed.
  //maps X axis to the index

  GLuint getVertexShader() {
    const char* vertex_shader =
      "#version 410 core\n"
      "in float val;"
      "uniform float total;"
      "uniform mat4 model_transform;"
      "uniform mat4 camera_transform;"
      "void main() {"
      "  vec4 base_position = vec4( float(gl_VertexID)/float(total) , val , 0. , 1.0 );"
      "  gl_Position =  camera_transform * model_transform * base_position;"
      "}";
    
    GLuint vs = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vs, 1, &vertex_shader, NULL );
    glCompileShader( vs );
    checkShaderError(vs);
    return vs;
  }
  
  GLuint getFragmentShader() {
    const char* fragment_shader =
      "#version 410 core\n"
      "out vec4 frag_colour;"
      "uniform vec4 color;"
      "void main() {"
      "  frag_colour = color;"
      "}";
    
    GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fs, 1, &fragment_shader, NULL );
    glCompileShader( fs );
    checkShaderError(fs);
    return fs;
  }

  
  GLuint shader_program;
  GLuint vao_points;

  GLuint nbpoints;
  GLint nbpoints_loc;
  GLint color_loc;
  GLint model_transform_loc;
  GLint camera_transform_loc;
  GLfloat pointSize;
  float color[4];

public:

  void setData(float* data, size_t nbpoints) {
    //TODO: clear out previous if necessary

    this->nbpoints = nbpoints;
    GLuint vbo_points = 0;
    glGenBuffers( 1, &vbo_points );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_points );
    glBufferData( GL_ARRAY_BUFFER,  nbpoints * sizeof( float ), data, GL_STATIC_DRAW );
    
    vao_points = 0;
    glGenVertexArrays( 1, &vao_points );
    glBindVertexArray( vao_points );
    
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_points );
    glVertexAttribPointer( 0, 1, GL_FLOAT, GL_FALSE, 0, NULL );

    glUseProgram( shader_program );
    glUniform1f( nbpoints_loc, nbpoints); //you need the program to be "used" to write its variables
    glCheckError();


  }

  GL1DPointsBasic(float* data, size_t nbpoints)
    :nbpoints(0), pointSize(1.), color(1.0, 1.0, 1.0, 1.0)
  {
    auto vs = getVertexShader();
    auto fs = getFragmentShader();
    
    this->shader_program = glCreateProgram();
    glAttachShader( shader_program, fs );
    glAttachShader( shader_program, vs );
    glLinkProgram( shader_program );

    nbpoints_loc = glGetUniformLocation( shader_program, "total" );
    if (nbpoints_loc == GL_INVALID_VALUE) {
      std::cerr<<"GL_INVALID_VALUE"<<'\n';
    }
    if (nbpoints_loc == GL_INVALID_OPERATION) {
      std::cerr<<"GL_INVALID_OPERATION"<<'\n';
    }

    color_loc = glGetUniformLocation( shader_program, "color" );
    if (color_loc == GL_INVALID_VALUE) {
      std::cerr<<"GL_INVALID_VALUE"<<'\n';
    }
    if (color_loc == GL_INVALID_OPERATION) {
      std::cerr<<"GL_INVALID_OPERATION"<<'\n';
    }


    model_transform_loc = glGetUniformLocation( shader_program, "model_transform" );
    if (model_transform_loc == -1) {
      std::cerr<<"can't find location"<<'\n';
    }
    if (model_transform_loc == GL_INVALID_VALUE) {
      std::cerr<<"GL_INVALID_VALUE"<<'\n';
    }
    if (model_transform_loc == GL_INVALID_OPERATION) {
      std::cerr<<"GL_INVALID_OPERATION"<<'\n';
    }

    camera_transform_loc = glGetUniformLocation( shader_program, "camera_transform" );
    if (camera_transform_loc == -1) {
      std::cerr<<"can't find location"<<'\n';
    }
    if (camera_transform_loc == GL_INVALID_VALUE) {
      std::cerr<<"GL_INVALID_VALUE"<<'\n';
    }
    if (camera_transform_loc == GL_INVALID_OPERATION) {
      std::cerr<<"GL_INVALID_OPERATION"<<'\n';
    }
    
    setColor(1.0, 1.0, 1.0, 1.0);
    if (data != nullptr)
      setData(data, nbpoints);
  }

  void setPointSize(GLfloat pointSize) {
    this->pointSize = pointSize;
    
  }

  void setTransform(const glm::mat4& tmat) {
    glUseProgram( shader_program );
      
    glUniformMatrix4fv(model_transform_loc, 1, GL_FALSE, &tmat[0][0]);
  }

  void setCamera(const glm::mat4& tmat) {
    glUseProgram( shader_program );
      
    glUniformMatrix4fv(camera_transform_loc, 1, GL_FALSE, &tmat[0][0]);
  }

  
  void setColor(float r, float g, float b, float a) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
    glUseProgram( shader_program);
    glUniform4f(color_loc, color[0], color[1], color[2], color[3]);
  }

  virtual void render() {
    //draw again with other shader
    glUseProgram( shader_program );
    glBindVertexArray( vao_points );
	
    glPointSize(pointSize);
    glDrawArrays( GL_POINTS, 0, nbpoints );
    glCheckError();
    //std::cout<<"drawn: "<<nbpoints<<" points"<<'\n';
  }
  
  virtual ~GL1DPointsBasic() {
    //this gotta leak like crazy
  }
  
};

#endif
