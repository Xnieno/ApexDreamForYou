#include"Menu.h"
#include "Render.h"
#include"Globals.h"
#include <set>
#include "Utils.h"
void Menu()
{
	if (Show_Menu)
	{
		CustomStyleColor();
		DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;
		float tab_size = 0.f;

		D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };

		if (bg == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, background, sizeof(background), &info, pump, &bg, 0);

		ImGui::GetBackgroundDrawList()->AddImage(bg, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(100, 100, 100, 255));

		if (us == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, user, sizeof(user), &info, pump, &us, 0);

		ImGui::SetNextWindowSize(ImVec2(855 * dpi_scale, 790 * dpi_scale));
		ImGui::Begin("Menu", &menu, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
		{

			const auto& p = ImGui::GetWindowPos();

			ImGuiStyle& s = ImGui::GetStyle();

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
			ImGui::BeginChild("G-Tab", ImVec2(173 * dpi_scale, 790 * dpi_scale), false);
			{
				ImGui::GetForegroundDrawList()->AddText(tab_text3, 20 * dpi_scale, ImVec2(20 * dpi_scale + p.x, 12 * dpi_scale + p.y), ImColor(255, 255, 255, 255), "HF");
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0 + p.x, 0 + p.y), ImVec2(273 * dpi_scale + p.x, 790 * dpi_scale + p.y), ImGui::GetColorU32(colors::Tab_Child), s.WindowRounding);

				ImGui::SetCursorPosY(60);

				ImGui::SetWindowFontScale(dpi_scale);
				if (ImGui::Tab("H", "Aimbot", "Legit,Trigger,Rage", 0 == tabs, ImVec2(150 * dpi_scale, 42 * dpi_scale))) tabs = 0;

				if (ImGui::Tab("F", "Visuals", "Player,World,Glow", 2 == tabs, ImVec2(150 * dpi_scale, 42 * dpi_scale))) tabs = 2;
				if (ImGui::Tab("E", "Misc", "Other settings", 3 == tabs, ImVec2(150 * dpi_scale, 42 * dpi_scale))) tabs = 3;

				/*if (ImGui::Tab("B", "Config", "Manage your configs", 6 == tabs, ImVec2(150 * dpi_scale, 42 * dpi_scale))) tabs = 6;*/



			}ImGui::EndChild();


			ImGui::PopStyleColor();

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0 + p.x, 0 + p.y), ImVec2(855 * dpi_scale + p.x, 790 * dpi_scale + p.y), ImGui::GetColorU32(colors::main_color), s.WindowRounding);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0 + p.x, 755 * dpi_scale + p.y), ImVec2(855 * dpi_scale + p.x, 755 * dpi_scale + p.y), ImGui::GetColorU32(colors::lite_color), s.WindowRounding);

			//   ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0 + p.x, 0 + p.y), ImVec2(705 + p.x, 60 + p.y), ImGui::GetColorU32(colors::lite_color), s.WindowRounding);


			tab_alpha = ImClamp(tab_alpha + (7.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
			tab_add = ImClamp(tab_add + (std::round(50.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

			if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;


			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * s.Alpha);

			ImGui::SetCursorPos(ImVec2(203 * dpi_scale, 30 * dpi_scale));

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
			ImGui::BeginChild("General", ImVec2(717 * dpi_scale, 720 * dpi_scale), false);
			{
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetColorU32(colors::lite_color));
				switch (active_tab) {

				case 0:
				{
					ImGui::SetCursorPosY(0 * dpi_scale);
					ImGui::BeginChildPos("AimBot", ImVec2(300 * dpi_scale, 720 * dpi_scale));
					{
						ImGui::SetWindowFontScale(dpi_scale);
						ImGui::Checkbox("Enable Aimbot", &var::EnableAimBot);
						ImGui::Checkbox("Enable AimbotFOV", &var::Enable_AimBot_FOV);
						const char* AimBotMode[2]{ "Dynamic AimBot", "Static AimBot" };
						ImGui::Combo("AimBot Mode", &AimBot_Mode, AimBotMode, IM_ARRAYSIZE(AimBotMode), 2);
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "FirstAimKey:");
						ImGui::SameLine(0, 45);
						HotkeyButton(aimkey, ChangeKey, keystatus);



						ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "SecondAimKey:");
						ImGui::SameLine(0, 30);
						HotkeyButton(aimkey1, ChangeKey1, keystatus1);
						const char* AimBotBone3[3]{ "Head", "Neck","Chest" };
						ImGui::Combo("AimBot Bones", &AimBot_Bones, AimBotBone3, IM_ARRAYSIZE(AimBotBone3), 3);
						ImGui::SliderFloat("AimBot Smooth", &AimBot_Smooth, 5, 100);
						ImGui::SliderInt("AimBot Fov", &AimBot_FOV, 0, 300);

				

					}
					ImGui::EndChild();
					ImGui::SetCursorPos(ImVec2(320 * dpi_scale, 0 * dpi_scale));
					ImGui::BeginChildPos("Trigger&&NoRecoli", ImVec2(300 * dpi_scale, 720 * dpi_scale));
					{
						ImGui::SetWindowFontScale(dpi_scale);
					
						ImGui::Checkbox("Enable NoRecoli", &var::EnableNoRecoli);
						ImGui::SliderInt("NoRecoli Rate", &var::NoRecoliRate,0,100);

						ImGui::Checkbox("Lock Knocked", &var::Knocked);
						ImGui::Checkbox("AimBot Visible Check", &var::AimBotCheck);
						ImGui::Checkbox("AimBot Assist Line", &var::AssistLine);
						ImGui::SliderInt("AimBot Distance", &var::AimBotDistance, 0, 450);
				

					}
					ImGui::EndChild();

				}
				break;
				case 2: {
					ImGui::SetCursorPosY(0 * dpi_scale);
					ImGui::BeginChildPos("Glow", ImVec2(300 * dpi_scale, 720 * dpi_scale));
					{
						ImGui::SetWindowFontScale(dpi_scale);
						
						ImGui::Checkbox("Player Glow", &var::EnablePlayerGlow);
						ImGui::Checkbox("NPC Glow", &var::EnableNPCGlow);
						ImGui::Checkbox("Enhance Glow", &var::EnhanceGlow);
						//ImGui::Checkbox("Weapon Disappare", &var::EnableWeaponDisappare);
						const char* GlowModeitems[9]{ "Bloodhound", "Black", "Custom", "Sonar","Flash","Color","Scan","Digital","Dynamic" };
						ImGui::Combo("GlowMode", &var::GlowModeSelect, GlowModeitems, IM_ARRAYSIZE(GlowModeitems), 9);
					
						ImGui::SliderInt("Glow BorderSize", &var::Glow_Border_Size, 45, 250);
						ImGui::ColorEdit4("Visible Color", var::Visible_color, picker_flags);
						ImGui::ColorEdit4("InVisible Color", var::InVisible_color, picker_flags);
						ImGui::Checkbox("Item Glow", &var::EnableItemGlow);

						ImGui::MultiCombo("Item Glow Select", var::ItemGlowSelects, itemGlows, 5);


					}
					ImGui::EndChild();

					ImGui::SetCursorPos(ImVec2(320 * dpi_scale, 0 * dpi_scale));
					ImGui::BeginChildPos("Esp", ImVec2(300 * dpi_scale, 720 * dpi_scale));
					{
						ImGui::BeginChild("Esp", ImVec2(300 * dpi_scale, 720 * dpi_scale));
						{
							ImGui::SetWindowFontScale(dpi_scale);
							ImGui::Checkbox("Enable Esp", &var::EnableEsp);
							ImGui::SliderInt("Enable EspDistance", &var::EspDistance, 0, 300);
							ImGui::SliderInt("Enable SeerDistance", &var::SeerDistance, 0, 200);
							ImGui::Checkbox("Enable 2DBox", &var::Enable2Dbox);
							ImGui::Checkbox("Enable EnemyLevel", &var::EnableLevel);
							ImGui::Checkbox("Enable EnemyDistance", &var::EnableDistance);
							ImGui::Checkbox("Enable ClassName", &var::EnableHero);
							ImGui::Checkbox("Enable EnemyName", &var::EnableName);
							ImGui::Checkbox("Enable EnemyHealth", &var::EnableHealth);
							ImGui::Checkbox("Enable EnemyShield", &var::EnableShield);
							ImGui::Checkbox("Enable EnemyTeamId", &var::EnableTeamId);
							ImGui::Checkbox("Enable EnemyPRO", &var::EnablePRO);
							ImGui::Checkbox("Enable Watcher", &var::EnableWattcher);
							ImGui::Checkbox("Enable Bones", &var::EnableBones);
							ImGui::Checkbox("Enable UserId", &var::EnableUserID);

						}
						ImGui::EndChild();
					}


					ImGui::EndChild();
				}
					  break;
				case 3: {
					ImGui::SetCursorPosY(0 * dpi_scale);
					ImGui::BeginChildPos("Others", ImVec2(300 * dpi_scale, 720 * dpi_scale));
					{
						ImGui::SetWindowFontScale(dpi_scale);

						ImGui::Checkbox("Enable Starfe", &var::EnableStrfe);
						ImGui::Checkbox("Auto Grapple", &var::Autto_Grapple);
						ImGui::Checkbox("Rabbit Jumps", &var::RadbbitJump);
						ImGui::Checkbox("Rope Jump", &var::RoppleJump);
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "RoppleJumpKey:");
						ImGui::SameLine(0, 45);
						HotkeyButton(aimkey3, ChangeKey3, keystatus3);
						ImGui::Checkbox("Auto Grenade", &var::Grenade);
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "AutoGrenadeJumpKey:");
						ImGui::SameLine(0, 45);
						HotkeyButton(aimkey4, ChangeKey4, keystatus4);

					/*ImGui::SliderFloat("AutoGrenadeSmooth", &var::GrenadeSmooth, 0, 100);*/
					/*	ImGui::Checkbox("Auto SG", &var::superGulide);
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "SuperGulideKey:");
						ImGui::SameLine(0, 45);
						HotkeyButton(aimkey5, ChangeKey5, keystatus5);*/

						ImGui::Checkbox("Enable FullMap", &var::FullMap);
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "FullMapKey:");
						ImGui::SameLine(0, 45);
						HotkeyButton(aimkey6, ChangeKey6, keystatus6);
						ImGui::Checkbox("EnableSecurity", &var::Security);
				
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "SecurityKey:");
						ImGui::SameLine(0, 45);
						HotkeyButton(aimkey7, ChangeKey7, keystatus7);



					}
					ImGui::EndChild();	
				}
					  break;
				case 7:
				{
					ImGui::BeginChildPos("", ImVec2(620 * dpi_scale, 100 * dpi_scale));
					{
						ImGui::GetForegroundDrawList()->AddText(tab_text3, 26 * dpi_scale, ImVec2(475 * dpi_scale + p.x, 55 * dpi_scale + p.y), ImColor(255, 255, 255, 255), "ChatBox");
						ImGui::GetForegroundDrawList()->AddText(tab_text3, 16 * dpi_scale, ImVec2(385 * dpi_scale + p.x, 85 * dpi_scale + p.y), ImColor(255, 255, 255, 255), "Chat online with other subscribed users");
					}
					ImGui::EndChild();
					ImGui::SetCursorPosY(120 * dpi_scale);
					ImGui::BeginChildPos("  ", ImVec2(620 * dpi_scale, 490 * dpi_scale));
					{ImGui::SetWindowFontScale(dpi_scale);
					static bool checboxes[40];
					ImGui::Text("credits to: hefan2429", checboxes);

					}
					ImGui::EndChild();
					ImGui::BeginChild(" ", ImVec2(620 * dpi_scale, 70 * dpi_scale), false);
					{ImGui::SetWindowFontScale(dpi_scale);
					ImGui::SetCursorPosX(10 * dpi_scale);
					ImGui::InputText("Enter chat content here", var::input, 64);
					ImGui::SetCursorPos(ImVec2(500 * dpi_scale, 30 * dpi_scale));
					ImGui::Button("Sending chat");
					}
					ImGui::EndChild();
				}
				break;
				case 8:
				{
					ImGui::BeginChildPos("Search here for what you want", ImVec2(620 * dpi_scale, 700 * dpi_scale));
					{ImGui::SetWindowFontScale(dpi_scale);
					ImGui::GetForegroundDrawList()->AddText(ico, 30 * dpi_scale, ImVec2(770 * dpi_scale + p.x, 85 * dpi_scale + p.y), ImColor(93, 93, 93, 255), "I");
					ImGui::SetCursorPosY(30 * dpi_scale);
					ImGui::SetNextItemWidth(540 * dpi_scale);
					ImGui::InputText(" ", var::input, 64);

					}
					ImGui::EndChild();
				}
				break;
				}
				ImGui::PopStyleColor(1);

				ImGui::Spacing();

				ImGui::EndChild();

				ImGui::PopStyleColor(1);
			}
			if (animated_background)
				Particles();
			ImGui::PopStyleVar(1);

		}
		ImGui::End();
	}

}


