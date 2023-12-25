#pragma once
#include <vector>

#include "Externs.h"

bool IsExistInEntityList(const std::vector<Entity>& players, uint64_t ptr);
Vector3 GetBonePosition(__int64 BoneArray, Vector3 NowPos, int ID);
bool WorldtoScreen2D(float matrix[4][4], Vector3 EntityPos, Vector2& Screen);
Vector3 move_away_from_target(const Vector3& source, const Vector3& target, float distance);
Vector3 move_and_project_vector3(const Vector3& source, const Vector3& target, float distance);
bool is_point_within_distance_to_segment(const Vector2& point, const Vector2& A, const Vector2& B, float distance);
bool IsDead(ULONG64 Ptr);

bool IsKnocked(ULONG64 Ptr);
Vector3 GetHitBoxPosition(DWORD64 Ptr, INT ID);
Vector2 CalcAngle(Vector3 src, Vector3 dst);
Vector3 PredictPos(Vector3 NowPos, Vector3 NowVelocity, FLOAT Distance, FLOAT BulletSpeed, FLOAT BulletGravity, ULONG64 WeaponID);
ULONG64 GetEntity(UINT index);
int getLevel(int xp);
void get_class_name(uint64_t entity_ptr, char* out_str);

bool isDummy(uint64_t entity_ptr);
bool isPlayer(uint64_t entity_ptr);

std::string GetPlayerNickname(__int64 ptr);

float getYaw(__int64 ptr);
float calculateAngleDifference(float angle1, float angle2);
BYTE GlowSetSelect(int id);
float getLastVisibleTime(ULONG64 Ptr);
bool isVisible(ULONG64 Ptr);
void HotkeyButton(int aimkey, void* changekey, int status);
void ChangeKey(void* blank);
void ChangeKey1(void* blank);
void ChangeKey2(void* blank);
void ChangeKey3(void* blank);

void ChangeKey4(void* blank);
void ChangeKey5(void* blank);
void ChangeKey6(void* blank);
void ChangeKey7(void* blank);
void Grenade();
void initAngleX();
