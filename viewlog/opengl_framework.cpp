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
#include "callbacks.hpp"

class Camera {
  glm::mat4 camera_matrix;
public:
  Camera()
    :camera_matrix(glm::mat4(1.0))
  {}

  const glm::mat4& getMatrix() const
  {return camera_matrix;}

  void reset() {
    camera_matrix = glm::mat4(1.0);
  }

  void wow() {
    camera_matrix = glm::mat4(0.5f, 0., 0., 0.,
			      0., 0.5f, 0., 0.,
			      0., 0., 1.f, 0.,
			      0., 0., 0., 1.f);
  }

  void translate(const glm::vec3& vec) {
    camera_matrix = camera_matrix * glm::translate(glm::mat4(1.0), vec);
  }
  void zoom(const float& zoomlevel) {
    camera_matrix = camera_matrix * glm::scale(glm::mat4(1.0), glm::vec3(zoomlevel, zoomlevel, 1.));
  }
};




//transform screen coordinate to viewport normalized [-1;1]
std::tuple<float, float> screenPosToViewportNormalized(GLFWwindow* window, int x, int y) {
  //screen coordinates are rooted top left
  int window_width, window_height;
  glfwGetWindowSize(window, &window_width, &window_height);
  
  GLint viewport[4]; //the viewport is rooted bottom-left
  glGetIntegerv(GL_VIEWPORT, viewport);

  //in bottom left coordinate
  int x1 = x;
  int y1 = window_height-y;

  //in viewport pixel coordinate
  int x2 = x1 - viewport[0];
  int y2 = y1 - viewport[1];

  //in viewport relative in [0;1]
  float x3 = (float)x2/viewport[2];
  float y3 = (float)y2/viewport[3] ;

  //convert to [-1;1]
  return {2*x3-1, 2*y3-1};
}

void doSomeGL(GLFWwindow* window) {
  float da_points[] = {-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.f, 0.f};

  GL2DPointsBasic viewport (da_points, sizeof(da_points)/2/sizeof(da_points[0]));

  
  glfwSetKeyCallback(window, key_callback); // Register the key callback

  glfwSetCursorPosCallback(window, mouse_pos_callback);

  glfwSetMouseButtonCallback(window, mouse_button_callback);

  auto  poscall = register_mouse_pos_callback([](GLFWwindow* window, int xpos, int ypos) {
    auto [nx, ny] = screenPosToViewportNormalized(window, xpos, ypos);

    std::cout<<xpos<<" "<<ypos<<" "<<nx<<" "<<ny<<'\n';
  });
  
  register_key_callback ([](GLFWwindow* window, int key, int scancode, int action, int mods) -> void{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  });

  register_mouse_button_callback([&](GLFWwindow*, int, int action, int)->void {std::cerr<<"click"<<'\n';
      if (action == GLFW_PRESS)
	unregister_mouse_pos_callback(poscall);
  });
  
  Camera cam;

  register_key_callback ([&cam](GLFWwindow* window, int key, int scancode, int action, int mods) -> void{
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
      cam.wow();
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
      cam.reset();
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
      cam.translate(glm::vec3(+0.1, 0., 0.));
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
      cam.translate(glm::vec3(-0.1, 0., 0.));
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
      cam.translate(glm::vec3(0., 0.1, 0.));
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
      cam.translate(glm::vec3(0., -0.1, 0.));
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
      cam.zoom(1.1);
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
      cam.zoom(1/1.1);
    }
  });
  
  
  std::cout<<"GO!\n";

  loglibrary::log l = loglibrary::log::parse_log("../log-1000");
  l.stats();
  l.shift_offset();
  l.stats();
  std::vector<float> data; //data will be a [0;1] encoding of the log
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

  //Full screening the plot
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

    log.setCamera(cam.getMatrix());
    
    viewport.render();
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
  GLFWwindow* window = glfwCreateWindow( 800,600, "Log Viz", NULL, NULL );
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
