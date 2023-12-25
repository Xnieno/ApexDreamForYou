#include <iostream>
#include<fstream>
#include<string>
#include<map>
#include"Imgui/imgui.h"
#include "Utils.h"
#include "Globals.h"
#include "Offsets.h"
#include "Driver.h"


//查看实体是否已经在列表中存在了，要不然有可能因为游戏的BUG遍历到重复的实体（游戏是有可能在实体列表中重复给出同一个实体的数据的）
bool IsExistInEntityList(const std::vector<Entity>& players, uint64_t ptr) {
    for (const auto& player : players) if (player.Ptr == ptr) return true;
    return false;
}

//通过骨骼序列，骨骼ID和当前的根坐标来获取对应的骨骼坐标，一般你是用不到这个的。
Vector3 GetBonePosition(__int64 BoneArray, Vector3 NowPos, int ID) {
    float Matrix[3][4] = { 0 };
    ReadMemory(ProcessId, BoneArray + ID * 0x30, (PVOID)Matrix, 48);
    return { Matrix[0][3] + NowPos.x, Matrix[1][3] + NowPos.y, Matrix[2][3] + NowPos.z };
}

//世界坐标到屏幕坐标，需要知道准星坐标
bool WorldtoScreen2D(float matrix[4][4], Vector3 EntityPos, Vector2& Screen) {
    float w = matrix[3][0] * EntityPos.x + matrix[3][1] * EntityPos.y + matrix[3][2] * EntityPos.z + matrix[3][3];
    if (w < 0.1f) return false;
    w = 1.0f / w;
    float x = matrix[0][0] * EntityPos.x + matrix[0][1] * EntityPos.y + matrix[0][2] * EntityPos.z + matrix[0][3];
    float y = matrix[1][0] * EntityPos.x + matrix[1][1] * EntityPos.y + matrix[1][2] * EntityPos.z + matrix[1][3];
    Vector2 SightPos = { (rcClient.right - rcClient.left) / 2, (rcClient.bottom - rcClient.top) / 2 };
    Screen.x = (SightPos.x + x * w * SightPos.x);
    Screen.y = (SightPos.y - y * w * SightPos.y);
    return true;
}

// 输入两个点坐标和一个距离，计算将一个点沿两点连线远离另一个点后的坐标。
Vector3 move_away_from_target(const Vector3& source, const Vector3& target, float distance) {
    Vector3 direction = { source.x - target.x, source.y - target.y, source.z - target.z };
    float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

    // 如果原点和目标点重合，不进行移动
    if (magnitude == 0.0) return source;

    // 归一化反方向向量
    direction.x /= magnitude;
    direction.y /= magnitude;
    direction.z /= magnitude;

    // 沿归一化的反方向向量移动指定的距离
    Vector3 moved_point;
    moved_point.x = source.x + direction.x * distance;
    moved_point.y = source.y + direction.y * distance;
    moved_point.z = source.z + direction.z * distance;

    return moved_point;
}

//输入两个点坐标和一个距离，计算将一个点在另一个点的方向上做投影，然后在投影所构成的X-Y平面上移动距离后点的坐标。
Vector3 move_and_project_vector3(const Vector3& source, const Vector3& target, float distance) {
    float xy_distance = sqrt(pow(target.x - source.x, 2) + pow(target.y - source.y, 2));

    // 如果原点和目标点在XY平面上重合，不进行移动
    if (xy_distance == 0.0) {
        return source;
    }

    // 计算方向向量并归一化
    Vector3 direction;
    direction.x = (target.x - source.x) / xy_distance;
    direction.y = (target.y - source.y) / xy_distance;
    direction.z = 0; // 这是一个XY平面的移动，所以Z分量为0

    // 将源点沿方向向量移动指定的距离
    Vector3 moved_point;
    moved_point.x = source.x + direction.x * distance;
    moved_point.y = source.y + direction.y * distance;
    moved_point.z = source.z; // 在投影时，保留目标点的Z坐标

    return moved_point;
}

