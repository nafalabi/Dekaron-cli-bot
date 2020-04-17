#pragma once

#include "clientmanager.h";
#include "processlist.h";
#include "console.h";

class input
{
private:
	clientmanager* clientManager;
	processlist* procList;
public:
	input(clientmanager*, processlist*);
	void start();
	void consoleInputThread();
	void keystrokesThread();

	bool isKeyPressed(std::vector<int>);
};

