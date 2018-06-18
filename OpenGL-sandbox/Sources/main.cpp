#include "engine.hpp"
#include <windows.h>


int main()
{
	FreeConsole();
	return Engine::instance().run();
}
