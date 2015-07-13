#include <cstdlib>          // Required for atexit()
#include <common.hpp>
#include <Application.hpp>

Application* Application::appInstance = 0;

Application& Application::instance()
{
	if (appInstance == 0) {
		appInstance = new Application();
	}

	return *appInstance;
}

Application::Application()
{
  DBG1("Creating new application instance.");
	atexit(&Application::cleanup);
}

Application::~Application()
{
    /* do nothing */
    DBG1("Deleting application instance.");
}

void Application::cleanup()
{
  DBG1("Cleaning up application.");

  if (appInstance != 0) {
    delete appInstance;
    appInstance = 0;
  }
}

Application& Application::init(int UNUSED(argc), char** UNUSED(argv))
{
  DBG1("Initializing application.");

  /* TODO: Parse arguments*/

  return Application::instance();
}

void Application::run()
{
  DBG1("Running application.");
}
