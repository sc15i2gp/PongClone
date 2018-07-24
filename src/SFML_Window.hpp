#pragma once
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <new>
#include "Types.hpp"
#include "LibInit.hpp"

class SFML_Window
{
public:
  SFML_Window(  int width             = 800,
                int height            = 600,
                const char* label     = "App Window");

  //Generic window functions
  //Setters
  void setClearColour(float clearColour[3]);
  void resize(int width, int height);
  void setToClose();

  //Getters
  int getWidth();
  int getHeight();
  bool shouldClose();
  bool wasResized();

  //SFML specific window functions
  void update(); //Calls sf::Window display()
  void clear(); //glClear colour bit
  bool pollEvent(sf::Event& event);
  void init(byte* buffer);
  sf::Window* getWindowReference();


private:
  void updateWindowViewport();
  sf::ContextSettings createContextSettings();

  bool        	m_shouldClose;
  bool		m_wasResized;
  int         	m_width;
  int         	m_height;
  float       	m_clearColour[3];
  const char* 	m_label;

  //SFML specific window details
  sf::Window* m_windowInstance;
};
