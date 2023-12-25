#pragma once
#include<Windows.h>
#include<vector>
#include <d3d11.h>
#include "Imgui/imgui.h"
#include "Externs.h"
#include <map>
#include <set>
#include <string>

static inline std::map<std::string, std::string> ModelNameDic = {
								   {"dummie", "NPC"},
								   {"stim",u8"动力小子"},
								   {"bangalore", u8"班加罗尔"},
								   {"fuse", u8"暴雷"},
								   {"ash", u8"艾许"},
								   {"madmaggie",u8"疯马吉"},
								   {"ballistic",u8"弹道"},
								   {"pathfinder",u8"探路者"},
								   {"wraith",u8"恶灵"},
								   {"revenant",u8"亡灵"},
								   {"nova",u8"地平线"},
								   {"valkyrie",u8"瓦尔基里"},
								   {"bloodhound",u8"寻血猎犬"},
								   {"crypto",u8"密客"},
								   {"seer",u8"希尔"},
								   {"vantage",u8"万蒂奇"},
								   {"gibraltar",u8"直布罗陀"},
								   {"lifeline",u8"命脉"},
								   {"holo",u8"幻象"},
								   {"loba",u8"罗芭"},
								   {"newcastle",u8"纽卡斯尔"},
								   {"conduit",u8"导线管"},
								   {"caustic",u8"侵蚀"},
								   {"wattson",u8"沃特森"},
								   {"rampart",u8"兰伯特"},
								   {"catalyst",u8"催化姬"}
};


//Overlay Need Var
inline HWND GameHwnd = 0;
inline DWORD ProcessId = 0;
inline RECT rcClient;



//Driver Need Var
inline DWORD64 GameBase = 0;

//Entity Need Var
inline float angleX[90];
inline DWORD64 LocalPlayer = 0;
inline DWORD64 ViewRender = 0;
inline DWORD64 ViewMatrix = 0;
inline int lastTeamID = 0;

inline std::vector<Entity> aEntitys;
inline std::vector<ProUser> Proplayers;
inline std::set<std::string> uniqueNicknames; // 唯一昵称集合，用于确保每个昵称只显示一次
inline std::set<int> uniquePROLevels;  // 用于存储已显示过的权限等级
inline std::set<std::string> uniquePROStatus;  // 用于存储已显示过的权限状态
inline std::vector<std::string> spectatorList; //观战的人

//Glow
struct GlowMode {
	BYTE GeneralGlowMode;
	BYTE BorderGlowMode;
	BYTE BorderSize;
	BYTE TransparentLevel;
};

enum GlowModeStyle :BYTE {
	GlowMode_Bloodhound = 12,
	GlowMode_Black = 75,
	GlowMode_Custom = 101,
	GlowMode_Sonar = 103,
	GlowMode_Flash = 117,
	GlowMode_Color = 118,
	GlowMode_Scan = 136,
	GlowMode_Digital = 134,
	GlowMode_Dynamic = 141,
};

inline float m_lastVisibleTime;


//AimBot Need Var
inline bool StartAimBot = false;
static inline Vector3 oldPunch = { 0,0,0 };
inline uint64_t aimPtr = 0;
inline int AimBot_FOV = 40;
inline int AimBot_Mode = 0;
inline int AimBot_Bones = 2;
inline float AimBot_Smooth = 30;
inline float Grenade_Smooth = 0;
inline int AimBot_Key1 = 0;
inline int AimBot_Key2 = 0;
inline int aimkey = VK_LBUTTON;
inline int aimkey1 = VK_LBUTTON;
inline int aimkey2 = VK_SHIFT;
inline int aimkey3;
inline int aimkey4;
inline int aimkey5;
inline int aimkey6;
inline int aimkey7;
inline int keystatus7;

inline int keystatus6;
inline int keystatus5;

inline int keystatus4;
inline int keystatus3;
inline int keystatus;
inline int keystatus1;
inline int keystatus2;
//Overlay Need Var
inline const char* GrapeWeapon[3]{ "Grenade", "Thermit", "Arc star" };
inline const char* itemGlows[5]{ "Lv1:White", "Lv2:Blue", "Lv3:Purple", "Lv4:Golden","Lv5:Red" };
inline int m_spectators = 0;
inline ImFont* myFont;
inline ImFont* largeFont;
inline ImFont* ico = nullptr;
inline ImFont* ico_2 = nullptr;
inline ImFont* ico_minimize = nullptr;
inline ImFont* tab_text;
inline ImFont* tab_text1;
inline ImFont* tab_text2;
inline ImFont* tab_text3;
inline ImFont* ico_logo;
inline ImFont* ico_subtab;
inline float slider_blur = 3.f;

