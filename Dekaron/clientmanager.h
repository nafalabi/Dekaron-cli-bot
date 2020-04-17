#pragma once
#include "stdafx.h"
#include "gameclient.h"
#include "console.h"
#include "settings.h"

class clientmanager
{
private:
	gameclient		clients[10];
	int				clientTotal;
	bool			checkClientExist(DWORD);
	// Clients state
	//-------------------------------------
	int				clientLeaderIndex = 0;
	bool			processIsActive = true;
	bool			autoAttackIsActive = false;
public:
	// Settings
	toolSettings settings;
	// Constructor
	clientmanager();
	// Helper functions
	void addClient(DWORD);
	void addClients(std::vector<int>* PIDs);
	void removeClient(int clientNum);
	void removeClients();
	int findClientIndex(int clientNum);
	void printClients();
	void callClientsMethod(void(gameclient::* funcPtr)());
	void callClientsMethodAsync(void(gameclient::* funcPtr)());
	// Process Functions
	bool getProcessState();
	void mainProcess();
	void stopProcess();
	// Action Functions
	void setLeaderByPID(int PID);
	void toggleAutoFollow();
	void toggleAutoAttack();
	void set(std::string name, int clienNum);
	void showAllClientDetails();
	void showClientDetails(int clientNum);
	void testFunction();
};

