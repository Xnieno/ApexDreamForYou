#pragma once
#include <Windows.h>
#include <math.h>

//定义玩家结构
struct Bones {
	USHORT Head;
	USHORT Neck;
	USHORT Hip;
	USHORT leftElbow, rightElbow;
	USHORT leftHand, rightHand;
	USHORT leftKnee, rightKnee;
	USHORT leftHeel, rightHeel;
};


struct Vector3
{
	float x, y, z;
	Vector3 operator-(Vector3 v) const { return { x - v.x, y - v.y, z - v.z }; }
	Vector3 operator+(Vector3 v) const { return { x + v.x, y + v.y, z + v.z }; }
	Vector3 operator*(float s) const { return { x * s, y * s, z * s }; }
	Vector3 operator/(float s) const { return { x / s, y / s, z / s }; }
	Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
	Vector3& operator+=(Vector3 v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(Vector3 v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	void Normalize() { while (x > 89.0f) x -= 180.f; while (x < -89.0f) x += 180.f; while (y > 180.f) y -= 360.f; while (y < -180.f) y += 360.f; }
	float Length() const { return sqrt(x * x + y * y + z * z); }
	float Length2D() const { return sqrt(x * x + y * y); }
	float DistTo(Vector3 v) const { return (*this - v).Length(); }
	float Dist2D(Vector3 v) const { return (*this - v).Length2D(); }
};


struct Vector2
{
	float x, y;
	Vector2 operator-(Vector2 v) const { return { x - v.x, y - v.y }; }
	Vector2 operator+(Vector2 v) const { return { x + v.x, y + v.y }; }
	Vector2 operator*(float s) const { return { x * s, y * s }; }
	Vector2 operator/(float s) const { return { x / s, y / s }; }
	Vector2& operator/=(float s) { x /= s; y /= s; return *this; }
	Vector2& operator+=(Vector2 v) { x += v.x; y += v.y; return *this; }
	Vector2& operator-=(Vector2 v) { x -= v.x; y -= v.y; return *this; }
	bool operator!=(Vector2 v) const { return x != v.x || y != v.y; }
	void Normalize() { if (x > 89.0f) x -= 180.f; if (x < -89.0f) x += 180.f; if (y > 180.f) y -= 360.f; if (y < -180.f) y += 360.f; }
	float Length2D() const { return sqrt(x * x + y * y); }
	float Dist2D(Vector2 v) const { return (*this - v).Length2D(); }
};



struct Bones2D {
	Vector2 Head;
	Vector2 Neck;
	Vector2 Chest;
	Vector2 Hip;

	Vector2 leftElbow, rightElbow;
	Vector2 leftHand, rightHand;
	Vector2 leftKnee, rightKnee;
	Vector2 leftHeel, rightHeel;
};


//Entity
struct Entity
{
	__int64 Ptr;
	int Team;
	int Shield;
	int ShieldType;
	int PROLevel;
	Bones Bones;
	Bones2D Bones2D;
	__int64 BoneArray;
	int Health;
	char ModelName[128];
	char nickName[256];
	char class_name[33] ;
	bool isPro;
	uint64_t UserId;
};

struct ClientClass {
	uint64_t pCreateFn;
	uint64_t pCreateEventFn;
	uint64_t pNetworkName;
	uint64_t pRecvTable;
	uint64_t pNext;
	uint32_t ClassID;
	uint32_t ClassSize;
};

struct ProUser {
	bool isPro;
	char nickName[256];
	int PROLevel;
	char Status [120];
};