void ProMenu() {

	// 设置窗口大小和背景透明度
	ImVec2 windowSize = ImVec2(400, 300); // 设置窗口的固定大小
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.3f); // 半透明灰色背景

	// 创建窗口
	if (ImGui::Begin("Pro", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
		// 计算文本宽度并居中显示
		const char* title = u8"权限玩家";
		ImVec2 textSize = ImGui::CalcTextSize(title);
		float titleAreaHeight = 50.0f; // 假设标题区域高度为50像素

		ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f); // 水平居中
		ImGui::SetCursorPosY((titleAreaHeight - textSize.y) * 0.5f); // 垂直居中

		// 设置大号字体
		ImGui::PushFont(largeFont); // 假设已经创建了 largeFont
		ImGui::TextColored(ImVec4(1, 1, 1, 1), title); // 纯白文本
		ImGui::PopFont();

		// 重置光标位置到标题区域的底部，并画一条横线
		ImGui::SetCursorPosY(titleAreaHeight);

		ImGui::Separator();


		// 在窗口渲染过程中
		for (const auto& player : Proplayers)
		{
			if (player.isPro != true)
				continue;

			std::string nickname = player.nickName;
			int proLevel = player.PROLevel;
			std::string prostatus = player.Status;
			// 对昵称和权限等级都进行唯一性检查

			ImGui::SameLine(0, 80);
			ImGui::PushFont(largeFont);
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", nickname.c_str()); // 显示昵称
			ImGui::PopFont();



			// 在同一行显示等级
			ImGui::SameLine(0, 10);
			ImGui::PushFont(largeFont);
			ImGui::TextColored(ImVec4(1, 1, 1, 1), u8"权限等级: %d", proLevel); // 显示等级
			ImGui::PopFont();
			ImGui::SameLine(0, 10);
			ImGui::PushFont(largeFont);
			ImGui::TextColored(ImVec4(1, 1, 1, 1), u8"当前状态: %s", prostatus.c_str()); // 显示昵称
			ImGui::PopFont();

			ImGui::NewLine();

		}


		ImGui::End();
	}
}

