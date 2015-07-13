#pragma once

/*
 * Only once instance of Application class can exist per process.
 * Hence the class is implemented as singleton.
 */
class Application {
private:
	Application();
	virtual ~Application();

	// not copyable
	Application(Application const&);
	Application& operator=(Application const&);

	static Application* appInstance;
	bool isRunning = false;

	static Application& instance();
	static void cleanup();
	static void catch_signal(int sig);

public:
  static Application& init(int argc, char** argv);
	void run();
	void start();
	void stop();
};