// 计算在给定二维平面上点与直线的距离
bool is_point_within_distance_to_segment(const Vector2& point, const Vector2& A, const Vector2& B, float distance) {
    Vector2 AB{ B.x - A.x, B.y - A.y };
    Vector2 AP{ point.x - A.x, point.y - A.y };

    float AB_length = sqrt(AB.x * AB.x + AB.y * AB.y);

    // 如果AB长度为0，即A和B重合
    if (AB_length == 0) {
        // 此时线段退化为点A，计算AP长度并与给定距离进行比较
        float AP_length = sqrt(AP.x * AP.x + AP.y * AP.y);
        return AP_length <= distance;
    }

    Vector2 AP_projected{ AP.x - (AP.x * AB.x + AP.y * AB.y) / (AB_length * AB_length) * AB.x, AP.y - (AP.x * AB.x + AP.y * AB.y) / (AB_length * AB_length) * AB.y };
    float distance_to_segment = sqrt(AP_projected.x * AP_projected.x + AP_projected.y * AP_projected.y);

    // 如果距离小于或等于给定距离，返回true
    if (distance_to_segment <= distance) {
        // 检查投影点是否在线段上
        float t = (AP.x * AB.x + AP.y * AB.y) / (AB_length * AB_length);
        return t >= 0 && t <= 1;
    }

    // 否则，返回false
    return false;
}

bool IsDead(ULONG64 Ptr) {
    return Read<INT>(ProcessId, Ptr + OFFSET_LIFE_STATE) > 0;
}




 float getLastVisibleTime(ULONG64 Ptr)
{
    return Read<float>(ProcessId,Ptr + OFFSET_VISIBLE_TIME);
}


bool isVisible(ULONG64 Ptr)
 {

     float currentTime = Read<float>(ProcessId,GameBase + OFFSET_GLOBAL_VARS + 0x28);
     float lastVisTime = getLastVisibleTime(Ptr);
     return currentTime - lastVisTime < 0.2f;
 }



bool IsKnocked(ULONG64 Ptr) {
    return Read<INT>(ProcessId, Ptr + OFFSET_BLEED_OUT_STATE) > 0;
}


Vector3 GetHitBoxPosition(DWORD64 Ptr, INT ID) {
    Vector3 PosTemp = Read<Vector3>(ProcessId, Ptr + OFFSET_ORIGIN);
    ULONG64 BoneArray = Read<ULONG64>(ProcessId, Ptr + OFFSET_BONES);
    ULONG64 Model = Read<ULONG64>(ProcessId, Ptr + OFFSET_STUDIOHDR);
    ULONG64 StudioHdr = Read<ULONG64>(ProcessId, Model + 0x8);
    USHORT HitboxCache = Read<USHORT>(ProcessId, StudioHdr + 0x34);
    ULONG64 HitboxArray = StudioHdr + ((USHORT)(HitboxCache & 0xFFFE) << (4 * (HitboxCache & 1)));
    USHORT IndexCache = Read<USHORT>(ProcessId, HitboxArray + 4);
    INT HitboxIndex = ((USHORT)(IndexCache & 0xFFFE) << (4 * (IndexCache & 1)));
    USHORT Bone = Read<USHORT>(ProcessId, HitboxIndex + HitboxArray + (ID * 0x20));
    FLOAT Matrix[3][4] = { 0 };
    ReadMemory(ProcessId, BoneArray + Bone * 0x30, (PVOID)Matrix, 48);
    return { Matrix[0][3] + PosTemp.x, Matrix[1][3] + PosTemp.y, Matrix[2][3] + PosTemp.z };
}
Vector2 CalcAngle(Vector3 src, Vector3 dst)
{
    Vector2 angle = { 0 };
    Vector3 delta = (src - dst);
    double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
    angle.x = atan(delta.z / hyp) * (180.0f / 3.1415926f);
    angle.y = atan(delta.y / delta.x) * (180.0f / 3.1415926f);
    if (delta.x >= 0.0) angle.y += 180.0f;
    return angle;
}
Vector3 PredictPos(Vector3 NowPos, Vector3 NowVelocity, FLOAT Distance, FLOAT BulletSpeed, FLOAT BulletGravity, ULONG64 WeaponID) {
    if (BulletSpeed == 1.0f) return NowPos;
    if (BulletSpeed == 10000.0f) BulletSpeed += pow(Read<UINT>(ProcessId, WeaponID + OFFSET_LAST_CHARGE_LEVEL), 5.468438819581);
    FLOAT FlyTime = Distance / BulletSpeed;
    Vector3 NewPos = { 0 };
    NewPos = NowPos + NowVelocity * FlyTime;
    NewPos.z += 350.f * BulletGravity * FlyTime * FlyTime;
    return NewPos;
}
ULONG64 GetEntity(UINT index) {
    if (index < 0 || index > 65536) return 0;
    ULONG64 base_entity = Read<ULONG64>(ProcessId, GameBase + OFFSET_ENTITYLIST);
    if (base_entity == 0) return 0;
    return Read<ULONG64>(ProcessId, GameBase + OFFSET_ENTITYLIST + (index << 5));
}
int levels[] = { 2750, 6650, 11400, 17000, 23350, 30450, 38300, 46450, 55050, 64100, 73600, 83550, 93950, 104800, 116100, 127850, 140050, 152400, 164900, 177550, 190350, 203300, 216400, 229650, 243050, 256600, 270300, 284150, 298150, 312300, 326600, 341050, 355650, 370400, 385300, 400350, 415550, 430900, 446400, 462050, 477850, 493800, 509900, 526150, 542550, 559100, 575800, 592650, 609650, 626800, 644100, 661550, 679150, 696900, 714800 };
int getLevel(int xp) {
    if (xp < 0) return 0;
    if (xp < 100) return 1;

    int level = 56;
    int arraySize = sizeof(levels) / sizeof(levels[0]);

    for (int i = 0; i < arraySize; i++) {
        if (xp < levels[i]) {
            return i + 1;
        }
    }

    return level + ((xp - levels[arraySize - 1] + 1) / 18000);
}