namespace var {
	inline bool Security = false;
	inline bool Grenade = true;
	inline bool EnableStrfe = true;
	inline bool Autto_Grapple = true;
	inline bool RadbbitJump = true;
	inline bool RoppleJump = true;
	inline bool superGulide = false;
	inline bool FullMap = true;

	inline bool RandomBoneSelection=false;
	inline bool RandomPosition=false;
	inline float GrenadeSmooth = 0;
	inline bool AutoGrenade = true;
	inline bool AssistLine = true;

	inline int AimBotDistance =150;
	inline bool AimBotCheck= true;
	inline bool Knocked = true;
	inline bool EnableTrigger = true;
	inline bool EnableNoRecoli = true;
	inline int NoRecoliRate = 80;
	inline bool EnableAimBot = true;
	inline bool Enable_AimBot_FOV = true;
	inline bool EnablePlayerGlow = true;
	inline bool EnableNPCGlow = true;
	inline bool EnhanceGlow =true;
	inline bool EnableWeaponDisappare = true;
	inline int	 AimBot_FOV_Thickness = 1;
	inline int GlowModeSelect = 0;
	inline int Glow_Border_Size = 50;
	inline  float WeaponChams_color[4] = { 0 , 8 , 8 , 1.f };
	inline  float Visible_color[4] = { 1 , 1 , 1 , 1.f };
	inline float InVisible_color[4] = { 1 , 0 , 0 , 1.f };
	inline bool EnableItemGlow = false;
	inline  bool ItemGlowSelects[5] = { false, false, false, false,false };
	inline bool EnableEsp = true;
	inline bool EnableLevel = true;
	inline int EspDistance = 100;
	inline int SeerDistance = 80;
	inline bool EnableBones = false;
	inline bool Enable2Dbox = true;
	inline bool EnableName = false;
	inline bool EnableHero = false;
	inline bool EnableHealth = true;
	inline bool EnableShield = true;
	inline bool EnableTeamId = true;
	inline bool EnableDistance = true;
	inline bool EnablePRO = true;
	inline bool EnableWattcher = true;
	inline bool EnableUserID = false;
	inline int PROLevel = 0;



	inline bool boundbox1 = false;
	inline  bool boundbox = false;
	inline bool bindbox = false;

	inline float slider_float1 = 0.0f;
	inline  float slider_float2 = 1.0f;
	inline float slider_float3 = 90.0f;
	inline int selectedItem3 = 0;
	inline const char* items3_eng[] = { "Selected 0", "Selected 1", "Selected 2", "Selected 3" };
	inline bool multi_items_count[5];
	inline  const char* multi_items[5] = { "One", "Two", "Three", "Four", "Five" };
	inline float color_edit0[4] = { 0.70f, 0.80f, 0.90f, 1.000f };
	inline float color_edit1[4] = { 0.70f, 0.80f, 0.90f, 1.000f };
	inline float color_edit2[4] = { 218 / 255.f, 96 / 255.f, 21 / 255.f, 255 / 255.f };
	inline  float color_edit3[4] = { 0.70f, 0.80f, 0.90f, 1.000f };
	inline float color_edit4[4] = { 0.70f, 0.80f, 0.90f, 1.000f };

	inline static const char* items[]{ "Default", "Triangles", "3D Box" };
	inline int selectedItem = 0;
	inline static char input[64] = { "" };
	inline char input2[64] = { "" };
	inline int key0;
	inline int key1;
	inline int key2;
}


inline static float tab_alpha = 0.f;
inline static float tab_add;
inline static int active_tab = 0;
inline int tabs = 0;
inline int sub_tabs = 0;

inline bool menu = true;

inline bool espkkkk = true;
inline float dpi_scale = 1.0f;

inline bool animated_background=true;
inline int key0;
inline int key1;
inline float color_edit[4] = { 64 / 255.f, 77 / 255.f, 236 / 255.f, 190 / 255.f };
inline ID3D11ShaderResourceView* dr = nullptr;



inline ImColor main_color(218, 96, 21, 255);
inline ImVec4 color_particle(0.7f, 0.f, 0.f, 1.f);
inline ImColor background_color(24, 24, 24, 255);

inline ImVec4 second_color(0.09f, 0.09f, 0.09f, 1.f);


inline ImVec2 frame_size = ImVec2(605, 65);

inline float anim_speed = 8.f;

inline bool draw_grind;
inline static int combo = 0;
inline float pos_offset;
inline bool size_change;
inline ID3D11ShaderResourceView* bg = nullptr;
inline ID3D11ShaderResourceView* us = nullptr;
inline static int keybind = 0;
inline  static int iSubTabs = 0;
inline const char* themes[]{ "Head","Chest", "Legs", "Hands" };


//Config
inline bool Show_Menu = true;