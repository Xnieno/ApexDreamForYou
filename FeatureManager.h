#pragma once
#include <vector>
#include "HighlightManager.h"
#include "Aimbot.h"
#include "Globals.h"
#include "Offsets.h"
#include "Externs.h"


extern std::vector<Entity> aEntitys;

class FeatureManager {
private:
    Aimbot aimbot;
 
    HighlightManager highlightManager;

public:
    FeatureManager() : highlightManager(GameBase, 0) {}

    void update() {
        std::vector<Entity> localEntityList = aEntitys;
         aimbot.update();

        updateEntityHighlights(localEntityList);
    }

private:
    void updateEntityHighlights(std::vector<Entity>& entities) {
        //if (var::EnableWeaponDisappare) {
        //    // highlightManager.ApplyWeaponChams(); // 确保HighlightManager有这个方法
        //}


        for (auto& entity : entities) {
            if (entity.Ptr == 0) continue;
            bool Visible = isVisible(entity.Ptr);

            int LocalTeam = Read<int>(ProcessId, LocalPlayer + OFFSET_TEAM);
            if (lastTeamID != LocalTeam)
                continue;
            int Team = Read<int>(ProcessId, entity.Ptr + OFFSET_TEAM);
            if (isDummy(entity.Ptr) && Team != lastTeamID) {
                if (var::EnableNPCGlow) {
                    applyEntityHighlight(entity.Ptr, Visible, Visible ? 64 : 65);
                }
            }
            else if (isPlayer(entity.Ptr) && var::EnablePlayerGlow && Team != lastTeamID) {
                applyEntityHighlight(entity.Ptr, Visible, Visible ? 66 : 67);
            }
            
        }
    }

    void applyEntityHighlight(uint64_t entity, bool isVisible, uint8_t settingIndex) {
        BYTE glowStyle = GlowSetSelect(var::GlowModeSelect);

        highlightManager.ApplyHightlightSettingsPtrUpdate();
        highlightManager.SetEntity(entity);
        highlightManager.SetContextId(1);
        highlightManager.SetSettingIndex(settingIndex);
        highlightManager.SetGlowStyle(glowStyle);

        if (isVisible) {
            highlightManager.ApplyHighlightSettingsVisible();
        }
        else if (!isVisible) {
            highlightManager.ApplyHighlightSettingsInVisible();
        }
        
    }
};
