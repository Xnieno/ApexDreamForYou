#include<iostream>
#include"NoRecoli.h"
#include"Driver.h"
#include"Globals.h"
#include "Offsets.h"
#include <thread>





void NoRecoi()
{
	while (true)
	{
		if (var::EnableNoRecoli)
		{

			if (!LocalPlayer) return;
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			//Sleep(20);//这里的无后座，是起源引擎通用的。不睡眠有个bug，视角动太快了，直接给你拉到地上去了。你睡眠一下，就不会拉过去。会直接一个点
			int attack = Read<int>(ProcessId, GameBase + OFFSET_IN_ATTACK);
		
			if (attack) {
				Vector2 RecoilVec = Read<Vector2>(ProcessId, LocalPlayer + OFFSET_AIMPUNCH);
		
				RecoilVec.x *= 0.01 * var::NoRecoliRate;
				RecoilVec.y *= 0.01 * var::NoRecoliRate;
		
				Vector2 RecoilMove = RecoilVec * (1.0 / 0.022) / Read<FLOAT>(ProcessId, GameBase + mouse_sensitivity + 0x60);
				if (RecoilMove.x != 0 || RecoilMove.y != 0) {
			
					mouse_event(1, RecoilMove.y, -RecoilMove.x, 0, 0);
			
				}
			}
			


		}
	}




}