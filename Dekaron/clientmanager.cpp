#include "clientmanager.h"

// Constructor
clientmanager::clientmanager()
{
	clientTotal = 0;
}

//=================================================
//				Helper functions
//=================================================

bool clientmanager::checkClientExist(DWORD PID)
{
	for (int i = 0; i < clientTotal; i++)
	{
		if (clients[i].getPID() == PID)
		{
			return true;
		}
	}
	return false;
}

void clientmanager::addClient(DWORD PID)
{
	std::stringstream ss;

	if (checkClientExist(PID))
	{
		ss << "PID " << PID << " is already added.\n";
		console::print(ss.str());
		return;
	}

	// Initiate a client
	clients[clientTotal] = gameclient(PID);
	// if the client is invalid return
	if (!clients[clientTotal].isClientValid())
	{
		ss << "The client for PID " << PID << " is invalid, make sure you have logged in to your character.\n";
		console::print(ss.str());
		return;
	}

	clientTotal += 1;
	ss << "PID " << PID << " has been added.\n";
	console::print(ss.str());
}

void clientmanager::addClients(std::vector<int> *PIDs)
{
	for (int i = 0; i < PIDs->size(); i++)
	{
		addClient(PIDs->at(i));
	}
}

void clientmanager::removeClient(int clientNum)
{
	int index = findClientIndex(clientNum);
	int clientListSize = sizeof(clients) / sizeof(gameclient);
	if (index == -1)
		return;
	if (index >= clientListSize)
		return;
	int PID = clients[index].getPID();

	// Delete the client from the list
	for (int i = index; i < clientListSize - 1; i++)
		clients[i] = clients[i + 1];
	// shrink the total number of clients
	clientTotal -= 1;

	std::stringstream ss;
	ss << "Client PID " << PID << " has been removed" << index << "\n";
	console::print(ss.str());
}

void clientmanager::removeClients()
{
	clientTotal = 0;

	std::stringstream ss;
	ss << "All the clients have been removed\n";
	console::print(ss.str());
}

int clientmanager::findClientIndex(int clientNum)
{
	for (int i = 0; i < clientTotal; i++)
	{
		if (clients[i].getPID() == clientNum)
			return i;
	}

	if (clientNum <= 0 || clientNum > clientTotal)
		return -1;

	return clientNum - 1;
}

void clientmanager::printClients()
{
	std::stringstream message;

	for (int i = 0; i < clientTotal; i++)
	{
		clients[i].hydrateValues();
		message << "\t" << (i + 1) << ". " << (i == clientLeaderIndex ? "*" : "");
		message << clients[i].charName;
		message << " PID " << clients[i].getPID() << "\n";
	}

	console::print(message.str());
}

void clientmanager::callClientsMethod(void (gameclient::*funcPtr)())
{
	void (gameclient::*function)();
	function = funcPtr;
	for (int i = 0; i < clientTotal; i++)
	{
		(clients[i].*function)();
	}
}

void clientmanager::callClientsMethodAsync(void (gameclient::*funcPtr)())
{
	void (gameclient::*func)();
	func = funcPtr;

	for (int i = 0; i < clientTotal; i++)
	{
		// auto dump = std::async(function, clients[i]);
		std::thread attack(func, clients[i]);
		attack.detach();
		Sleep(1);
	}
	Sleep(clients[0].settings.skillKeys.size() * 10);
}

//=================================================
//			   Process functions
//=================================================

bool clientmanager::getProcessState()
{
	return processIsActive;
}

void clientmanager::mainProcess()
{
	while (processIsActive)
	{
		callClientsMethod(&gameclient::manaAutoPot);
		callClientsMethod(&gameclient::healthAutoPot);

		if (autoAttackIsActive)
		{
			callClientsMethodAsync(&gameclient::doAutoAttack);
		}
		Sleep(1);
	}
}

void clientmanager::stopProcess()
{
	processIsActive = false;
}

//=================================================
//				Action functions
//=================================================

void clientmanager::toggleAutoFollow()
{
	// Turn off auto attack
	autoAttackIsActive = false;

	// Get Leader Id
	int leaderId = clients[clientLeaderIndex].charId;

	for (int i = 0; i < clientTotal; i++)
	{
		if (i != clientLeaderIndex)
		{
			clients[i].doFollowUser(leaderId);
		}
	}

	std::stringstream message;
	message << "All clients is following ";
	message << clients[clientLeaderIndex].charName;
	// Print to console
	console::printl(message.str());
}

void clientmanager::toggleAutoAttack()
{
	autoAttackIsActive = !autoAttackIsActive;

	std::stringstream message;
	message << "Auto attack is ";
	message << (autoAttackIsActive ? "ON" : "OFF");
	// Print to console
	console::printl(message.str());
}

void clientmanager::set(std::string name, int clienNum)
{
	int index = findClientIndex(clienNum);
	if (index == -1)
		return;

	if (name == "leader")
	{
		clientLeaderIndex = index;
		std::stringstream message;
		message << "Leader Has been set to ";
		message << clients[clientLeaderIndex].charName;
		// Print to console
		console::print(message.str());
	}
	else if (name == "autoHP")
	{
		console::question("input 1 to enable it and 0 to disable it, [1/0]");
		bool value;
		std::cin >> value;
		clients[index].settings.autoPotHpEnabled = value;
	}
	else if (name == "autoMP")
	{
		console::question("input 1 to enable it and 0 to disable it, [1/0]");
		bool value;
		std::cin >> value;
		clients[index].settings.autoPotMpEnabled = value;
	}
	else if (name == "autoAttack")
	{
		console::question("input 1 to enable it and 0 to disable it, [1/0]");
		bool value;
		std::cin >> value;
		clients[index].settings.autoAttackEnabled = value;
	}
	else if (name == "HPPercentge")
	{
	}
	else if (name == "MPPercentage")
	{
	}
	else if (name == "potHPKey")
	{
	}
	else if (name == "potMPKey")
	{
	}
	else if (name == "skillKeys")
	{
	}
	else if (name == "followKey")
	{
	}
	else if (name == "selfTargetKey")
	{
	}
	else if (name == "nextTargetKey")
	{
	}
}

void clientmanager::showAllClientDetails()
{
	for (int i = 0; i < clientTotal; i++)
	{
		showClientDetails(i + 1);
		console::print("---------------------------------------\n");
	}
}

void clientmanager::showClientDetails(int clientNum)
{
	int index = findClientIndex(clientNum);
	gameclient *cl = &clients[index];
	std::stringstream ss;
	ss << "Name               : " << cl->charName << "\n";
	ss << "autoHP             : " << (cl->settings.autoPotHpEnabled ? "enabled" : "disabled") << "\n";
	ss << "autoMP             : " << (cl->settings.autoPotMpEnabled ? "enabled" : "disabled") << "\n";
	ss << "autoAttack         : " << (cl->settings.autoAttackEnabled ? "enabled" : "disabled") << "\n";
	ss << "auto HP percentage : " << cl->settings.toleratedHpPercentage << "%"
	   << "\n";
	ss << "auto HP percentage : " << cl->settings.toleratedMpPercentage << "%"
	   << "\n";
	console::print(ss.str());
}

void clientmanager::testFunction()
{
	for (int i = 0; i < clientTotal; i++)
	{
		clients[i].sendKeyStroke(112);
	}
}