void get_class_name(uint64_t entity_ptr, char* out_str)
{
    uint64_t client_networkable_vtable = Read<uint64_t>(ProcessId,entity_ptr + 8 * 3);

    uint64_t get_client_class =Read<uint64_t>(ProcessId, client_networkable_vtable + 8 * 3);

    uint32_t disp = Read<uint32_t>(ProcessId, get_client_class + 3);
    const uint64_t client_class_ptr = get_client_class + disp + 7;

    ClientClass client_class = Read<ClientClass>(ProcessId, client_class_ptr);

    ReadMemory(ProcessId, client_class.pNetworkName, out_str, 32);

}

bool isDummy(uint64_t entity_ptr)
{
    char class_name[33] = {};
    get_class_name(entity_ptr, class_name);

    return strncmp(class_name, "CAI_BaseNPC", 11) == 0;
}

bool isPlayer(uint64_t entity_ptr)
{
    char class_name[33] = { 0 };
    get_class_name(entity_ptr, class_name);
    return strncmp(class_name, "CPlayer", 11) == 0;
}


//获取player的name
std::string GetPlayerNickname(__int64 ptr) {
    // 获取实体索引
    ULONG entityIndex = Read<ULONG>(ProcessId, ptr + 0x38);

    // 计算名称列表中的偏移量
    ULONG64 nameOffset = (entityIndex - 1) << 4;

    // 读取名称的内存地址
    ULONG64 nameAddress = Read<ULONG64>(ProcessId, GameBase + OFFSET_NAME_LIST + nameOffset);

    // 读取昵称
    char nickname[256] = { 0 };
    ReadMemory(ProcessId, nameAddress, nickname, sizeof(nickname));

    // 返回昵称字符串
    return std::string(nickname);
}

float getYaw(__int64 ptr)
{
    return (float)(ptr + OFFSET_YAW);
}


float calculateAngleDifference(float angle1, float angle2)
{
    float diff = angle1 - angle2;

    if (diff > 180) {
        diff -= 360;
    }
    else if (diff < -180) {
        diff += 360;
    }

    if (diff < 0) {
        diff *= -1;
    }

    return diff;
}



BYTE GlowSetSelect(int id)
{
    BYTE glowMode;

    switch (id)
    {
    case 0:
        glowMode = GlowMode_Bloodhound;
        break;
    case 1:
        glowMode = GlowMode_Black;
        break;
    case 2:
        glowMode = GlowMode_Custom;
        break;
    case 3:
        glowMode = GlowMode_Sonar;
        break;
    case 4:
        glowMode = GlowMode_Flash;
        break;
    case 5:
        glowMode = GlowMode_Color;
        break;
    case 6:
        glowMode = GlowMode_Scan;
        break;
    case 7:
        glowMode = GlowMode_Digital;
        break;
    case 8:
        glowMode = GlowMode_Dynamic;
        break;
    default:
        // 处理无效的 id，你可以选择返回默认值或者抛出异常等
        glowMode = 0;
        break;
    }

    return glowMode;
}


