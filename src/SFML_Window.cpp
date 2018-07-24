#include "SFML_Window.hpp"

SFML_Window::SFML_Window( int width, int height,
                          const char* label):   m_width(width),
                                                m_height(height),
                                                m_label(label),
                                                m_shouldClose(false),
						m_wasResized(false)
{
}


//Generic window functions
void SFML_Window::setClearColour(float clearColour[3])
{
  for(int i = 0; i < 3; i++)
  {
    m_clearColour[i] = clearColour[i];
  }
}



void SFML_Window::resize(int width, int height)
{
  m_width = width;
  m_height = height;
  updateWindowViewport();
  m_wasResized = true;
}



void SFML_Window::setToClose()
{
  m_shouldClose = true;
}



int SFML_Window::getWidth()
{
  return m_width;
}



int SFML_Window::getHeight()
{
  return m_height;
}



bool SFML_Window::shouldClose()
{
  return m_shouldClose;
}



bool SFML_Window::wasResized()
{
	bool ret = m_wasResized;
	if(m_wasResized) m_wasResized = false;
	return ret;
}

//SFML specific functions
void SFML_Window::init(byte* buffer)
{
  //Create sf::Window
  sf::ContextSettings context = createContextSettings();
  m_windowInstance = new (buffer) sf::Window(sf::VideoMode(m_width, m_height),
                                m_label, sf::Style::Default, context);

  //Set sf::Window attributes
  m_windowInstance->setVerticalSyncEnabled(true);
  m_windowInstance->setActive(true);

  //Update viewport
  updateWindowViewport();
}



sf::ContextSettings SFML_Window::createContextSettings()
{
  uint depth = 16; //Depth buffer size
  uint stencil = 6; //Stencil buffer size
  uint antialiasing = 0;
  uint major = 4;
  uint minor = 1;
  uint attributes = sf::ContextSettings::Attribute::Core;
  return sf::ContextSettings(depth, stencil, antialiasing, major, minor,
                              attributes);
}



void SFML_Window::update()
{
  m_windowInstance->display();
}



void SFML_Window::clear()
{
  glClearColor(m_clearColour[0], m_clearColour[1], m_clearColour[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



bool SFML_Window::pollEvent(sf::Event& event)
{
  return m_windowInstance->pollEvent(event);
}



void SFML_Window::updateWindowViewport()
{
  glViewport(0, 0, m_width, m_height);
}

sf::Window* SFML_Window::getWindowReference()
{
  return this->m_windowInstance;
}
