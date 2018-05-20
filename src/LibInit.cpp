#include "LibInit.hpp"

namespace LibInit
{
  bool initGLEW()
  {
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if(error != GLEW_OK)
    {
      std::cout << "Failed to initialise GLEW with error: " << error;
      std::cout << " (This error can be ignored)" << std::endl;
      return false;
    }
    glGetError();
    return true;
  }
}
