#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include "glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "myglutils.hpp"
#include "myglobjects.hpp"
#include "myglobjects_points.hpp"
#include "logs.hpp"

GLuint getVertexShader() {
  const char* vertex_shader =
    "#version 410 core\n"
    "layout (location=0) in vec3 vp;"
    "layout (location=1) in int zorglub;"
    "uniform int clicked;"
    "void main() {"
    "  gl_Position = vec4( vp, 1.0 );"
    "  gl_Position.x += clicked * .3;"
    "  gl_Position.x += zorglub * .3 ;"
    "  "
    "}";

  GLuint vs = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( vs, 1, &vertex_shader, NULL );
  glCompileShader( vs );
  checkShaderError(vs);
  return vs;
}


GLuint getVertexShader2() {
  const char* vertex_shader =
    "#version 410 core\n"
    "in vec3 vp;"
    "void main() {"
    "  gl_Position = vec4( vp, 1.0 );"
    "  gl_Position.x += .5; "
    "  gl_Position.y += .5; "
    "}";

  GLuint vs = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( vs, 1, &vertex_shader, NULL );
  glCompileShader( vs );
  return vs;
}



GLuint getFragmentShader() {
  const char* fragment_shader =
    "#version 410 core\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4( 1.0, 0.0, 0.5, 1.0 );"
    "}";
 
  GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( fs, 1, &fragment_shader, NULL );
  glCompileShader( fs );
  return fs;
}

GLuint getFragmentShader2() {
  const char* fragment_shader =
    "#version 410 core\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4( 1.0, 1.0, 0.5, 1.0 );"
    "}";
 
  GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( fs, 1, &fragment_shader, NULL );
  glCompileShader( fs );
  return fs;
}





bool flag = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    std::cout << "Key: " << key << ", Action: " << action << std::endl;
    if (action == 1)
      flag = !flag;
}

void doSomeGL(GLFWwindow* window) {
  float points[] = {
    0.0f,  0.5f,  0.0f, // x,y,z of first point.
    0.5f, -0.5f,  0.0f, // x,y,z of second point.
    -0.5f, -0.5f,  0.0f  // x,y,z of third point.
  };

  
  GLint zog[] = {1, 4, 6};
  
  GLuint vbo = 0;
  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  glBufferData( GL_ARRAY_BUFFER, 9 * sizeof( float ), points, GL_STATIC_DRAW );

  GLuint vbzo = 0;
  glGenBuffers( 1, &vbzo );
  glBindBuffer( GL_ARRAY_BUFFER, vbzo );
  glBufferData( GL_ARRAY_BUFFER, 3 * sizeof( int ), zog, GL_STATIC_DRAW );


  
  
  GLuint vao = 0;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glEnableVertexAttribArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );

  //bind zorglub
  glEnableVertexAttribArray( 1 );
  glBindBuffer( GL_ARRAY_BUFFER, vbzo );
  glVertexAttribIPointer( 1, 1, GL_INT, 0, NULL );


  

  auto vs = getVertexShader();
  auto vs2 = getVertexShader2();
  auto fs = getFragmentShader();
  auto fs2 = getFragmentShader2();
  
  GLuint shader_program = glCreateProgram();
  glAttachShader( shader_program, fs );
  glAttachShader( shader_program, vs );
  glLinkProgram( shader_program );
  int clicked_loc = glGetUniformLocation( shader_program, "clicked" );

  
  GLuint shader_program2 = glCreateProgram();
  glAttachShader( shader_program2, fs2 );
  glAttachShader( shader_program2, vs2 );
  glLinkProgram( shader_program2 );


  float da_points[] = {-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.f, 0.f};

  GL2DPointsBasic mypts (da_points, sizeof(da_points)/2/sizeof(da_points[0]));

  //float da_1dpoints[] = {.0f, .025f, .05f, .1f, .15f, .2f, .25f, .3f};
  
  //GL1DPointsBasic mypts1d (da_1dpoints, sizeof(da_1dpoints)/sizeof(da_1dpoints[0]));

  
  
  glfwSetKeyCallback(window, key_callback); // Register the key callback

  std::cout<<"GO!\n";

  loglibrary::log l = loglibrary::log::parse_log("../log-1000");
  l.stats();
  l.shift_offset();
  l.stats();
  std::vector<float> data;
  for (const auto& a : l.entries) {
    data.push_back((float)((size_t)a.ptr));
  }

  {
    auto m = data[0];
    for (auto& a: data) {
      if (a > m)
	m = a;
    }
    std::cout<<"max "<<m<<'\n';
    for (auto& a: data) {
      a /= m;
      a *= .98;
      a += .01;
      //      std::cout<<a<<"\n";
    }
  }
  
  GL1DPointsBasic log (&(data[0]), data.size());
  log.setPointSize(1.);
  log.setColor(1., 1., 1., 0.2);

  {
    glm::mat4 Model;
    Model = glm::translate(glm::mat4(1.0), glm::vec3(-.5, -.5, 0.));
    Model = glm::scale(glm::mat4(1.0), glm::vec3(2., 2., 1.)) * Model;
    log.setTransform(Model);
  }

  
  while ( !glfwWindowShouldClose( window ) ) {
    // Update window events.
    glfwPollEvents();
    
    // Wipe the drawing surface clear.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Put the shader program, and the VAO, in focus in OpenGL's state machine.
    //glUseProgram( shader_program );
    //glUniform1i( clicked_loc, (flag?1:0));
    //glBindVertexArray( vao );
    
    // Draw points 0-3 from the currently bound VAO with current in-use shader.
    //    glDrawArrays( GL_TRIANGLES, 0, 3 );

    //glCheckError();
 
    //draw again with other shader
    //glUseProgram( shader_program2 );
    //glDrawArrays( GL_TRIANGLES, 0, 3 );
 

    //points test

    mypts.render();
    //if (flag)
    //  mypts1d.render();
    log.render();
 
    
    // Put the stuff we've been drawing onto the visible area.
    glfwSwapBuffers( window );
  }
}




