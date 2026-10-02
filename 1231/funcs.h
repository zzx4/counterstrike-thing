#pragma once
#include "MemMan.h"
#include "csgo.hpp"

MemMan MemCls;

struct offset
{
	DWORD player = hazedumper::signatures::dwLocalPlayer;
	DWORD jump = hazedumper::signatures::dwForceJump;
	DWORD flag = hazedumper::netvars::m_fFlags;
	DWORD forceatk = hazedumper::signatures::dwForceAttack;
	DWORD i_team = hazedumper::netvars::m_iTeamNum;
	DWORD i_health = hazedumper::netvars::m_iHealth;
	DWORD ent_list = hazedumper::signatures::dwEntityList;
	DWORD crosshair = hazedumper::netvars::m_iCrosshairId;
	DWORD GlowIndex = hazedumper::netvars::m_iGlowIndex;
	DWORD GlowObject = hazedumper::signatures::dwGlowObjectManager;
	DWORD fov = hazedumper::netvars::m_iFOV;
	DWORD observer = hazedumper::netvars::m_iObserverMode;
}offset;

struct variables
{
	DWORD localPlay;
	DWORD proc;
	DWORD gameMod;
	BYTE flag;
	int iteam;
}variable;


void fire()
{
	MemCls.writeMem<int>(variable.gameMod + offset.forceatk, 5);
	Sleep(10);
	MemCls.writeMem<int>(variable.gameMod + offset.forceatk, 4);
	Sleep(10);
}

bool checkbot()
{
	int xhair = MemCls.readMem<int>(variable.localPlay + offset.crosshair);
	if (xhair != 0 && xhair < 64) // validity check
	{
		DWORD ent = MemCls.readMem<DWORD>(variable.gameMod + offset.ent_list + ((xhair - 1) * 0x10)); // get entity
		int enemyteam = MemCls.readMem<int>(ent + offset.i_team); // get team
		int enemyhealth = MemCls.readMem<int>(ent + offset.i_health); // get health
		if (enemyteam != variable.iteam && enemyhealth > 0) // if entity is healthy and on enemy team
			return true;
		else
			return false;
	}
	else
		return false;
}

void handlebot()
{
	if (checkbot())
		fire();
}

bool pressing(unsigned const vk_code)
{
	return 0 != GetAsyncKeyState(vk_code);
}

void glow()
{
	while (true)
	{
		variable.flag = MemCls.readMem<BYTE>(variable.localPlay + offset.flag);
		variable.iteam = MemCls.readMem<int>(variable.localPlay + offset.i_team);

		DWORD glowObj = MemCls.readMem<DWORD>(variable.gameMod + offset.GlowObject);

		for (int i = 0; i < 32; i++) {
			DWORD ent = MemCls.readMem<DWORD>(variable.gameMod + offset.ent_list + i * 0x10);
			DWORD myself = MemCls.readMem<DWORD>(variable.gameMod + offset.player);
			int enthealth = MemCls.readMem<int>(ent + offset.i_health);

			if (ent != NULL) {
				int glowIndex = MemCls.readMem<int>(ent + offset.GlowIndex);
				int entTeam = MemCls.readMem<int>(ent + offset.i_team);

				if (ent == myself)
				{
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0x8), 0.84);
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0xC), 0.5);
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0x10), 1);
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0x14), 0.8);
				}
				else if (variable.iteam == entTeam) {
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0x8), 0);
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0xC), 0);
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0x10), 1);
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0x14), 0.5);
				}
				else {
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0x8), (1 - (enthealth / float(100))));
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0xC), (enthealth / float(100)));
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0x10), 0);
					MemCls.writeMem<float>(glowObj + ((glowIndex * 0x38) + 0x14), 0.8);
				}
				MemCls.writeMem<bool>(glowObj + ((glowIndex * 0x38) + 0x28), true);
				MemCls.writeMem<bool>(glowObj + ((glowIndex * 0x38) + 0x29), false);
			}
		}
		Sleep(1);
	}
}

void jump()
{
	while (true)
	{
		if (pressing(VK_SPACE))
		{
			if (variable.flag & (1 << 0))
				MemCls.writeMem<DWORD>(variable.gameMod + offset.jump, 6);
		}
		Sleep(1);
	}
}

void trigger()
{
	while (true)
	{
		if (pressing(VK_MENU)) {
			handlebot();
		}
		Sleep(1);
	}
}

void tp()
{
	while (true)
	{
		while (pressing(VK_MBUTTON)) {
			MemCls.writeMem<int>(variable.localPlay + offset.observer, 1);
			Sleep(1);
		}

		MemCls.writeMem<int>(variable.localPlay + offset.observer, 0);
		Sleep(1);
	}
}