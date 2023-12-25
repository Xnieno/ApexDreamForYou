////Miscellaneous
#define OFFSET_ENTITYLIST			0x1d71858 // cl_entitylist
#define OFFSET_LOCAL_ENT			0x211fac8 //.?AVC_GameMovement@@ + 0x8
#define OFFSET_NAME_LIST            0xc275bf0 // NameList
#define OFFSET_LEVEL_NAME           0x16966f0     //LevelName
#define OFFSET_GLOBAL_VARS          0x16961f0   //GlobalVars
#define OFFSET_INPUT_SYSTEM         0x17151c0 //InputSystem

////Entity
#define OFFSET_TEAM					0x037c //m_iTeamNum
#define OFFSET_HEALTH				0x036c //m_iHealth
#define OFFSET_SHIELD				0x01a0 //m_shieldHealth
#define OFFSET_SHIELD_TYPE			0x4634 //m_armorType
#define OFFSET_NAME					0x04b9 //m_iName
#define OFFSET_ABS_VELOCITY         0x0170 //m_vecAbsVelocity
#define OFFSET_VISIBLE_TIME         0x19c0 //CPlayer!lastVisibleTime
#define OFFSET_ZOOMING      		0x1c01 //m_bZooming
#define OFFSET_YAW                  0x226c - 0x8 //m_currentFramePlayer.m_ammoPoolCount - 0x8
#define OFFSET_LIFE_STATE			0x06c8  //m_lifeState, >0 = dead
#define OFFSET_BLEED_OUT_STATE		0x2710 //m_bleedoutState, >0 = knocked
#define OFFSET_TIME_BASE            0x20b8 //m_currentFramePlayer.timeBase
#define OFFSET_PLAT_FROM_USER_ID    0x2578 //m_platformUserId
#define OFFSET_MODEL_NAME           0x0030 // m_ModelName
#define OFFSET_MODEL_INDEX          0x00d8 //m_currentFrame.modelIndex
#define OFFSET_VIEW_MODELS          0x2d50 //m_hViewModels
#define OFFSET_SIGNIFIER_NAME       0x04b0 //m_iSignifierName

/////Aimbot
#define OFFSET_ORIGIN				0x017c //m_vecAbsOrigin
#define OFFSET_BONES				0x0e18 //m_nForceBone + 0x48
#define OFFSET_STUDIOHDR            0x1020 //CBaseAnimating!m_pStudioHdr
#define OFFSET_AIMPUNCH				0x2468 //m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
#define OFFSET_CAMERAPOS			0x1f00 //CPlayer!camera_origin
#define OFFSET_VIEWANGLES			0x2550 //m_ammoPoolCapacity - 0x14
#define OFFSET_BREATH_ANGLES		(OFFSET_VIEWANGLES - 0x10)
#define OFFSET_OBSERVER_MODE		0x34c4 //m_iObserverMode
#define OFFSET_OBSERVING_TARGET		0x34d0 //m_hObserverTarget
#define OFFSET_CROSSHAIR_LAST       (OFFSET_VISIBLE_TIME + 0x08) // CPlayer!lastCrosshairTargetTime
#define OFFSET_MATRIX				0x11a350 // ViewMatrix
#define OFFSET_RENDER				0x7389d40 //ViewRender

