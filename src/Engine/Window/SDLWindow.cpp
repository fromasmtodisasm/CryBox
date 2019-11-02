#include <BlackBox/SDLWindow.hpp>
#include <BlackBox/Render/BaseTexture.hpp>

#include <SDL2/SDL.h>

CSDLWindow::CSDLWindow(std::string, int width, int height)
{

}

CSDLWindow::~CSDLWindow()
{
}

bool CSDLWindow::init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen)
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	if (!Create(width, height, fullscreen))
	{
		return false;
	}
  //m_Window->setVerticalSyncEnabled(true);
  //m_Window->setFramerateLimit(60);
  //m_Window->setMouseCursorGrabbed(true);

#ifdef GUI
  ImGui::SFML::Init(*m_Window);
#endif // GUI

  // Make it the active window for OpenGL calls
  //m_Window->setActive();

  // Input handling specific
  //Mouse.curr_pos = Mouse.curr_pos = sf::Mouse::getPosition(*m_Window);
  Mouse.x_wraped = Mouse.y_wraped = false;
  Mouse.locked = false;

	//auto ip = GetISystem()->GetIConsole()->GetCVar("image_path");
	//if (ip)
	//{
		//std::string icon(ip->GetString());
		Image img;
		if (img.load("res/images/icon.jpg", nullptr))
		{
			;// m_Window->setIcon(img.width, img.height, static_cast<sf::Uint8*>(img.data));
		}
	//}

  return true;

}

void CSDLWindow::update()
{
}

void CSDLWindow::clear()
{
}

bool CSDLWindow::closed()
{
	return false;
}

void CSDLWindow::swap()
{
	SDL_GL_SwapWindow(m_Window);
}

void CSDLWindow::setTitle(const char*)
{
}

void CSDLWindow::show()
{
}

void* CSDLWindow::getHandle()
{
	return m_Window;
}

int CSDLWindow::getWidth()
{
	int w = 0;
	SDL_GetWindowSize(m_Window, &w, nullptr);
	return w;
}

int CSDLWindow::getHeight()
{
	int h = 0;
	SDL_GetWindowSize(m_Window, nullptr, &h);
	return h;
}

void CSDLWindow::setFlags(int flags)
{
}

bool CSDLWindow::Create(int width, int height, bool fullscreen)
{
	/* Request opengl 3.2 context.
		 * SDL doesn't have the ability to choose which profile at this time of writing,
		 * but it should default to the core profile */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	/* Turn on double buffering with a 24bit Z buffer.
	 * You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create window
	m_Window = SDL_CreateWindow(m_Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (m_Window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(m_Window);

	return true;
}

Vec2 CSDLWindow::nextMousePos(Vec2& position)
{
	return Vec2();
}

void CSDLWindow::setMouseWrap(bool wrap)
{
}

void CSDLWindow::glInit()
{
}

Rect& CSDLWindow::getViewPort()
{
	return viewPort;
}

bool CSDLWindow::create(Params params)
{
	return false;
}

void CSDLWindow::changeSize(int w, int h)
{
}

void CSDLWindow::setCursor(Cursor* cursor)
{
}

extern "C" {
	inline IWindow* CreateIWindow()
	{
		return new CSDLWindow();
	}
}
