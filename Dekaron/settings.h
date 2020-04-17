#pragma once
#include "stdafx.h"

class clientSettings
{
public:
	bool	autoPotHpEnabled = true;
	bool	autoPotMpEnabled = true;
	bool	autoAttackEnabled = true;
	int		toleratedHpPercentage = 70;
	int		toleratedMpPercentage = 20;
	// In Game Keys
	//-------------------------------------
	int					potHpKey = 112; // F1 key
	int					potMpKey = 113; // F2 key
	std::vector<int>	skillKeys = { 49,50,51,52,53,54,55,56,57,48 }; // key codes for 1,2,3,4,5,6,7,8,9,0
	std::vector<int>	keycodeFollow = { 17,85 }; // Combination keys: ctrl + U
	std::vector<int>	keycodeSelfTargeting = { 69 }; // E Keycode
	std::vector<int>	keycodeNextTarget = { 85 };
};

class toolSettings
{
public:
	// Proccess Key / Action Key
	std::vector<int>	autoAttackKey = { VK_SHIFT, 87 }; // ctrl + shift + W
	std::vector<int>	autoFollowKey = { VK_SHIFT, 69 }; // ctrl + shift + E
};


// Offset and address
// =======================================
struct offsets {
	int							charNameBaseAddr = 0x87A8D4;
	std::vector<unsigned int>	charName = { 0x4,0x8,0x100,0x4,0x18c };
	//
	int							charInfoBaseAddr = 0x87BC34;
	std::vector<unsigned int>	maxHealth = { 0x4, 0x44, 0x10 };
	std::vector<unsigned int>	curHealth = { 0x4, 0x44, 0x14 };
	std::vector<unsigned int>	maxMana	= { 0x4, 0x44, 0x24 };
	std::vector<unsigned int>	curMana	= { 0x4, 0x44, 0x28 };
	//
	int							selectedPlayerBaseAddr = 0x87BD74;
	std::vector<unsigned int>	selecterPlayer = { 0xc,0x88 };
};
