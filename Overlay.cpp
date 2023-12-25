#include"Overlay.h"
#include "Imgui/imgui.h"
#include <stdio.h>
#include <iostream>
#include"Driver.h"
#include "Externs.h"
#include "Imgui/imgui_internal.h"
#include "Utils.h"
#include "Offsets.h"
#include "Globals.h"
#include "Menu.h"
#include "AimBot.h"
void Overlay()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	char str[80]; // 假设足够大来容纳字符串
	float framerate = ImGui::GetIO().Framerate;
	snprintf(str, sizeof(str), "Fps %.1f", framerate);


	ImDrawList* drawlist = ImGui::GetForegroundDrawList();//GetBackgroundDrawList
	drawlist->AddText(ImVec2(20, 20), IM_COL32_WHITE, str);

	ImVec2 screenCenter = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

	std::vector<Entity> Entitys;
	Entitys.assign(aEntitys.begin(), aEntitys.end());

	float Matrix[4][4]{};
	ReadMemory(ProcessId, ViewMatrix, Matrix, 64);
	Vector3 LocalPos = Read<Vector3>(ProcessId, LocalPlayer + OFFSET_ORIGIN);
	aimPtr = 0;
	bool hasAim = false;

	uint64_t tempAimPtr = 0;
	float aimMinFov = AimBot_FOV;//最小值
	float aimTempFov = rcClient.right; //默认屏幕宽
	ImVec2 aimScreenPos = screenCenter;
	for (std::vector<Entity>::iterator iter = Entitys.begin(); iter != Entitys.end(); iter++)
	{

		ULONG64 Ptr = (*iter).Ptr;
		Vector2 sEntityPos{};
		Vector2 sHighestPos{};
		if (IsKnocked(Ptr) && var::Knocked == false)
			continue;

		Vector3 EntityPos = Read<Vector3>(ProcessId, Ptr + OFFSET_ORIGIN);
		Vector3 HeadPos = GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.Head);
		Vector3 NeckPos = GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.Neck);
		Vector3 HipPos = GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.Hip);

		Vector3 HighestPos = { EntityPos.x,EntityPos.y,HeadPos.z + 10 }; // vecMaxs 记录实体在引擎中的三维顶值
		WorldtoScreen2D(Matrix, HighestPos, sHighestPos);
		WorldtoScreen2D(Matrix, EntityPos, sEntityPos);
		int Level = getLevel(Read<int>(ProcessId, Ptr + m_xp));
		char InfoText[256]{};
		FLOAT Distance = EntityPos.DistTo(LocalPos);
		FLOAT RealDistance = Distance * INCHES_TO_METERS; // 引擎距离转换为真实距离
		if (sEntityPos.x <= 0 || sEntityPos.x >= rcClient.right || sEntityPos.y <= 0 || sEntityPos.y >= rcClient.bottom) continue;

		FLOAT RectHeight = std::fabs(sEntityPos.y - sHighestPos.y) * 1;
		FLOAT RectWidth = std::fabs(sEntityPos.y - sHighestPos.y) * 0.6;
		if (var::EnableEsp) {
			if (RealDistance < var::EspDistance)
			{
				if (RealDistance < var::SeerDistance)
				{
					if (var::EnableHealth) {
						// 计算 health 的填充宽度
						float healthFillWidth = (*iter).Health / 100.0f * 100.0f;

						// 绘制 health 的矩形框
						ImVec2 healthRectMin(sEntityPos.x - 50, sEntityPos.y - RectHeight - 10.0f);
						ImVec2 healthRectMax(healthRectMin.x + 100.0f, healthRectMin.y + 5.0f);
						drawlist->AddRect(healthRectMin, healthRectMax, IM_COL32(255, 255, 255, 255));

						// 绘制 health 的填充矩形，根据 health 设置透明度
						ImVec2 healthFillRectMax(healthRectMin.x + healthFillWidth, healthRectMax.y);
						healthFillRectMax.x = ImMin(healthFillRectMax.x, healthRectMax.x);  // 限制填充范围不超过矩形框
						drawlist->AddRectFilled(healthRectMin, healthFillRectMax, IM_COL32(255, 255, 255, 255), 0.0f);  // 使用白色填充
					}

					if (var::EnableShield)
					{
						// 计算 Shield 的填充宽度，根据不同的 Shield 值计算不同的宽度
						float shieldFillWidth;
						if ((*iter).Shield <= 50.0f) {
							shieldFillWidth = (*iter).Shield / 50.0f * 100.0f;
						}
						else if ((*iter).Shield <= 75.0f) {
							shieldFillWidth = (*iter).Shield / 75.0f * 100.0f;
						}
						else if ((*iter).Shield <= 100.0f) {
							shieldFillWidth = (*iter).Shield / 100.0f * 100.0f;
						}
						else {  // Assuming Shield values can't be greater than 125
							shieldFillWidth = (*iter).Shield / 125.0f * 100.0f;
						}

						ImVec2 ShieldRectMin(sEntityPos.x - 5, sEntityPos.y + 8);

						// 绘制 Shield 的矩形框，放在 health 的上方
						ImVec2 shieldRectMin(sEntityPos.x - 50, sEntityPos.y - RectHeight - 5 - 10.0f);
						ImVec2 shieldRectMax(shieldRectMin.x + 100.0f, shieldRectMin.y - 5);  // 修正此行
						drawlist->AddRect(shieldRectMin, shieldRectMax, IM_COL32(255, 255, 255, 255));

						// 根据 Shield 设置填充颜色
						ImU32 shieldFillColor = IM_COL32(255, 255, 255, 0);

						switch ((*iter).ShieldType)
						{
						case 1:
							shieldFillColor = IM_COL32(255, 255, 255, 255);  // 白色
							break;
						case 2:
							shieldFillColor = IM_COL32(0, 0, 255, 255);  // 蓝色
							break;
						case 3:
							shieldFillColor = IM_COL32(128, 0, 128, 255);  // 紫色
							break;
						case 4:
							shieldFillColor = IM_COL32(255, 255, 0, 255);  // 金色
							break;
						case 5:
							shieldFillColor = IM_COL32(255, 0, 0, 255);  // 红色
							break;
						default:
							break;
						}





						// 绘制 Shield 的填充矩形
						ImVec2 shieldFillRectMax(shieldRectMin.x + shieldFillWidth, shieldRectMax.y);
						shieldFillRectMax.x = ImMin(shieldFillRectMax.x, shieldRectMax.x);  // 限制填充范围不超过矩形框
						drawlist->AddRectFilled(shieldRectMin, shieldFillRectMax, shieldFillColor, 0.0f);
					}

				}
				// 计算矩形的一半宽度
				if (var::Enable2Dbox)
				{

					FLOAT RectHeight = std::fabs(sEntityPos.y - sHighestPos.y) * 1;
					FLOAT RectWidth = std::fabs(sEntityPos.y - sHighestPos.y) * 0.6;

					// 计算矩形的一半宽度
					FLOAT HalfWidth = RectWidth / 4;
					Vector2 boxTopLeft = { sEntityPos.x - RectWidth / 2,sEntityPos.y - RectHeight };

					drawlist->AddLine(ImVec2(boxTopLeft.x, boxTopLeft.y), ImVec2(boxTopLeft.x + HalfWidth, boxTopLeft.y), IM_COL32(255, 255, 255, 255), 2.0f);
					drawlist->AddLine(ImVec2(boxTopLeft.x, boxTopLeft.y), ImVec2(boxTopLeft.x, boxTopLeft.y + HalfWidth), IM_COL32(255, 255, 255, 255), 2.0f);

					drawlist->AddLine(ImVec2(boxTopLeft.x + RectWidth, boxTopLeft.y), ImVec2(boxTopLeft.x + RectWidth - HalfWidth, boxTopLeft.y), IM_COL32(255, 255, 255, 255), 2.0f);
					drawlist->AddLine(ImVec2(boxTopLeft.x + RectWidth, boxTopLeft.y), ImVec2(boxTopLeft.x + RectWidth, boxTopLeft.y + HalfWidth), IM_COL32(255, 255, 255, 255), 2.0f);

					drawlist->AddLine(ImVec2(boxTopLeft.x, boxTopLeft.y + RectHeight), ImVec2(boxTopLeft.x + HalfWidth, boxTopLeft.y + RectHeight), IM_COL32(255, 255, 255, 255), 2.0f);
					drawlist->AddLine(ImVec2(boxTopLeft.x, boxTopLeft.y + RectHeight), ImVec2(boxTopLeft.x, boxTopLeft.y + RectHeight - HalfWidth), IM_COL32(255, 255, 255, 255), 2.0f);

					drawlist->AddLine(ImVec2(boxTopLeft.x + RectWidth, boxTopLeft.y + RectHeight), ImVec2(boxTopLeft.x + RectWidth - HalfWidth, boxTopLeft.y + RectHeight), IM_COL32(255, 255, 255, 255), 2.0f);
					drawlist->AddLine(ImVec2(boxTopLeft.x + RectWidth, boxTopLeft.y + RectHeight), ImVec2(boxTopLeft.x + RectWidth, boxTopLeft.y + RectHeight - HalfWidth), IM_COL32(255, 255, 255, 255), 2.0f);
				}
				if (strlen((*iter).nickName) <= 64) {

					if (var::EnableDistance)
					{
						sprintf_s(InfoText, "%d M/", (int)RealDistance);
					}

					if (var::EnableName)
					{
						strcat_s(InfoText, " ");
						strcat_s(InfoText, (*iter).nickName);
					}

					if (var::EnableLevel)
					{
						strcat_s(InfoText, " ");
						sprintf_s(InfoText + strlen(InfoText), sizeof(InfoText) - strlen(InfoText), "%d Level,", Level);
					}

					if (var::EnableTeamId)
					{
						strcat_s(InfoText, " ");
						sprintf_s(InfoText + strlen(InfoText), sizeof(InfoText) - strlen(InfoText), "%d TeamId", (*iter).Team);

					}

					if (var::EnableHero)
					{
						strcat_s(InfoText, " ");
						sprintf_s(InfoText + strlen(InfoText), sizeof(InfoText) - strlen(InfoText), "%s ", (*iter).ModelName);
					}

					if (var::EnableUserID)
					{
						strcat_s(InfoText, " ");
						sprintf_s(InfoText + strlen(InfoText), sizeof(InfoText) - strlen(InfoText), "%llu uid", (*iter).UserId);
					}

					if (var::EnablePRO)
					{
						if ((*iter).isPro == true)
						{
							strcat_s(InfoText, " ");
							sprintf_s(InfoText + strlen(InfoText), sizeof(InfoText) - strlen(InfoText), "%d PROLevel", (*iter).PROLevel);
						}
					}



				}

				ImVec2 InfoTextPos = { sEntityPos.x - ImGui::CalcTextSize(InfoText).x / 2,sEntityPos.y + 12 };
				drawlist->AddText(myFont, 18, { InfoTextPos.x - 1,InfoTextPos.y - 1 }, 0xAA000000, InfoText);
				drawlist->AddText(myFont, 18, { InfoTextPos.x - 1,InfoTextPos.y + 1 }, 0xAA000000, InfoText);
				drawlist->AddText(myFont, 18, { InfoTextPos.x + 1,InfoTextPos.y - 1 }, 0xAA000000, InfoText);
				drawlist->AddText(myFont, 18, { InfoTextPos.x + 1,InfoTextPos.y + 1 }, 0xAA000000, InfoText);
				drawlist->AddText(myFont, 18, { InfoTextPos.x ,InfoTextPos.y }, 0xAA000000, InfoText);
				drawlist->AddText(myFont, 18, InfoTextPos, 0xFFFFFFFF, InfoText);

				bool drawBoneResult = true;

				drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, HeadPos, (*iter).Bones2D.Head) : false;
				drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, NeckPos, (*iter).Bones2D.Neck) : false;
				drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, HipPos, (*iter).Bones2D.Hip) : false;
				drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.leftHand), (*iter).Bones2D.leftHand) : false;
				drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.rightHand), (*iter).Bones2D.rightHand) : false;
				drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.leftHeel), (*iter).Bones2D.leftHeel) : false;
				drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.rightHeel), (*iter).Bones2D.rightHeel) : false;
				bool lowPoly = RealDistance > 50.f && !Read<bool>(ProcessId, LocalPlayer + 0x1b91);

				if (!lowPoly)
				{
					drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.leftKnee), (*iter).Bones2D.leftKnee) : false;
					drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.rightKnee), (*iter).Bones2D.rightKnee) : false;
					drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.leftElbow), (*iter).Bones2D.leftElbow) : false;
					drawBoneResult = drawBoneResult ? WorldtoScreen2D(Matrix, GetBonePosition((*iter).BoneArray, EntityPos, (*iter).Bones.rightElbow), (*iter).Bones2D.rightElbow) : false;
				}

				ImColor Color = ImColor(255, 255, 0, 255);
				FLOAT Thickness = 1.f;
				if (var::EnableBones) {
					if (lowPoly) {
						if (var::EnableBones)
						{
							drawlist->AddLine({ (*iter).Bones2D.Head.x,(*iter).Bones2D.Head.y }, { (*iter).Bones2D.Hip.x,(*iter).Bones2D.Hip.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.Neck.x,(*iter).Bones2D.Neck.y }, { (*iter).Bones2D.leftHand.x,(*iter).Bones2D.leftHand.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.Neck.x,(*iter).Bones2D.Neck.y }, { (*iter).Bones2D.rightHand.x,(*iter).Bones2D.rightHand.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.Hip.x,(*iter).Bones2D.Hip.y }, { (*iter).Bones2D.leftHeel.x,(*iter).Bones2D.leftHeel.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.Hip.x,(*iter).Bones2D.Hip.y }, { (*iter).Bones2D.rightHeel.x,(*iter).Bones2D.rightHeel.y }, Color, Thickness);
						}

					}
					else {
						if (var::EnableBones)
						{
							drawlist->AddLine({ (*iter).Bones2D.Head.x,(*iter).Bones2D.Head.y }, { (*iter).Bones2D.Neck.x,(*iter).Bones2D.Neck.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.Neck.x,(*iter).Bones2D.Neck.y }, { (*iter).Bones2D.leftElbow.x,(*iter).Bones2D.leftElbow.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.Neck.x,(*iter).Bones2D.Neck.y }, { (*iter).Bones2D.rightElbow.x,(*iter).Bones2D.rightElbow.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.leftElbow.x,(*iter).Bones2D.leftElbow.y }, { (*iter).Bones2D.leftHand.x,(*iter).Bones2D.leftHand.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.rightElbow.x,(*iter).Bones2D.rightElbow.y }, { (*iter).Bones2D.rightHand.x,(*iter).Bones2D.rightHand.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.Neck.x,(*iter).Bones2D.Neck.y }, { (*iter).Bones2D.Hip.x,(*iter).Bones2D.Hip.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.Hip.x,(*iter).Bones2D.Hip.y }, { (*iter).Bones2D.leftKnee.x,(*iter).Bones2D.leftKnee.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.Hip.x,(*iter).Bones2D.Hip.y }, { (*iter).Bones2D.rightKnee.x,(*iter).Bones2D.rightKnee.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.leftKnee.x,(*iter).Bones2D.leftKnee.y }, { (*iter).Bones2D.leftHeel.x,(*iter).Bones2D.leftHeel.y }, Color, Thickness);
							drawlist->AddLine({ (*iter).Bones2D.rightKnee.x,(*iter).Bones2D.rightKnee.y }, { (*iter).Bones2D.rightHeel.x,(*iter).Bones2D.rightHeel.y }, Color, Thickness);
						}

					}
				}

			}
		}


		float aimDis2d = (*iter).Bones2D.Head.Dist2D({ screenCenter.x , screenCenter.y });


		if (aimDis2d < aimTempFov && RealDistance < var::AimBotDistance)
		{

			aimTempFov = aimDis2d;
			if (AimBot_Mode == 0)
			{
				if (RectWidth * 2/*变量*/ > aimMinFov) aimMinFov = RectWidth * 2;
			}

			if (aimDis2d < aimMinFov) {
				if (RealDistance < var::AimBotDistance) {
					//检查可视
					hasAim = true;
					tempAimPtr = Ptr;
					aimScreenPos = { (*iter).Bones2D.Head.x ,(*iter).Bones2D.Head.y };

				}
			}
		}


	}

	if (hasAim)
	{
		aimPtr = tempAimPtr;
		if (var::AssistLine) {
			drawlist->AddLine(screenCenter, aimScreenPos, IM_COL32_WHITE);
		}

	}


	if (var::Enable_AimBot_FOV)
	{
		drawlist->AddCircle(screenCenter, aimMinFov, IM_COL32_WHITE, 40.0F, var::AimBot_FOV_Thickness);
	}
}