static const char* keyNames[] = {
    "",
    "Left Mouse",
    "Right Mouse",
    "Cancel",
    "Middle Mouse",
    "Mouse 5",
    "Mouse 4",
    "",
    "Backspace",
    "Tab",
    "",
    "",
    "Clear",
    "Enter",
    "",
    "",
    "Shift",
    "Control",
    "Alt",
    "Pause",
    "Caps",
    "",
    "",
    "",
    "",
    "",
    "",
    "Escape",
    "",
    "",
    "",
    "",
    "Space",
    "Page Up",
    "Page Down",
    "End",
    "Home",
    "Left",
    "Up",
    "Right",
    "Down",
    "",
    "",
    "",
    "Print",
    "Insert",
    "Delete",
    "",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "",
    "",
    "",
    "",
    "",
    "Numpad 0",
    "Numpad 1",
    "Numpad 2",
    "Numpad 3",
    "Numpad 4",
    "Numpad 5",
    "Numpad 6",
    "Numpad 7",
    "Numpad 8",
    "Numpad 9",
    "Multiply",
    "Add",
    "",
    "Subtract",
    "Decimal",
    "Divide",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
};
static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}
void HotkeyButton(int aimkey, void* changekey, int status)
{
    const char* preview_value = NULL;
    if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
        Items_ArrayGetter(keyNames, aimkey, &preview_value);

    std::string aimkeys;
    if (preview_value == NULL)
        aimkeys = ("Select Keys");
    else
        aimkeys = preview_value;

    if (status == 1)
    {
        aimkeys = ("Pressing Keys");
    }
    if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 20)))
    {
        if (status == 0)
        {
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
        }
    }
}
void ChangeKey(void* blank)
{
    keystatus = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetKeyState(i) & 0x8000)
            {
                aimkey = i;
                keystatus = 0;
                return;
            }
        }
    }
}
void ChangeKey3(void* blank)
{
    keystatus3 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetKeyState(i) & 0x8000)
            {
                aimkey3 = i;
                keystatus3 = 0;
                return;
            }
        }
    }
}

void ChangeKey4(void* blank)
{
    keystatus4 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetKeyState(i) & 0x8000)
            {
                aimkey4 = i;
                keystatus4 = 0;
                return;
            }
        }
    }
}

void ChangeKey5(void* blank)
{
    keystatus5 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetKeyState(i) & 0x8000)
            {
                aimkey5 = i;
                keystatus5 = 0;
                return;
            }
        }
    }
}

void ChangeKey6(void* blank)
{
    keystatus6 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetKeyState(i) & 0x8000)
            {
                aimkey6 = i;
                keystatus6 = 0;
                return;
            }
        }
    }
}


void ChangeKey7(void* blank)
{
    keystatus7 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetKeyState(i) & 0x8000)
            {
                aimkey7 = i;
                keystatus7 = 0;
                return;
            }
        }
    }
}

void ChangeKey1(void* blank)
{
    keystatus1 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetKeyState(i) & 0x8000)
            {
                aimkey1 = i;
                keystatus1 = 0;
                return;
            }
        }
    }
}

void ChangeKey2(void* blank)
{
    keystatus2 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetKeyState(i) & 0x8000)
            {
                aimkey2 = i;
                keystatus2 = 0;
                return;
            }
        }
    }
}


void initAngleX()
{
    for (int i = 1; i <= 5; i++)
    {
        angleX[i] = -83.8f;
    }
    for (int i = 6; i <= 44; i++)
    {
        angleX[i] = -(-(0.01 * (i * i)) + 84);
    }
    for (int i = 45; i <= 90; i++)
    {
        angleX[i] = -(-(0.008 * (i * i)) + 80);
    }
}

//直接就能用。而且很鸡巴准 只有手雷呗。对，所以我得重新写一套，而且我的四元数QAangle。还没改正
void Grenade()
{

    Vector3 muzzle = Read<Vector3>(ProcessId, LocalPlayer + OFFSET_CAMERAPOS);

    Vector3 EntityPos = Read<Vector3>(ProcessId, aimPtr + OFFSET_ORIGIN);
    Vector3 LocalPos = Read<Vector3>(ProcessId, LocalPlayer + OFFSET_ORIGIN);

    FLOAT dis = EntityPos.DistTo(LocalPos) * INCHES_TO_METERS;


    if (var::Grenade)
    {
        if ((GetAsyncKeyState(aimkey4) & 0x8000))
        {
            Vector3 viewAngles = Read<Vector3>(ProcessId, LocalPlayer + OFFSET_VIEWANGLES);

            viewAngles.x = angleX[(int)dis];

            Write<Vector2>(ProcessId, LocalPlayer + OFFSET_VIEWANGLES, { viewAngles.x, viewAngles.y });
        }

    }
}



