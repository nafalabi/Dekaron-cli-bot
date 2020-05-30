#include "input.h"

input::input(clientmanager* cm, processlist* pl)
{
	clientManager = cm;
	procList = pl;
}

void input::start()
{
	std::thread consoleInput(&input::consoleInputThread, this);
	std::thread keystrokes(&input::keystrokesThread, this);
	consoleInput.join();
	keystrokes.join();
}


//=============================================
//             Thread Functions
//=============================================

void input::consoleInputThread()
{
	std::string input;

	while (input != "exit") {
		std::cout << "Dekaron > ";
		std::getline(std::cin, input);
		std::stringstream ss(input);
		std::string firstInput;
		ss >> firstInput;

		if (firstInput == "exit") {
			clientManager->stopProcess();
			std::cout << "Bye!";
			break;
		}
		else if (firstInput == "") {
			console::print("\r");
			continue;
		}
		else if (firstInput == "clear") {
			console::clear();
		}
		else if (firstInput == "allprocs") {
			procList->printProcesses();
		}
		else if (firstInput == "procs") {
			procList->printDekaronProcs();
		}
		else if (firstInput == "add" || firstInput == "addclient") {
			char secondInput[10];
			ss >> secondInput;
				
			if (strcmp(secondInput,"all") == 0) {
				procList->hydrateList();
				clientManager->addClients(&procList->dekaronPIDs);
			}
			else {
				int PID = atoi(secondInput);
				clientManager->addClient(PID);
			}
		}
		else if (firstInput == "remove" || firstInput == "removeclient") {
			char secondInput[10];
			ss >> secondInput;

			if (strcmp(secondInput, "all") == 0) {
				clientManager->removeClients();
			}
			else {
				int clientNum = atoi(secondInput);
				clientManager->removeClient(clientNum);
			}
		}
		else if (firstInput == "list" || firstInput == "listclient") {
			clientManager->printClients();
		}
		else if (firstInput == "test") {
			clientManager->testFunction();
		}
		else if (firstInput == "set") {
			std::string setName;
			int clientNum; // could be PID or index client
			ss >> setName;
			ss >> clientNum;
			clientManager->set(setName, clientNum);
		}
		else if (firstInput == "details") {
			char secondInput[5];
			ss >> secondInput;

			if (strcmp(secondInput, "all") == 0) {
				clientManager->showAllClientDetails();
			}
			else {
				int clientNum = atoi(secondInput);
				clientManager->showClientDetails(clientNum);
			}
		}
		else {
			std::cout << "Command not found\n";
		}

		std::cout << "\n";
	}
}

void input::keystrokesThread()
{
	while (clientManager->getProcessState()) {
		// Auto Follow Toggler
		if (isKeyPressed(clientManager->settings.autoFollowKey)) {
			clientManager->toggleAutoFollow();
			Sleep(50);
		}
		// Auto Attack Toggler
		else if (isKeyPressed(clientManager->settings.autoAttackKey)) {
			clientManager->toggleAutoAttack();
			Sleep(50);
		}
		Sleep(50);
	}
}



//=============================================
//             Helper functions
//=============================================

bool input::isKeyPressed(std::vector<int> keys)
{
	for (size_t i = 0; i < keys.size(); i++) {
		// the "& 0x8000" is bitwise operation to filter the value from false positive result
		if (!(GetAsyncKeyState(keys[i]) & 0x8000)) return false;
	}
	return true;
}