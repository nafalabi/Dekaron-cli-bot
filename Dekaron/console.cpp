#include "console.h"

void console::newCommand()
{
	std::cout << "Dekaron > ";
}

void console::newLine()
{
	std::cout << std::endl;
}

void console::print(std::string message)
{
	std::cout << message;
}

void console::printl(std::string message)
{
	std::cout << message;
	newLine(); 
	newLine();
	newCommand();
}

void console::clear()
{
	std::cout << "\033[2J\033[1;1H";
}

void console::question(std::string question)
{
	std::cout << question << ": ";
}
