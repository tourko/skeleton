#include <Application.hpp>

int main(int argc, char** argv)
{
  Application& app = Application::init(argc, argv);
  app.run();

  return 0;
}