int foo()
{
        glm::vec4 Position = glm::vec4(glm::vec3(0.0), 1.0);
	glm::mat4 Model = glm::mat4(1.0);
	Model = glm::translate(glm::mat4(1.0), glm::vec3(-.5, -.5, 0.));
	glm::vec4 Transformed = Model * Position;
	Position=Transformed;
	std::cout<<Position[0]<<" "<<Position[1]<<" "<<Position[2]<<" "<<Position[3]<<"\n";
        return 0;
}

int main( void ) {

  //  std::cout<<foo();
  
  // Start OpenGL context and OS window using the GLFW helper library.
  if ( !glfwInit() ) {
    fprintf( stderr, "ERROR: could not start GLFW3.\n" );
    return 1;
  }

  // Request an OpenGL 4.6, core, context from GLFW.
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
  glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

  // Create a window on the operating system, then tie the OpenGL context to it.
  GLFWwindow* window = glfwCreateWindow( 800, 600, "Log Viz", NULL, NULL );
  if ( !window ) {
    fprintf( stderr, "ERROR: Could not open window with GLFW3.\n" );
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent( window );
                                  
  // Start Glad, so we can call OpenGL functions.
  //int version_glad = gladLoadGL( glfwGetProcAddress );
  int version_glad = gladLoadGL( );
  if ( version_glad == 0 ) {
    fprintf( stderr, "ERROR: Failed to initialize OpenGL context.\n" );
    return 1;
  }
  //  printf( "Loaded OpenGL %i.%i\n", GLAD_VERSION_MAJOR( version_glad ), GLAD_VERSION_MINOR( version_glad ) );

  // Try to call some OpenGL functions, and print some more version info.
  printf( "Renderer: %s.\n", glGetString( GL_RENDERER ) );
  printf( "OpenGL version supported %s.\n", glGetString( GL_VERSION ) );

  /* OTHER STUFF GOES HERE NEXT */
  doSomeGL(window);
  
  // Close OpenGL window, context, and any other GLFW resources.
  glfwTerminate();
  return 0;
}
