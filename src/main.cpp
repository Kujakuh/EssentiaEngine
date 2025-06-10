#include <EssentiaEngine>

#include "Testing/generalTesting.cpp"
#include "GameDemo/2dGame.cpp"

#ifndef LIBRARY_EXPORTS
#	define DllExport __declspec(dllexport)
#	define DllImport __declspec(dllimport)
#endif

int main(void)
{
	if (!glfwInit()) return -1;

	//generalTesting();
	twoDGame();

	glfwTerminate();
	return 0;
}

