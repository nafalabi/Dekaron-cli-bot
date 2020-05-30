#pragma once
#include "stdafx.h"
#include "processhelper.h"
#include "settings.h"
#include <WinUser.h>

class gameclient
{
private:
	DWORD		processId = 0;
	HANDLE		hProcess = 0;
	HWND		hwnd = 0;
	uintptr_t	baseAddr = 0;
public:
	// Contructors
	gameclient();
	gameclient(DWORD);

	// Settings
	clientSettings settings;
	offsets offsets;

	// Client Values
	int		charId = 0;
	CHAR	charName[16] = "";
	float	maxHealth = 0;
	float	curHealth = 0;
	float	maxMana = 0;
	float	curMana = 0;
	int		curSelectedTarget = 0;
	
	// Helper functions
	BOOL	isClientValid();
	DWORD	getPID();
	VOID	hydrateValues();
	VOID	setWindowName();
	VOID	sendKeyStroke(int keycode, int interval = 100);
	VOID	sendMultipleKeyStrokes(std::vector<int> keys, int interval = 100);

	// Values getter functions
	VOID	getCharName(CHAR*);
	FLOAT	getMaxHealth();
	FLOAT	getCurHealth();
	FLOAT	getMaxMana();
	FLOAT	getCurMana();
	int		getCurSelectedTarget();
	int		getSelfCharId();

	// Process functions
	void	healthAutoPot();
	void	manaAutoPot();
	void	doAutoAttack();
	void	doFollowUser(int userId);
};
