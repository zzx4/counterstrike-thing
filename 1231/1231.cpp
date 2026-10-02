#include <iostream>
#include <Windows.h>
#include "MemMan.h"
#include "csgo.hpp"
#include "funcs.h"
#include <thread>
#include <chrono>

int main()
{
	bool tbotstate = false;
	int proc = MemCls.getProcess(L"csgo.exe");
	variable.proc = MemCls.getProcess(L"csgo.exe");
	variable.gameMod = MemCls.getModule(variable.proc, L"client.dll");

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, variable.proc);

	variable.localPlay = MemCls.readMem<DWORD>(variable.gameMod + offset.player);

	if (variable.localPlay == NULL)
		while (variable.localPlay == NULL) {
			variable.localPlay = MemCls.readMem<DWORD>(variable.gameMod + offset.player);
			Sleep(10000);
		}

	std::thread glowing(glow);
	std::thread jumping(jump);
	std::thread trigg(trigger);
	//std::thread third(tp);

	while (true)
	{
		
		variable.localPlay = MemCls.readMem<DWORD>(variable.gameMod + offset.player);

		if (variable.localPlay == NULL)
			while (variable.localPlay == NULL) {
				variable.localPlay = MemCls.readMem<DWORD>(variable.gameMod + offset.player);
				Sleep(10000);
			}

		//MemCls.writeMem<int>(variable.localPlay + offset.fov, 120);
		
		Sleep(45000);

	}

	glowing.join();
	jumping.join();
	trigg.join();
	//third.join();

}