////Weapon
#define OFFSET_INVENTORY            0x18f8 //m_inventory
#define OFFSET_VIEW_OFFSET          0x00e8 //m_currentFrame.viewOffset
#define OFFSET_WEAPON				0x1964 //m_latestPrimaryWeapons
#define OFFSET_OFF_WEAPON           0x1974 //m_latestNonOffhandWeapons
#define OFFSET_WEAPON_NAME_INDEX    0x17a8 //[RecvTable.DT_WeaponX] m_weaponNameIndex
#define OFFSET_BULLET_SPEED         0x1ebc //CWeaponX!m_flProjectileSpeed
#define OFFSET_BULLET_SCALE         0x1ec4 //CWeaponX!m_flProjectileScale
#define OFFSET_ZOOM_FOV             0x1600 + 0x00b8 //m_playerData + m_curZoomFOV
#define OFFSET_AMMO                 0x15b0 //[RecvTable.DT_WeaponX_LocalWeaponData].m_ammoInClip
#define OFFSET_SEMI_AUTO            0x1b6c //m_isSemiAuto
#define OFFSET_LAST_CHARGE_LEVEL    0x1710 // m_lastChargeLevel
#define OFFSET_NEXT_READY_TIME      0x1588 //m_nextReadyTime
#define OFFSET_NEXT_PRIMARY_ATTACK_TIME  0x158c //m_nextPrimaryAttackTime
#define OFFSET_SHOT_COUNT           0x1724 //m_shotCount
#define OFFSET_ZOOM_TIME_IN         0x0da0 //zoom_time_in
#define mouse_sensitivity         0x0210ca60

////Item
#define OFFSET_ITEM_ID             0x1588 //m_customScriptInt

////Highlight
#define OFFSET_HIGHLIGHT_SETTING 0xb5cc530
#define OFFSET_HIGHLIGHT_CURRENT_CONTEXT_ID 0x0294
#define OFFSET_HIGHLIGHT_SERVER_ACTIVE_STATES 0x0298
#define OFFSET_HIGHLIGHT_VISIBILITY_TYPE   0x278
#define OFFSET_HIGHLIGHT_DISTANCE   0x26c

///Glow
#define OFFSET_GLOW_DISTANCE		0x26c  // //OK Script_Highlight_SetFarFadeDist / m_highlightServerFadeEndTimes + 52(0x34)
#define OFFSET_GLOW_COLOR			0x1D0 + 0x30 // m_highlightParams + 0x18
#define OFFSET_GLOW_MODE			0x29c  // m_highlightFunctionBits + 0x4
#define OFFSET_GLOW_ENABLE			0x29b0
#define OFFSET_GLOW_THROUGH_WALLS	0x278

////Silent
#define OFFSET_PCOMMANDS 0x2102b60 //pCommands
#define OFFSET_LATEST_COMMAND_NUMBER 0x16a91cc //LatestCommandNumber
#define OFFSET_NET_CHANNEL 0x1690510 //NetChannel

////Scripts
#define OFFSET_FLAGS 0x00c8 //m_fFlags
#define OFFSET_IN_ATTACK    0x0738af90  //in_attack
#define OFFSET_IN_JUMP    0x0738b0a0 //in_jump
#define OFFSET_IN_FORWARD 0x07383d30 //in_forward
#define OFFSET_IN_DUCK 0x0738b1d0 //in_duck
#define OFFSET_IN_RELOAD 0x0738b000 //in_reload

#define OFFSET_IS_SKYDIVING 0x4690 //m_skydiveState
#define OFFSET_GRAPPLE 0x2c78 //[RecvTable.DT_Player].m_grapple
#define OFFSET_GRAPPLE_ATTACHED 0x0048//m_grappleAttached

#define OFFSET_FPS_MAX 0x015cd8f0 //fps_max
#define OFFSET_TRAVERSAL_PROGRESS 0x2b2c //m_traversalProgress
#define OFFSET_TRAVERSAL_START_TIME 0x2b30 //m_traversalStartTime
#define OFFSET_TRAVERSAL_RELEASE_TIME 0x2b38 //m_traversalReleaseTime
#define OFFSET_CURRENT_FRAME 0x20b8 //m_currentFramePlayer

#define m_vecMaxs (0x03f0 + 0x001c) // 我不是教过你这里是什么吗

#define m_xp       0x369c

#define INCHES_TO_METERS 0.0254

#define OFFSET_IOBSERVERMODE 0x34c4 //m_iObserverMode 7代表着无人机
