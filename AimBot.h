#pragma once
#include "Globals.h"
#include "Offsets.h"
#include "Driver.h"
#include "Utils.h"
#include <windows.h> // 对于 GetAsyncKeyState
#include <cstdlib> // 对于 rand() 和 srand()
#include <ctime> // 对于 time()
#include"NoRecoli.h"

class Aimbot {
public:
	Aimbot() {
		srand(time(NULL)); // 初始化随机数生成器
	}

	void update() {
		bool isAimKeyPressed = false;
		if (!shouldExecuteAimbot()) return;
		ULONG64 weaponID = GetEntity(Read<INT>(ProcessId, LocalPlayer + OFFSET_WEAPON - 20) & 0xFFFF);
		FLOAT bulletSpeed = Read<FLOAT>(ProcessId, weaponID + OFFSET_BULLET_SPEED);

		ULONG64 ptr = aimPtr;
		if (ptr == 0) return;
		Grenade();

		if (var::AimBotCheck && isVisible(ptr)) {

			Vector3 localPos = Read<Vector3>(ProcessId, LocalPlayer + OFFSET_CAMERAPOS);
			Vector3 entityPos = GetHitBoxPosition(ptr, AimBot_Bones);

			FLOAT predictDistance = localPos.DistTo(entityPos);
			Vector3 ptrVelocity = Read<Vector3>(ProcessId, ptr + OFFSET_ABS_VELOCITY);

			FLOAT bulletGravity = Read<FLOAT>(ProcessId, weaponID + OFFSET_BULLET_SCALE);
			entityPos = PredictPos(entityPos, ptrVelocity, predictDistance, bulletSpeed, bulletGravity, weaponID);

			Vector2 calcedAngle = CalcAngle(localPos, entityPos);
			Vector2 viewAngle = Read<Vector2>(ProcessId, LocalPlayer + OFFSET_VIEWANGLES);
			Vector2 newAngle = smoothAimMove(viewAngle, calcedAngle, AimBot_Smooth);
			newAngle.Normalize();

			Write<Vector2>(ProcessId, LocalPlayer + OFFSET_VIEWANGLES, newAngle);
		}
		else if (!var::AimBotCheck) {

			Vector3 localPos = Read<Vector3>(ProcessId, LocalPlayer + OFFSET_CAMERAPOS);
			Vector3 entityPos = GetHitBoxPosition(ptr, AimBot_Bones);

			FLOAT predictDistance = localPos.DistTo(entityPos);
			Vector3 ptrVelocity = Read<Vector3>(ProcessId, ptr + OFFSET_ABS_VELOCITY);

			FLOAT bulletGravity = Read<FLOAT>(ProcessId, weaponID + OFFSET_BULLET_SCALE);
			entityPos = PredictPos(entityPos, ptrVelocity, predictDistance, bulletSpeed, bulletGravity, weaponID);

			Vector2 calcedAngle = CalcAngle(localPos, entityPos);
			Vector2 viewAngle = Read<Vector2>(ProcessId, LocalPlayer + OFFSET_VIEWANGLES);
			Vector2 newAngle = smoothAimMove(viewAngle, calcedAngle, AimBot_Smooth);
			newAngle.Normalize();

			Write<Vector2>(ProcessId, LocalPlayer + OFFSET_VIEWANGLES, newAngle);
		}


	}

private:
	bool shouldExecuteAimbot() {


		return var::EnableAimBot;

	}





	Vector2 smoothAimMove(Vector2 currentAngle, Vector2 targetAngle, float smoothFactor) {
		Vector2 newAngle = targetAngle - currentAngle;
		newAngle.Normalize();
		return currentAngle + newAngle / smoothFactor;
	}

};
