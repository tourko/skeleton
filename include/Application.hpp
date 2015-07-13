#pragma once

/*
 * Only once instance of Application class can exist per process.
 * Hence the class is implemented as singleton.
 * Class is meant to be instanciated and used in the main thread only
 * and thus it is implemented without thread safity in mind.
 */
class Application {
private:
	Application();
	virtual ~Application();

	// not copyable
	Application(Application const&);
	Application& operator=(Application const&);

	static Application* appInstance;
	static Application& instance();
	static void cleanup();

public:
  static Application& init(int argc, char** argv);
	void run();
};
