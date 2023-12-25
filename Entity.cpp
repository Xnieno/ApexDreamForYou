#include"Entity.h"
#include"Driver.h"
#include "Globals.h"
#include "Offsets.h"
#include "Externs.h"
#include "Utils.h"
#include<iostream>
#include <thread>

#include "PRO.h"
void EntityLoop()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        //Sleep(300);//每300ms更新一次实体列表,降低CPU开销
        LocalPlayer = Read<__int64>(ProcessId, GameBase + OFFSET_LOCAL_ENT);
        ViewRender = Read<__int64>(ProcessId, GameBase + OFFSET_RENDER);
        ViewMatrix = Read<__int64>(ProcessId, ViewRender + OFFSET_MATRIX);
        
        std::string m_levelName = "";
        char LevelName[64]= { 0 };
        ReadMemory(ProcessId, GameBase + OFFSET_LEVEL_NAME, LevelName, 64);
        m_levelName= LevelName;
     

        int LocalTeam = Read<int>(ProcessId, LocalPlayer + OFFSET_TEAM);
        if (LocalTeam > 1)
            lastTeamID = LocalTeam;

        std::vector<Entity> Entitys;
        std::vector<ProUser> ProEntitys;
        std::vector<std::string> m_spectatorList;//观战
      
        int isum = 0;
        int StackNum = 2;
        // 自动检测是否在靶场中，如果在靶场中循环512个Stack(16384次)，如果不在则只循环2个Stack(64次)
        if(m_levelName.compare("mp_rr_canyonlands_staging_mu1") == 0) StackNum = 400;  //训练场
        if (m_levelName.compare("mp_lobby")==0) StackNum = 0;
        if (var::EnableItemGlow)
            StackNum = 400;
    

   
        uint64_t highlightSettingsPtr = Read<uint64_t>(ProcessId, GameBase + OFFSET_HIGHLIGHT_SETTING);

       
        for (int u = 0; u < StackNum; u++)
        {
            __int64 EntityStack[128];
            ReadMemory(ProcessId, GameBase + OFFSET_ENTITYLIST + (u << 10), EntityStack, sizeof(EntityStack));
            for (int i = 0; i < 32; i++)
            {
                if (EntityStack[i << 2] <= 0) continue;

                __int64 Ptr = EntityStack[i << 2];
                

                int ContextId = Read<int>(ProcessId, (long long)Ptr + OFFSET_GLOW_ENABLE);
                uint8_t settingIndex = Read<uint8_t>(ProcessId, Ptr + ContextId + OFFSET_HIGHLIGHT_SERVER_ACTIVE_STATES);
                if (var::ItemGlowSelects[settingIndex - 32] == true && var::EnableItemGlow) {
                    Write<GlowMode>(ProcessId, highlightSettingsPtr + 0x28 * settingIndex + 4, { 137,138,64,64 });//物资样式
                }

                if (Ptr == LocalPlayer) continue;
                if (IsExistInEntityList(Entitys, Ptr)) continue;

                int Team = Read<int>(ProcessId, Ptr + OFFSET_TEAM);
                if (Team <= 0) continue;
                if (LocalTeam == Team) continue;

                int Health = Read<int>(ProcessId, Ptr + OFFSET_HEALTH);
                if (Health <= 0) continue;
                int Sheild = Read<int>(ProcessId, Ptr + OFFSET_SHIELD);
                int SheildType = Read<int>(ProcessId, Ptr + OFFSET_SHIELD_TYPE);
                uint64_t Uid = Read<uint64_t>(ProcessId, Ptr + OFFSET_PLAT_FROM_USER_ID);
                char ModelName[128] = { 0 };
                ReadMemory(ProcessId, Read<ULONG64>(ProcessId, Ptr + OFFSET_MODEL_NAME), ModelName, 128);
                bool isBloodHound = (strstr(ModelName, "bloodhound") != NULL); //寻血猎犬的骨骼特殊处理。
                char class_name[33] = { 0 };
                get_class_name(Ptr, class_name);
              
                bool Status = IsDead(Ptr);
               
                if (calculateAngleDifference(getYaw(LocalPlayer), getYaw(Ptr)) <= 1.f && IsDead(Ptr))
                {
                    m_spectators++;
                    m_spectatorList.push_back(GetPlayerNickname(Ptr));
                }

                std::string HeroName = "";
                std::string ModelNameString(ModelName);
                for (const auto& pair : ModelNameDic) {
                    if (ModelNameString.find(pair.first) != std::string::npos) {
                        HeroName = pair.second;
                        break;
                    }
                }
                bool isPro = false;
                if (checkIsPro(Uid)){
                    isPro = true;
                }

                int Level=getProLevel(Uid);

                Entity newEntity{};
                ProUser Pro{};
                newEntity.Ptr = Ptr;
                newEntity.Team = Team;
                newEntity.Health = Health;
                newEntity.Shield = Sheild;
                newEntity.ShieldType = SheildType;
                newEntity.UserId = Uid;
                newEntity.isPro = isPro;
                newEntity.PROLevel = Level;
                Pro.PROLevel= Level;
                Pro.isPro = isPro;
                const char StatusString1[80] = u8"死亡";
                const char StatusString2[80] = u8"存活";
                if (Status) {
                    strncpy_s(Pro.Status, StatusString1, 80);
                }
                else {
                    strncpy_s(Pro.Status, StatusString2, 80);
                }
                

                strncpy_s(newEntity.class_name, sizeof(newEntity.class_name), class_name, _TRUNCATE);
                strcpy_s(newEntity.ModelName, sizeof(newEntity.ModelName), HeroName.c_str());
                ULONG EntityIndex = Read<ULONG>(ProcessId, Ptr + 0x38);
                ReadMemory(ProcessId, Read<ULONG64>(ProcessId, GameBase + OFFSET_NAME_LIST + ((EntityIndex - 1) << 4)), newEntity.nickName, 256);
                {
                    ULONG64 Model = Read<ULONG64>(ProcessId, Ptr + OFFSET_STUDIOHDR);
                    ULONG64 StudioHdr = Read<ULONG64>(ProcessId, Model + 0x8);
                    USHORT HitboxCache = Read<USHORT>(ProcessId, StudioHdr + 0x34);
                    ULONG64 HitboxArray = StudioHdr + ((USHORT)(HitboxCache & 0xFFFE) << (4 * (HitboxCache & 1)));
                    USHORT IndexCache = Read<USHORT>(ProcessId, HitboxArray + 4);
                    INT HitboxIndex = ((USHORT)(IndexCache & 0xFFFE) << (4 * (IndexCache & 1)));

                    USHORT boneIndices[512] = { 0 };
                    ReadMemory(ProcessId, HitboxIndex + HitboxArray, boneIndices, sizeof(boneIndices));



                    newEntity.Bones.Head = boneIndices[0];
                    newEntity.Bones.Neck = boneIndices[16];
                    newEntity.Bones.Hip = boneIndices[48];

                    newEntity.Bones.leftElbow = boneIndices[112];
                    newEntity.Bones.rightElbow = boneIndices[isBloodHound ? 144 : 160]; // 9 10

                    newEntity.Bones.leftHand = boneIndices[isBloodHound ? 304 : 128]; // 19 8
                    newEntity.Bones.rightHand = boneIndices[isBloodHound ? 160 : 176];// 10 11

                    newEntity.Bones.leftKnee = boneIndices[isBloodHound ? 192 : 208];// 12 13
                    newEntity.Bones.rightKnee = boneIndices[isBloodHound ? 256 : 272];// 16 17

                    newEntity.Bones.leftHeel = boneIndices[isBloodHound ? 208 : 224];//13 14
                    newEntity.Bones.rightHeel = boneIndices[isBloodHound ? 272 : 288];// 17 18

                    __int64 BoneArray = Read<__int64>(ProcessId, Ptr + OFFSET_BONES);
                    newEntity.BoneArray = BoneArray;
                }

                if (isPro) {
                    const size_t MAX_SIZE = 256;
                    // 拷贝 nickName 到 destination，确保不超过 MAX_SIZE
                    strncpy_s(Pro.nickName, newEntity.nickName, MAX_SIZE);
                    // 由于 strncpy 不自动添加 null 字符，需要手动确保字符串结尾
                    Pro.nickName[MAX_SIZE - 1] = '\0';
                }
            

                ProEntitys.push_back(Pro);
                Entitys.push_back(newEntity);
              

            }
        }

        //同步到全局实体列表中
        aEntitys.assign(Entitys.begin(), Entitys.end());
        Proplayers.assign(ProEntitys.begin(), ProEntitys.end());
        spectatorList.assign(m_spectatorList.begin(), m_spectatorList.end());
    }
   
}


