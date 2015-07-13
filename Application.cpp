#include <cstdlib>          // Required for atexit()
#include <csignal>          // Required for signal handling
#include <cstring>          // Required for strsignal()
#include <thread>           // Required for threading support
#include <chrono>           // Required for sleeping
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
  DBG1("Cleaning up the application.");

  if (appInstance != 0) {
    delete appInstance;
    appInstance = 0;
  }
}

Application& Application::init(int UNUSED(argc), char** UNUSED(argv))
{
  DBG1("Initializing application.");

  /* TODO: Parse arguments*/

  // Register signals handler
  DBG1("Registering signals handler.");
  signal(SIGINT,  catch_signal);  //<-- Catch keyboard interrupt (e.g. CTRL-C)
  signal(SIGTRAP, catch_signal);  //<-- Catch SIGTRAP but ingore them. Used to trap to GDB if gdb is attached
  signal(SIGTERM, catch_signal);  //<-- Catch Termination Signal (e.g. kill <pid>)
  signal(SIGTSTP, catch_signal);  //<-- Catch suspends a process (e.g. CTRL-Z)

  return Application::instance();
}

void Application::run()
{
  this->isRunning = true;

  while (this->isRunning) {
    DBG1("Sleeping...");
    std::this_thread::sleep_for(std::chrono::seconds(30));
  }
}

void Application::start()
{
  if (unlikely(this->isRunning)) {
    WRN("Cannot start an already running application.");
    return;
  }

  INF("Starting the application.");
  this->run();
}

void Application::stop()
{
  if (unlikely(!this->isRunning)) {
    WRN("Cannot stop a non-running application.");
    return;
  }

  INF("Stopping the application.");
  this->isRunning = false;
}

// Unix signal handler
void Application::catch_signal(int sig) {
  switch (sig) {
    case SIGTRAP: // <-- Ignore it. GDB will catch this, but we do not need it for normal runtime (it's thrown by die_assert() while staying in context)
    case SIGTSTP: // <-- Ignore it.
      DBG1("Caught signal %s. Ignoring the signal.", strsignal(sig));
      return;

    case SIGTERM: // <-- Catch Termination Signal (e.g. kill <pid>) and quite immediatelly
      DBG1("Caught signal %s. Terminating application immediatelly.", strsignal(sig));
      exit(EXIT_FAILURE);

    case SIGINT: // <-- Graceful shutdown of application on CTRL-C or 'kill -SIGINT <pid>'
      DBG1("Caught signal %s. Initiating graceful application shutdown.", strsignal(sig));
      Application::instance().stop();
      break;
      
    default:
      INF("Caught unhandled signal %s. Ignoring the signal.", strsignal(sig));
      break;
  }
}