void SpectatorMenu() {
	// 设置窗口大小和背景透明度
	ImVec2 windowSize = ImVec2(400, 300); // 设置窗口的固定大小
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::SetNextWindowBgAlpha(0.3f); // 半透明灰色背景

	// 创建窗口
	if (ImGui::Begin("Spectators", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
		// 计算文本宽度并居中显示
		const char* title = u8"观战玩家";
		ImVec2 textSize = ImGui::CalcTextSize(title);
		float titleAreaHeight = 50.0f; // 假设标题区域高度为50像素

		ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f); // 水平居中
		ImGui::SetCursorPosY((titleAreaHeight - textSize.y) * 0.5f); // 垂直居中

		// 设置大号字体
		ImGui::PushFont(largeFont); // 假设已经创建了 largeFont
		ImGui::TextColored(ImVec4(1, 1, 1, 1), title); // 纯白文本
		ImGui::PopFont();

		// 重置光标位置到标题区域的底部，并画一条横线
		ImGui::SetCursorPosY(titleAreaHeight);
		ImGui::Separator();

		// 遍历观战玩家名单并显示每个玩家的名字
		for (const auto& spectator : spectatorList) {
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", spectator.c_str()); // 显示玩家名字
			ImGui::NewLine(); // 每个名字后换行
		}

		ImGui::End();
	}
}


