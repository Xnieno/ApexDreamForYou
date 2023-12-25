#pragma once
#include <cstdint>
#include "Driver.h"
#include "Globals.h"
#include "Offsets.h"

class HighlightManager {
private:
    uint64_t gameBase;
    uint64_t entity;
    int contextId;
    uint8_t settingIndex;
    BYTE GlowStyle;
    uint64_t highlightSettingsPtr;
    bool isHighlightSettingsPtrUpdate=false;

public:
    HighlightManager(uint64_t gameBase, uint64_t entity, int contextId, uint8_t settingIndex, int GlowStyle)
        : gameBase(gameBase), entity(entity), contextId(contextId), settingIndex(settingIndex), GlowStyle(GlowStyle) {}
    HighlightManager(uint64_t gameBase, uint64_t entity) : HighlightManager(gameBase, entity, 0, 0, 0) {}
    void SetContextId(int newContextId) {
        contextId = newContextId;
    }

    void SetSettingIndex(uint8_t newSettingIndex) {
        settingIndex = newSettingIndex;
    }

    void SetGlowStyle(int newGlowStyle) {
        GlowStyle = newGlowStyle;
    }

    void SetEntity(uint64_t newEntity) {
        entity = newEntity;
    }

    void ApplyHighlightSettingsVisible() {
        applyHighlightSettings(var::Visible_color, var::EnhanceGlow);
    }

    void ApplyHighlightSettingsInVisible() {
        applyHighlightSettings(var::InVisible_color, var::EnhanceGlow);
    }

    void ApplyDefaultHighlightSettings(float* color) {
        applyHighlightSettings(color, false); // 默认情况下不增强Glow
    }

    void ApplyHightlightSettingsPtrUpdate() {
        applyHightlightSettingsPtrUpdate();//更新Ptr，直到GameBase更新
    }

    void ApplyItemGlow(__int64 Ptr) {
        applyItemGlow(Ptr);//物资透视
    }

private:
    void applyHighlightSettings(float* color, bool enhanceGlow) {
        Write<int>(ProcessId, entity + OFFSET_GLOW_THROUGH_WALLS, 1); // 1表示穿墙
        Write<int>(ProcessId, entity + OFFSET_HIGHLIGHT_CURRENT_CONTEXT_ID, contextId); // 上下文id
        Write<uint8_t>(ProcessId, entity + contextId + OFFSET_HIGHLIGHT_SERVER_ACTIVE_STATES, settingIndex);
        Write<GlowMode>(ProcessId, highlightSettingsPtr + 0x28 * settingIndex + 4, { GlowStyle, 125, static_cast<BYTE>(var::Glow_Border_Size), 64 });
        for (int i = 0; i < 3; i++) {
            float value = enhanceGlow ? color[i] * 255 : color[i];
            Write<float>(ProcessId, highlightSettingsPtr + 0x28 * settingIndex + 8 + i * 4, value);
        }
    }

    void applyHightlightSettingsPtrUpdate() {
        //减少Ptr的计算，只计算一次，直到GameBase发生更新
        static uint64_t lastGameBase = 0;
        if (gameBase != lastGameBase&& !isHighlightSettingsPtrUpdate) {
            highlightSettingsPtr = Read<uint64_t>(ProcessId, gameBase + OFFSET_HIGHLIGHT_SETTING);
            lastGameBase = gameBase;
            isHighlightSettingsPtrUpdate = true;
        }
    }

    void applyItemGlow(__int64 Ptr) {
        int ContextId = Read<int>(ProcessId, (long long)Ptr + OFFSET_GLOW_ENABLE);
        uint8_t settingIndex = Read<uint8_t>(ProcessId, Ptr + ContextId + OFFSET_HIGHLIGHT_SERVER_ACTIVE_STATES);
        if (var::EnableItemGlow) {
            if (var::ItemGlowSelects[settingIndex - 32] == true) {
                Write<GlowMode>(ProcessId, highlightSettingsPtr + 0x28 * settingIndex + 4, { 137,138,64,64 });//物资样式
            }
        }
    }
};
